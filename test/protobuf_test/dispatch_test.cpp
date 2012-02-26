
using testing::_;
using testing::Invoke;

struct mock_handler
{
	MOCK_METHOD1(on_test, void (const proto::test& ));
};

TEST(protobuf_test, dispatch_test)
{
	mock_handler handler;
	inet::protobuf::dispatcher dispatcher;

	dispatcher.add<proto::test>(boost::bind(&mock_handler::on_test, boost::ref(handler), _1));

	EXPECT_CALL(handler, on_test(_))
		.Times(1);
	ON_CALL(handler, on_test(_))
		.WillByDefault(Invoke([&] (const proto::test& data) {
			EXPECT_EQ(std::string("hello world"), data.message());
		}));

	proto::test msg;
	msg.set_message("hello world");
	EXPECT_EQ(std::string("hello world"), msg.message());

	dispatcher.dispatch(inet::protobuf::to_shared_buffer(msg));
}