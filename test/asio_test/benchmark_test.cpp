
using testing::_;
using testing::Invoke;

class mock_session_b : public inet::asio::session
{
};

class mock_acceptor_b : public inet::asio::acceptor
{
};

class mock_proactor_b : public inet::asio::proactor
{
public:
	explicit mock_proactor_b(int count)
		: inet::asio::proactor(count)
	{
	}

	inet::connector_ptr create_connector()
	{
		boost::shared_ptr<inet::asio::session> session(new mock_session_b);
		session->open(io_service_);

		create_connector_hooker_(session);
		return session;
	}

	inet::session_ptr create_session()
	{
		boost::shared_ptr<inet::asio::session> session(new mock_session_b);
		session->open(io_service_);

		create_session_hooker_(session);
		return session;
	}

	inet::acceptor_ptr create_acceptor()
	{
		boost::shared_ptr<inet::asio::acceptor> acceptor(new mock_acceptor_b);
		acceptor->open(io_service_, [this] { return create_session(); });

		create_acceptor_hooker_(acceptor);
		return acceptor;
	}

	inet::delegate<boost::shared_ptr<inet::asio::session>> create_connector_hooker_;
	inet::delegate<boost::shared_ptr<inet::asio::session>> create_session_hooker_;
	inet::delegate<boost::shared_ptr<inet::asio::acceptor>> create_acceptor_hooker_;
};

class multi_thread_connect_benchmark : public ::testing::TestWithParam<boost::tuple<int, int, int>> 
{
};

TEST_P(multi_thread_connect_benchmark, connect_test)
{
	int thread_count, time_out, connect_count;
	boost::tie(thread_count, time_out, connect_count) = GetParam();
	tbb::atomic<int> accepted_count;
	tbb::atomic<int> accept_failed_count;
	tbb::atomic<int> connected_count;
	tbb::atomic<int> connect_failed_count;
	accepted_count = 0;
	accept_failed_count = 0;
	connected_count = 0;
	connect_failed_count = 0;

	mock_proactor_b net(thread_count);
	
	net.run();

	net.create_connector_hooker_ += [&] (boost::shared_ptr<inet::asio::session> s) {
		boost::weak_ptr<inet::session> s_handle = s;
		s->on_connect += [&, s_handle] {
			++connected_count;
			s_handle.lock()->close();
		};
		s->on_error += [&] (const inet::error& ) {
			++connect_failed_count;
		};
	};

	net.create_acceptor_hooker_ += [&] (boost::shared_ptr<inet::asio::acceptor> a) {
		boost::shared_ptr<mock_acceptor_b> acceptor = boost::static_pointer_cast<mock_acceptor_b>(a);
		acceptor->on_connect += [&] (inet::session_ptr ) {
			++accepted_count;
		};
		acceptor->on_error += [&] (const inet::error& ) {
			++accept_failed_count;
		};
	};

	static boost::uint16_t port = 33533;
	net.listen(port);

	inet::timer_ptr end_timer = net.create_timer();
	end_timer->start([&] () ->bool {
		net.set_force_end();
		return false;
	}, time_out);

	inet::timer_ptr start_timer = net.create_timer();
	start_timer->start([&] () ->bool {
		for(int i = 0; i < connect_count; ++i) {
			net.connect(inet::end_point("localhost", port));
		}
		return false;
	}, 0);

	net.wait_end();
	port++;

	std::cout << "accepted count : " << accepted_count << std::endl;
//	std::cout << "accept failed count : " << accept_failed_count << std::endl;
	std::cout << "connected count : " << connected_count << std::endl;
//	std::cout << "connect failed count : " << connect_failed_count << std::endl;
}

class multi_thread_chat_benchmark : public ::testing::TestWithParam<boost::tuple<int, int>> 
{
};

TEST_P(multi_thread_chat_benchmark, chat_test)
{
	int thread_count, connect_count;
	boost::tie(thread_count, connect_count) = GetParam();

	tbb::atomic<int> accepted_count;
	tbb::atomic<int> accept_failed_count;
	tbb::atomic<int> received_count;
	tbb::atomic<int> connected_count;
	tbb::atomic<int> connect_failed_count;
	tbb::atomic<int> closed_count;
	accepted_count = 0;
	accept_failed_count = 0;
	received_count = 0;
	connected_count = 0;
	connect_failed_count = 0;
	closed_count = 0;

	mock_proactor_b net(thread_count);

	net.run();

	net.create_connector_hooker_ += [&] (boost::shared_ptr<inet::asio::session> s) {
		boost::weak_ptr<inet::session> s_handle = s;
		s->on_connect += [&] {
			++connected_count;
		};
		s->on_error += [&] (const inet::error& ) {
			++connect_failed_count;
		};

		s->on_data += [&, s_handle] (inet::shared_buffer ) {
			++received_count;
			s_handle.lock()->close();
		};
	};

	net.create_acceptor_hooker_ += [&] (boost::shared_ptr<inet::asio::acceptor> a) {
		boost::shared_ptr<mock_acceptor_b> acceptor = boost::static_pointer_cast<mock_acceptor_b>(a);
		acceptor->on_connect += [&] (inet::session_ptr s) {
			++accepted_count;
			s->on_close += [&] {
				if(connect_count <= ++closed_count) {
					net.set_force_end();
				}
			};

			msg::chat message;
			message.set_message("protobuf message");
			s->send(inet::protobuf::to_shared_buffer(message));
		};
		acceptor->on_error += [&] (const inet::error& ) {
			++accept_failed_count;
		};
	};	

	static boost::uint16_t port = 33633;
	net.listen(port);

	inet::timer_ptr start_timer = net.create_timer();
	start_timer->start([&] () ->bool {
		for(int i = 0; i < connect_count; ++i) {
			net.connect(inet::end_point("localhost", port));
		}
		return false;
	}, 0);

	net.wait_end();
	port++;

// 	std::cout << "accepted count : " << accepted_count << std::endl;
// 	std::cout << "accept failed count : " << accept_failed_count << std::endl;
// 	std::cout << "received count : " << received_count << std::endl;
// 	std::cout << "connected count : " << connected_count << std::endl;
// 	std::cout << "connect failed count : " << connect_failed_count << std::endl;
}



#ifdef _DEBUG

// <0> thread count [0 = default] <1> time(milliseconds) <2> connect count
INSTANTIATE_TEST_CASE_P(benchmark, multi_thread_connect_benchmark, ::testing::Values(
	boost::tuples::make_tuple(0, 500, 100)
	));

// <0> thread count [0 = default] <1> connect count <2> send count per connection
INSTANTIATE_TEST_CASE_P(benchmark, multi_thread_chat_benchmark, ::testing::Values(
	boost::tuples::make_tuple(0, 50)
	));

#else

// <0> thread count [0 = default] <1> time(milliseconds) <2> connect count
INSTANTIATE_TEST_CASE_P(benchmark, multi_thread_connect_benchmark, ::testing::Values(
	boost::tuples::make_tuple(0, 500, 100),
	boost::tuples::make_tuple(0, 500, 300),
	boost::tuples::make_tuple(0, 500, 500),
	boost::tuples::make_tuple(0, 999, 500),
	boost::tuples::make_tuple(0, 999, 999)
	));

// <0> thread count [0 = default] <1> connect count <2> send count per connection
INSTANTIATE_TEST_CASE_P(benchmark, multi_thread_chat_benchmark, ::testing::Values(
	boost::tuples::make_tuple(0, 500),
	boost::tuples::make_tuple(0, 600),
	boost::tuples::make_tuple(0, 700),
	boost::tuples::make_tuple(0, 800),
	boost::tuples::make_tuple(0, 900),
	boost::tuples::make_tuple(0, 999)
	));

#endif