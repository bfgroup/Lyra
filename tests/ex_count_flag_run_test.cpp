/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "main_test.hpp"

#include "count_flag.cpp"

#include "main_test.hpp"

int main()
{
	bfg::mini_test::scope test;
	{
		TEST_MAIN(test, "count_flag");
		TEST_MAIN(test, "count_flag", "-v");
		TEST_MAIN(test, "count_flag", "-v", "-v", "-v");
		TEST_MAIN(test, "count_flag", "-vvvvv");
		TEST_MAIN_FAIL(test, "count_flag", "-vvvvvvvvvv");
	}
	return test;
}
