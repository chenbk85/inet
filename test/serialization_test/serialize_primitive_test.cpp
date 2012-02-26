TEST(serialize_primitive_test, save_test)
{
	using namespace inet;

	char buf[1024] = {};
	mutable_buffer buffer = inet::buffer(buf);

	{
		int value = 10;
		buffer = buffer & value;
		EXPECT_EQ(1020, buffer_size(buffer));
	}

	{
		int8 value = 10;
		buffer = buffer & value;
		EXPECT_EQ(1019, buffer_size(buffer));
	}

	{
		uint16 value = 10;
		buffer = buffer & value;
		EXPECT_EQ(1017, buffer_size(buffer));
	}

	{
		int64 value = 10;
		buffer = buffer & value;
		EXPECT_EQ(1009, buffer_size(buffer));
	}
}

TEST(serialize_primitive_test, load_test)
{
	using namespace inet;

	char buf[15] = {};

	{
		mutable_buffer buffer = inet::buffer(buf);

		{
			int value = 10;
			buffer = buffer & value;
		}

		{
			int8 value = 10;
			buffer = buffer & value;
		}

		{
			uint16 value = 10;
			buffer = buffer & value;
		}

		{
			int64 value = 10;
			buffer = buffer & value;
		}
	}


	{
		const_buffer buffer = inet::buffer(buf);

		{
			int value = 0;
			buffer = buffer & value;
			EXPECT_EQ(10, value);
		}

		{
			int8 value = 0;
			buffer = buffer & value;
			EXPECT_EQ(10, value);
		}

		{
			uint16 value = 0;
			buffer = buffer & value;
			EXPECT_EQ(10, value);
		}

		{
			int64 value = 0;
			buffer = buffer & value;
			EXPECT_EQ(10, value);
		}
	}
}