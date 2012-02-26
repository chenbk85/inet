
TEST(protobuf_test, pack_test)
{
	inet::shared_buffer buffer;
	{
		proto::test msg;
		msg.set_message("hello world");
		EXPECT_EQ(std::string("hello world"), msg.message());

		buffer = inet::protobuf::to_shared_buffer(msg);
	}

	{
		std::string id;
		auto remain_buffer = inet::unpack(inet::buffer(buffer), id);
		buffer.remove(buffer.size() - inet::buffer_size(remain_buffer));
		boost::shared_ptr<proto::test> msg = inet::protobuf::to_message<proto::test>(buffer);
		EXPECT_EQ(std::string("hello world"), msg->message());
	}
}