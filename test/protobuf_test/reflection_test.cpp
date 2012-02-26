
TEST(protobuf_test, reflection_test)
{
	const google::protobuf::Descriptor* descriptor = 
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("proto.test");
	ASSERT_TRUE(descriptor != nullptr);

	const google::protobuf::Message* message = 
		google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
	ASSERT_TRUE(message != nullptr);

	proto::test* test = dynamic_cast<proto::test*>(message->New());
	ASSERT_TRUE(test != nullptr);

	test->set_message("hello world");
	EXPECT_EQ(std::string("hello world"), test->message());
}