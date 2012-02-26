TEST(shared_buffer_test, raw_buffer_test)
{
	inet::uint8 buf[1000] = {0};
	inet::shared_buffer buffer(buf);

	buffer.remove(100);
	EXPECT_EQ(900, buffer.size());

	buffer.remove(850);
	EXPECT_EQ(50, buffer.size());

	buffer.remove(100);
	EXPECT_EQ(0, buffer.size());
}

TEST(shared_buffer_test, const_buffer_test)
{
	inet::uint8 buf[1000] = {0};
	inet::const_buffer input_buffer = inet::buffer(buf);
	inet::shared_buffer buffer(input_buffer);

	buffer.remove(100);
	EXPECT_EQ(900, buffer.size());

	buffer.remove(850);
	EXPECT_EQ(50, buffer.size());

	buffer.remove(100);
	EXPECT_EQ(0, buffer.size());
}

TEST(shared_buffer_test, mutable_buffer_test)
{
	inet::uint8 buf[1000] = {0};
	inet::mutable_buffer output_buffer = inet::buffer(buf);
	inet::shared_buffer buffer(output_buffer);

	buffer.remove(100);
	EXPECT_EQ(900, buffer.size());

	buffer.remove(850);
	EXPECT_EQ(50, buffer.size());

	buffer.remove(100);
	EXPECT_EQ(0, buffer.size());
}