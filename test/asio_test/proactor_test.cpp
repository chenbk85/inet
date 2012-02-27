#include <boost/tuple/tuple.hpp>

using testing::_;
using testing::Invoke;


struct mock_handler	{
	MOCK_METHOD0(on_connect, void ());
	MOCK_METHOD1(on_data, void (const inet::shared_buffer& ));
	MOCK_METHOD1(on_error, void (const inet::error& ));
	MOCK_METHOD0(on_close, void ());
};

class multi_connect : public ::testing::TestWithParam<boost::tuple<int, int>>
{
};

TEST_P(multi_connect, connect_test)
{
	int thread_count, repeat_count;
	boost::tie(thread_count, repeat_count) = GetParam();
	mock_handler handler;

	inet::proactor_ptr net(new inet::asio::proactor(thread_count));
	inet::timer_ptr end_timer = net->create_timer();

	tbb::atomic<int> closed_verify_count;
	closed_verify_count = repeat_count * 2;
	auto close_handler = [&] {
		if(--closed_verify_count <= 0) {
			end_timer->start([&] () -> bool {
				net->set_end();
				return false;
			}, 50);
		}
	};

	EXPECT_CALL(handler, on_connect())
		.Times(repeat_count);
	EXPECT_CALL(handler, on_close())
		.Times(repeat_count * 2);

	ON_CALL(handler, on_close())
		.WillByDefault(Invoke(close_handler));

	auto acceptor = net->listen(33333);
	acceptor->on_connection = [&] (inet::session_ptr session) {
		session->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
		session->on_data = boost::bind(&mock_handler::on_data, boost::ref(handler), _1);
	};

	inet::timer_ptr connect_timer = net->create_timer();
	connect_timer->start([&] () ->bool {
		for(int i = 0; i < repeat_count; ++i) {
			auto connector = net->connect(inet::end_point("localhost", 33333));
			connector->on_connect = boost::bind(&mock_handler::on_connect, boost::ref(handler));
			connector->on_connect += boost::bind(&inet::session::close, connector);
			connector->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
		}
		return false;
	}, 50);

	net->run();
	net->wait_end();
}

class multi_listen : public ::testing::TestWithParam<boost::tuple<int, int, int>>
{
};

TEST_P(multi_listen, listen_test)
{
	int thread_count, listen_count, repeat_count;
	boost::tie(thread_count, listen_count, repeat_count) = GetParam();
	mock_handler handler;

	inet::proactor_ptr net(new inet::asio::proactor(thread_count));
	inet::timer_ptr end_timer = net->create_timer();

	tbb::atomic<int> closed_verify_count;
	closed_verify_count = repeat_count * listen_count * 2;
	auto close_handler = [&] {
		if(--closed_verify_count <= 0) {
			end_timer->start([&] () -> bool {
				net->set_end();
				return false;
			}, 50);
		}
	};

	EXPECT_CALL(handler, on_connect())
		.Times(repeat_count * listen_count);
	EXPECT_CALL(handler, on_close())
		.Times(repeat_count * listen_count * 2);

	ON_CALL(handler, on_close())
		.WillByDefault(Invoke(close_handler));

	inet::uint16 start_port = 33333;
	for(inet::uint16 i = 0, port = start_port; i < listen_count; ++i, ++port)
	{
		auto acceptor = net->listen(port);
		acceptor->on_connection = [&] (inet::session_ptr session) {
			session->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
			session->on_data = boost::bind(&mock_handler::on_data, boost::ref(handler), _1);
		};
	}	

	inet::timer_ptr connect_timer = net->create_timer();
	connect_timer->start([&] () ->bool {
		for(int i = 0; i < repeat_count; ++i) {
			for(inet::uint16 i = 0, port = start_port; i < listen_count; ++i, ++port) {
				auto connector = net->connect(inet::end_point("localhost", port));
				connector->on_connect = boost::bind(&mock_handler::on_connect, boost::ref(handler));
				connector->on_connect += boost::bind(&inet::session::close, connector);
				connector->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
			}
		}
		return false;
	}, 50);

	net->run();
	net->wait_end();
}


/*
class multi_chat : public ::testing::TestWithParam<boost::tuple<int, int>>
{
};

TEST_P(multi_chat, chat_test)
{
	int thread_count, repeat_count;
	boost::tie(thread_count, repeat_count) = GetParam();

	boost::scoped_ptr<mock_proactor> net(new mock_proactor(thread_count));
	net->run();

	inet::timer_ptr end_timer = net->create_timer();
	
	tbb::atomic<int> closed_verify_count;
	closed_verify_count = repeat_count * 2;
	auto close_handler = [&] {
		if(--closed_verify_count <= 0) {
			end_timer->start([&] () -> bool {
				net->set_end();
				return false;
			}, 50);
		}
	};

	net->create_connector_hooker_ += [&] (boost::shared_ptr<inet::asio::session> s) {
		auto session = boost::static_pointer_cast<mock_session>(s);
		boost::weak_ptr<mock_session> session_handle = session;

		EXPECT_CALL(*session, on_mock_connect())
			.Times(1);
		EXPECT_CALL(*session, on_mock_close())
			.Times(1);
		EXPECT_CALL(*session, on_mock_data(_))
			.Times(1);

		ON_CALL(*session, on_mock_close())
			.WillByDefault(Invoke(close_handler));
		ON_CALL(*session, on_mock_data(_))
			.WillByDefault(Invoke([=] (const inet::shared_buffer& data) {
				ASSERT_EQ(session_handle.expired(), false);
				auto s = session_handle.lock();
				s->dispatcher_.dispatch(data);
				s->close();
			}));

		session->dispatcher_.add<msg::chat>([&] (const msg::chat& msg) {
			EXPECT_EQ(std::string("protobuf message"), msg.message());
		});		
	};

	tbb::atomic<int> expect_call_count;
	expect_call_count = repeat_count;
	net->create_session_hooker_ += [&] (boost::shared_ptr<inet::asio::session> ) {
	};

	net->create_acceptor_hooker_ += [&] (boost::shared_ptr<inet::asio::acceptor> a) {
		auto acceptor = boost::static_pointer_cast<mock_acceptor>(a);
		EXPECT_CALL(*acceptor, on_mock_connect(_))
			.Times(repeat_count);
		EXPECT_CALL(*acceptor, on_mock_close())
			.Times(1);
		ON_CALL(*acceptor, on_mock_connect(_))
			.WillByDefault(Invoke([&] (inet::session_ptr s) {
				auto session = boost::static_pointer_cast<mock_session>(s);
				EXPECT_CALL(*session, on_mock_close())
					.Times(1);
				ON_CALL(*session, on_mock_close())
					.WillByDefault(Invoke(close_handler));

				msg::chat message;
				message.set_message("protobuf message");
				session->send(message);
			}));
	};

	net->listen(33333);

	inet::timer_ptr connect_timer = net->create_timer();
	connect_timer->start([&] () ->bool {
		for(int i = 0; i < repeat_count; ++i) {
			net->connect(inet::end_point("localhost", 33333));
		}
		return false;
	}, 50);

	net->wait_end();
}
*/

INSTANTIATE_TEST_CASE_P(stable_test, multi_connect, ::testing::Values(
	boost::tuples::make_tuple(1, 1),
 	boost::tuples::make_tuple(1, 10),
 	boost::tuples::make_tuple(1, 20)
	));

INSTANTIATE_TEST_CASE_P(stable_test, multi_listen, ::testing::Values(
	boost::tuples::make_tuple(1, 2, 1),
 	boost::tuples::make_tuple(1, 3, 10),
 	boost::tuples::make_tuple(1, 4, 20)
	));
/*
INSTANTIATE_TEST_CASE_P(stable_test, multi_chat, ::testing::Values(
	boost::tuples::make_tuple(1, 1),
	boost::tuples::make_tuple(1, 10),
	boost::tuples::make_tuple(1, 20)
	));
*/