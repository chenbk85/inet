
using ::testing::_;
using ::testing::Invoke;

class mock_handler
{
public:
	MOCK_METHOD0(on_event, void ());
};

TEST(dispatcher_test, function_test)
{
	inet::dispatcher<int, inet::delegate<>> d;
	mock_handler listener;

	d.add(1, boost::bind(&mock_handler::on_event, &listener));
	EXPECT_CALL(listener, on_event())
		.Times(1);
	
	d.dispatch(1);
}

TEST(dispatcher_test, multi_function_test)
{
	inet::dispatcher<int, inet::delegate<>> d;
	mock_handler listener;

	d.add(1, boost::bind(&mock_handler::on_event, &listener));
	d.add(1, boost::bind(&mock_handler::on_event, &listener));
	EXPECT_CALL(listener, on_event())
		.Times(2);

	d.dispatch(1);
}

TEST(dispatcher_test, remove_function_test)
{
	inet::dispatcher<int, inet::delegate<>> d;
	mock_handler listener;

	EXPECT_CALL(listener, on_event())
		.Times(3);

	auto handle1 = d.add(1, boost::bind(&mock_handler::on_event, &listener));
	{
		boost::signals::scoped_connection handle2 = d.add(1, boost::bind(&mock_handler::on_event, &listener));
		d.dispatch(1);
	}
	
	d.dispatch(1);
	handle1.disconnect();
	d.dispatch(1);
}