
TEST(protobuf_test, basic_test)
{
	proto::test msg;
	msg.set_message("hello world");
	EXPECT_EQ(std::string("hello world"), msg.message());
}