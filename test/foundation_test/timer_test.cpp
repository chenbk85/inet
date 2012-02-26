
using testing::_;
using testing::Invoke;

class mock_timer : public inet::timer
{
public:
	explicit mock_timer(boost::asio::io_service& io_service)
		: inet::timer(io_service)
	{
	}

	MOCK_METHOD1(on_timeout, void(const boost::system::error_code& ));
};

TEST(timer_test, basic_test)
{
	boost::asio::io_service io_service;

	boost::shared_ptr<mock_timer> timer(new mock_timer(io_service));
	timer->start([&] () ->bool { return false; }, 100);

	bool ended = false;
	EXPECT_CALL(*timer, on_timeout(_))
		.Times(1);
	ON_CALL(*timer, on_timeout(_))
		.WillByDefault(Invoke([&](const boost::system::error_code& ) {
			ended = true;
		}));

	while(!ended) {
		io_service.poll();
		boost::thread::yield();
	}
}