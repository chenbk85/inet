
using testing::_;
using testing::Invoke;

class mock_timer
{
public:
	MOCK_METHOD0(on_timeout, void());
};

TEST(timer_test, basic_test)
{
	boost::asio::io_service io_service;

	inet::timer timer(io_service);
	mock_timer timer_handler;
	timer.async_wait(boost::bind(&mock_timer::on_timeout, &timer_handler), boost::chrono::milliseconds(100));

	bool ended = false;
	EXPECT_CALL(timer_handler, on_timeout())
		.Times(1);
	ON_CALL(timer_handler, on_timeout())
		.WillByDefault(Invoke([&] {
			ended = true;
	}));

	while(!ended) {
		io_service.poll();
		boost::thread::yield();
	}
}