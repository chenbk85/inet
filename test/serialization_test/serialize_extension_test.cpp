#include <vector>
#include <list>
#include <deque>
#include <string>
#include <unordered_map>
#include <boost/array.hpp>

TEST(serialize_extension_test, save_test)
{
	using namespace inet;

	char buf[1024] = {};
	mutable_buffer buffer = inet::buffer(buf);

	{
		std::string value = "hello world";
		buffer = buffer & value;
		EXPECT_EQ(1011, buffer_size(buffer));
	}

	{
		std::wstring value = L"hello world";
		buffer = buffer & value;
		EXPECT_EQ(987, buffer_size(buffer));
	}

	{
		std::vector<int> value(5, 10);
		buffer = buffer & value;
		EXPECT_EQ(965, buffer_size(buffer));
	}

	{
		std::list<int> value(5, 10);
		buffer = buffer & value;
		EXPECT_EQ(943, buffer_size(buffer));
	}

	{
		std::deque<int> value(5, 10);
		buffer = buffer & value;
		EXPECT_EQ(921, buffer_size(buffer));
	}

	{
		std::map<inet::uint8, inet::uint16> value;
		value.insert(std::make_pair(1, 1));
		buffer = buffer & value;
		EXPECT_EQ(916, buffer_size(buffer));
	}

	{
		std::tr1::unordered_map<inet::uint8, inet::uint16> value;
		value.insert(std::make_pair(1, 1));
		buffer = buffer & value;
		EXPECT_EQ(911, buffer_size(buffer));
	}

	{
		inet::uint32 array[5];
		buffer = buffer & array;
		EXPECT_EQ(889, buffer_size(buffer));
	}

	{
		boost::array<int, 5> array;
		buffer = buffer & array;
		EXPECT_EQ(867, buffer_size(buffer));
	}
}

TEST(serialize_extension_test, load_test)
{
	using namespace inet;

	char buf[1024] = {};
	{
		mutable_buffer buffer = inet::buffer(buf);

		{
			std::string value = "hello world";
			buffer = buffer & value;
		}

		{
			std::wstring value = L"hello world";
			buffer = buffer & value;
		}

		{
			std::vector<int> value(5, 10);
			buffer = buffer & value;
		}

		{
			std::list<int> value(5, 10);
			buffer = buffer & value;
		}

		{
			std::deque<int> value(5, 10);
			buffer = buffer & value;
		}

		{
			std::map<inet::uint8, inet::uint16> value;
			value.insert(std::make_pair(1, 1));
			buffer = buffer & value;
		}

		{
			std::tr1::unordered_map<inet::uint8, inet::uint16> value;
			value.insert(std::make_pair(1, 1));
			buffer = buffer & value;
		}

		{
			inet::uint32 array[5] = {1, 2, 3, 4, 5};
			buffer = buffer & array;
		}

		{
			boost::array<int, 5> array = {1, 2, 3, 4, 5};
			buffer = buffer & array;
		}
	}

	{
		const_buffer buffer = inet::buffer(buf);

		{
			std::string value;
			buffer = buffer & value;
			EXPECT_STREQ("hello world", value.c_str());
		}

		{
			std::wstring value;
			buffer = buffer & value;
			EXPECT_STREQ(L"hello world", value.c_str());
		}

		{
			std::vector<int> value;
			buffer = buffer & value;

			EXPECT_EQ(5, value.size());			
		}

		{
			std::list<int> value;
			buffer = buffer & value;
			EXPECT_EQ(5, value.size());
		}

		{
			std::deque<int> value;
			buffer = buffer & value;
			EXPECT_EQ(5, value.size());
		}

		{
			std::map<inet::uint8, inet::uint16> value;
			buffer = buffer & value;
			EXPECT_EQ(1, value.size());
		}

		{
			std::tr1::unordered_map<inet::uint8, inet::uint16> value;
			buffer = buffer & value;
			EXPECT_EQ(1, value.size());
		}

		{
			inet::uint32 array[5] = {0};
			buffer = buffer & array;
			EXPECT_EQ(1, array[0]);
			EXPECT_EQ(2, array[1]);
			EXPECT_EQ(3, array[2]);
			EXPECT_EQ(4, array[3]);
			EXPECT_EQ(5, array[4]);
		}

		{
			boost::array<int, 5> array;
			buffer = buffer & array;
			EXPECT_EQ(1, array[0]);
			EXPECT_EQ(2, array[1]);
			EXPECT_EQ(3, array[2]);
			EXPECT_EQ(4, array[3]);
			EXPECT_EQ(5, array[4]);
		}
	}
}
