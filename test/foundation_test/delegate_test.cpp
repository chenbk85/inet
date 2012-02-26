
using ::testing::_;
using ::testing::Invoke;

class notifier
{
public:
	inet::delegate<notifier, int> event_;
};

class mock_listener
{
public:
	MOCK_METHOD2(on_event, void (const notifier& , int));
};

TEST(delegate_test, function_test)
{
	notifier sender;
	mock_listener observer;

	auto handle = sender.event_.add(boost::bind(&mock_listener::on_event, &observer, _1, _2));

	EXPECT_CALL(observer, on_event(_, 1))
		.Times(1);
	EXPECT_CALL(observer, on_event(_, 2))
		.Times(1);

	sender.event_(sender, 1);
	sender.event_(sender, 2);
	sender.event_.remove(handle);
	sender.event_(sender, 1);
	sender.event_(sender, 2);
}

TEST(delegate_test, multi_function_test)
{
	notifier sender;
	mock_listener observer;

	auto handle = (sender.event_ = boost::bind(&mock_listener::on_event, &observer, _1, _2))
		.add(boost::bind(&mock_listener::on_event, &observer, _1, _2));

	EXPECT_CALL(observer, on_event(_, 1))
		.Times(2);
	EXPECT_CALL(observer, on_event(_, 2))
		.Times(1);
	EXPECT_CALL(observer, on_event(_, 3))
		.Times(1);

	sender.event_(sender, 1);
	sender.event_.remove(handle);
	sender.event_(sender, 2);

	sender.event_ = boost::bind(&mock_listener::on_event, &observer, _1, _2);
	sender.event_(sender, 3);
}