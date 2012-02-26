#include <deque>
#include <boost/array.hpp>

struct test_struct
{
	int index;
	inet::uint16 value;
};

TEST(packet_test, pack_test)
{
	static const int buffer_size = 8192;
	inet::uint8 buf[buffer_size] = {0};

	{
		inet::int8 value = 1;
		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		inet::uint16 value = 1;
		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		int value = 1;
		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		inet::uint64 value = 1;
		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		test_struct value;
		value.index = 1;
		value.value = 1;

		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	

	{
		std::string str = "hello world";
		auto buffer = inet::pack(inet::buffer(buf), str);
		EXPECT_EQ(buffer_size - str.length() - sizeof(inet::uint16), inet::buffer_size(buffer));
	}

	{
		std::vector<int> array(5, 10);
		auto buffer = inet::pack(inet::buffer(buf), array);
		EXPECT_EQ(buffer_size - array.size() * sizeof(int) - sizeof(inet::uint16), inet::buffer_size(buffer));
	}

	{
		std::list<int> array(5, 10);
		auto buffer = inet::pack(inet::buffer(buf), array);
		EXPECT_EQ(buffer_size - array.size() * sizeof(int) - sizeof(inet::uint16), inet::buffer_size(buffer));
	}

	{
		std::deque<int> array(5, 10);
		auto buffer = inet::pack(inet::buffer(buf), array);
		EXPECT_EQ(buffer_size - array.size() * sizeof(int) - sizeof(inet::uint16), inet::buffer_size(buffer));
	}
}

TEST(packet_test, unpack_test)
{
	static const int buffer_size = 8192;
	inet::uint8 buf[buffer_size] = {0};

	{
		inet::int8 value = 1;
		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		inet::int8 value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(1, value);
	}

	{
		inet::uint16 value = 1;
		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		inet::uint16 value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(1, value);
	}

	{
		int value = 1;
		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		int value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(1, value);
	}

	{
		inet::uint64 value = 1;
		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		inet::uint64 value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(1, value);
	}

	{
		test_struct value;
		value.index = 1;
		value.value = 1;

		auto buffer = inet::pack(inet::buffer(buf), value);
		EXPECT_EQ(buffer_size - sizeof(value), inet::buffer_size(buffer));
	}

	{
		test_struct value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(1, value.index);
		EXPECT_EQ(1, value.value);
	}

	

	{
		std::string str = "hello world";
		auto buffer = inet::pack(inet::buffer(buf), str);
		EXPECT_EQ(buffer_size - str.length() - sizeof(inet::uint16), inet::buffer_size(buffer));
	}

	{
		std::string value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(std::string("hello world"), value);
	}

	{
		std::vector<int> array(5, 10);
		auto buffer = inet::pack(inet::buffer(buf), array);
		EXPECT_EQ(buffer_size - array.size() * sizeof(int) - sizeof(inet::uint16), inet::buffer_size(buffer));
	}

	{
		std::vector<int> value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(5, value.size());
		EXPECT_EQ(10, value.front());
	}

	{
		std::list<int> array(5, 10);
		auto buffer = inet::pack(inet::buffer(buf), array);
		EXPECT_EQ(buffer_size - array.size() * sizeof(int) - sizeof(inet::uint16), inet::buffer_size(buffer));
	}

	{
		std::list<int> value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(5, value.size());
		EXPECT_EQ(10, value.front());
	}

	{
		std::deque<int> array(5, 10);
		auto buffer = inet::pack(inet::buffer(buf), array);
		EXPECT_EQ(buffer_size - array.size() * sizeof(int) - sizeof(inet::uint16), inet::buffer_size(buffer));
	}

	{
		std::deque<int> value;
		auto buffer = inet::unpack(inet::buffer((const inet::uint8*)buf, buffer_size), value);
		EXPECT_EQ(5, value.size());
		EXPECT_EQ(10, value.front());
	}
}