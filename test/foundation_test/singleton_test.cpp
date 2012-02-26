
namespace test {

struct inheritance_struct : public inet::singleton<inheritance_struct>
{
};

struct typedef_struct
{
};

typedef inet::singleton<typedef_struct> typedef_singleton;

}

TEST(singleton_test, unique_instance_test)
{
	test::inheritance_struct* inheritance = &test::inheritance_struct::instance();
	test::typedef_struct* typedefed = &test::typedef_singleton::instance();

	EXPECT_TRUE(nullptr != inheritance);
	EXPECT_TRUE(nullptr != typedefed);

	EXPECT_EQ(inheritance, &test::inheritance_struct::instance());	
	EXPECT_EQ(typedefed, &test::typedef_singleton::instance());
}

TEST(singleton_test, unique_instance_multithead_test)
{
	boost::barrier barrier(2);
	test::typedef_struct* s1 = nullptr;
	test::typedef_struct* s2 = nullptr; 

	boost::thread_group thread_group;
	thread_group.create_thread([&]{ barrier.wait(); s1 = &test::typedef_singleton::instance(); });
	thread_group.create_thread([&]{ barrier.wait(); s2 = &test::typedef_singleton::instance(); });

	thread_group.join_all();

	EXPECT_EQ(s1, s2);
}