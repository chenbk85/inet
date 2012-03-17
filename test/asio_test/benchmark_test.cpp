
using testing::_;
using testing::Invoke;


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

	inet::proactor_ptr net(new inet::asio::proactor(thread_count));
	
	auto acceptor = net->listen(33533, [&] (inet::session_ptr ) { ++accepted_count; });
	acceptor->on_error = [&] (const inet::error& ) { ++accept_failed_count; };

	for(int i = 0; i < connect_count; ++i) {
		auto connector = net->connect(inet::end_point("localhost", 33533), [&] { ++connected_count; });
		connector->on_error = [&] (const inet::error& ) { ++connect_failed_count; };
	}

	net->set_timeout([&] {
		net->set_end();
	}, boost::chrono::milliseconds(time_out));

	net->wait_end();

	std::cout << "accepted : accept failed : connected : connect failed" << std::endl;
	std::cout << accepted_count << " : " << accept_failed_count << " : " << connected_count << " : " << connect_failed_count << std::endl;
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

	inet::proactor_ptr net(new inet::asio::proactor(thread_count));

	auto acceptor = net->listen(33633, [&] (inet::session_ptr session) { 
		++accepted_count;
		session->on_close = [&] {
			if(connect_count <= ++closed_count) {
				net->set_end();
			}
		};
		msg::chat message;
		message.set_message("protobuf message");
		session->send(inet::protobuf::to_shared_buffer(message));
	});
	acceptor->on_error = [&] (const inet::error& ) { ++accept_failed_count; };

	for(int i = 0; i < connect_count; ++i) {
		auto connector = net->connect(inet::end_point("localhost", 33633), [&] { ++connected_count; });
		inet::connector_handle handle = connector;
		connector->on_data = [&, handle] (const inet::shared_buffer& ) { 
			++received_count;
			if(inet::connector_ptr connector = handle.lock()) {
				connector->close();
			}
		};
		connector->on_error = [&] (const inet::error& ) { ++connect_failed_count; };
	}

	net->wait_end();

	std::cout << "accepted : accept failed : received count : connected : connect failed" << std::endl;
	std::cout << accepted_count << " : " << accept_failed_count << " : " << received_count << " : " << connected_count << " : " << connect_failed_count << std::endl;
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
