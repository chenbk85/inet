int _tmain(int argc, _TCHAR* argv[])
{
	::testing::GTEST_FLAG(throw_on_failure) = true;
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

