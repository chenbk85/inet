namespace test
{

struct vector : public inet::tag::serialize
{
	int x;
	int y;
	int z;

	template <typename Archive>
	Archive serialize(Archive& ar)
	{
		return ar & x & y & z;
	}
};

}

TEST(serialize_member_test, save_test)
{
	using namespace inet;

	char buf[1024] = {};
	mutable_buffer buffer = inet::buffer(buf);

	{
		test::vector v;
		v.x = 1;
		v.y = 2;
		v.z = 3;

		buffer = buffer & v;
		EXPECT_EQ(1012, buffer_size(buffer));
	}
}

TEST(serialize_member_test, load_test)
{
	using namespace inet;

	char buf[1024] = {};

	{
		test::vector v;
		v.x = 1;
		v.y = 2;
		v.z = 3;

		mutable_buffer buffer = inet::buffer(buf);
		buffer = buffer & v;
		EXPECT_EQ(1012, buffer_size(buffer));
	}

	{
		test::vector v;

		const_buffer buffer = inet::buffer(buf);
		buffer = buffer & v;
		EXPECT_EQ(1, v.x);
		EXPECT_EQ(2, v.y);
		EXPECT_EQ(3, v.z);
	}
}


namespace test
{

struct vector_split_impl : public inet::tag::serialize_split
{
	int x;
	int y;
	int z;

	template <typename Archive>
	Archive save(Archive& ar) const
	{
		return ar & x & y & z;
	}

	template <typename Archive>
	Archive load(Archive& ar)
	{
		int temp;
		return ar & x & y & temp;
	}
};

}

TEST(serialize_member_test, split_save_test)
{
	using namespace inet;

	char buf[1024] = {};
	mutable_buffer buffer = inet::buffer(buf);

	{
		test::vector_split_impl v;
		v.x = 1;
		v.y = 2;
		v.z = 3;

		buffer = buffer & v;
		EXPECT_EQ(1012, buffer_size(buffer));
	}
}

TEST(serialize_member_test, split_load_test)
{
	using namespace inet;

	char buf[1024] = {};

	{
		test::vector_split_impl v;
		v.x = 1;
		v.y = 2;
		v.z = 3;

		mutable_buffer buffer = inet::buffer(buf);
		buffer = inet::pack(buffer, v);
		EXPECT_EQ(1012, buffer_size(buffer));
	}

	{
		test::vector_split_impl v;
		v.x = 0; v.y = 0; v.z = 0;

		const_buffer buffer = inet::buffer(buf);
		buffer = buffer & v;
		EXPECT_EQ(1, v.x);
		EXPECT_EQ(2, v.y);
		EXPECT_EQ(0, v.z);
	}
}