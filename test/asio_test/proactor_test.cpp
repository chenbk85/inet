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
	
	tbb::atomic<int> closed_verify_count;
	closed_verify_count = repeat_count * 2;
	auto close_handler = [&] {
		if(--closed_verify_count <= 0) {
			net->set_end();
		}
	};

	EXPECT_CALL(handler, on_connect())
		.Times(repeat_count);
	EXPECT_CALL(handler, on_close())
		.Times(repeat_count * 2);

	ON_CALL(handler, on_close())
		.WillByDefault(Invoke(close_handler));

	net->listen(33333, [&] (inet::session_ptr session) {
		session->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
		session->on_data = boost::bind(&mock_handler::on_data, boost::ref(handler), _1);
	});

	for(int i = 0; i < repeat_count; ++i) {
		auto connector = net->connect(inet::end_point("localhost", 33333));
		connector->on_connect = boost::bind(&mock_handler::on_connect, boost::ref(handler));
		connector->on_connect += boost::bind(&inet::session::close, connector);
		connector->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
	}

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
	
	tbb::atomic<int> closed_verify_count;
	closed_verify_count = repeat_count * listen_count * 2;
	auto close_handler = [&] {
		if(--closed_verify_count <= 0) {
			net->set_end();
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
		net->listen(port, [&] (inet::session_ptr session) {
			session->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
			session->on_data = boost::bind(&mock_handler::on_data, boost::ref(handler), _1);
		});
	}	

	for(int i = 0; i < repeat_count; ++i) {
		for(inet::uint16 i = 0, port = start_port; i < listen_count; ++i, ++port) {
			auto connector = net->connect(inet::end_point("localhost", port));
			connector->on_connect = boost::bind(&mock_handler::on_connect, boost::ref(handler));
			connector->on_connect += boost::bind(&inet::session::close, connector);
			connector->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
		}
	}

	net->wait_end();
}



class multi_chat : public ::testing::TestWithParam<boost::tuple<int, int>>
{
};

TEST_P(multi_chat, chat_test)
{
	int thread_count, repeat_count;
	boost::tie(thread_count, repeat_count) = GetParam();

	mock_handler handler;

	inet::proactor_ptr net(new inet::asio::proactor(thread_count));

	tbb::atomic<int> closed_verify_count;
	closed_verify_count = repeat_count * 2;
	auto close_handler = [&] {
		if(--closed_verify_count <= 0) {
			net->set_end();
		}
	};

	EXPECT_CALL(handler, on_connect())
		.Times(repeat_count);
	EXPECT_CALL(handler, on_close())
		.Times(repeat_count * 2);
	EXPECT_CALL(handler, on_data(_))
		.Times(repeat_count * 2);

	ON_CALL(handler, on_close())
		.WillByDefault(Invoke(close_handler));

	net->listen(33333, [&] (inet::session_ptr session) {
		inet::session_handle handle = session;
		session->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
		session->on_data = boost::bind(&mock_handler::on_data, boost::ref(handler), _1);
		session->on_data += [=] (const inet::shared_buffer& buffer) {
			if(auto session = handle.lock()) {
				session->send(buffer);
			}
		};
	});

	for(int i = 0; i < repeat_count; ++i) {
		auto connector = net->connect(inet::end_point("localhost", 33333));
		inet::connector_handle handle = connector;
		connector->on_connect = boost::bind(&mock_handler::on_connect, boost::ref(handler));
		connector->on_connect += [=] {
			if(auto connector = handle.lock()) {
				msg::chat message;
				message.set_message("protobuf message");
				connector->send(inet::protobuf::to_shared_buffer(message));
			}
		};
		connector->on_data = boost::bind(&mock_handler::on_data, boost::ref(handler), _1);
		connector->on_data += boost::bind(&inet::session::close, connector);
		connector->on_close = boost::bind(&mock_handler::on_close, boost::ref(handler));
	}

	net->wait_end();
}


INSTANTIATE_TEST_CASE_P(stable_test, multi_connect, ::testing::Values(
	boost::tuples::make_tuple(1, 1),
 	boost::tuples::make_tuple(1, 10),
 	boost::tuples::make_tuple(1, 20),
	boost::tuples::make_tuple(4, 1),
	boost::tuples::make_tuple(3, 10),
	boost::tuples::make_tuple(2, 20)
	));
	
INSTANTIATE_TEST_CASE_P(stable_test, multi_listen, ::testing::Values(
	boost::tuples::make_tuple(1, 2, 1),
 	boost::tuples::make_tuple(1, 3, 10),
 	boost::tuples::make_tuple(1, 4, 20),
	boost::tuples::make_tuple(4, 2, 1),
	boost::tuples::make_tuple(3, 3, 10),
	boost::tuples::make_tuple(2, 4, 20)
	));

INSTANTIATE_TEST_CASE_P(stable_test, multi_chat, ::testing::Values(
	boost::tuples::make_tuple(1, 1),
	boost::tuples::make_tuple(1, 10),
	boost::tuples::make_tuple(1, 20),
	boost::tuples::make_tuple(4, 1),
	boost::tuples::make_tuple(3, 10),
	boost::tuples::make_tuple(2, 20)
	));
