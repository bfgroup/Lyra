/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "main_test.hpp"

#include "doc_groups.cpp"

#include "main_test.hpp"

int main()
{
	bfg::mini_test::scope test;
	{
		TEST_MAIN(test, "doc_groups_run_test", "--help");
		TEST_MAIN(test, "doc_groups_run_test", "--width=1024", "--height=512");
		TEST_MAIN(test, "doc_groups_run_test", "--height=512", "--width=1024");
		TEST_MAIN(test, "doc_groups_run_test", "--aspect=1.5");
		TEST_MAIN_FAIL(test, "doc_groups_run_test", "--width=1024");
		TEST_MAIN_FAIL(test, "doc_groups_run_test", "--height=512");
		TEST_MAIN_FAIL(
			test, "doc_groups_run_test", "--height=512", "--aspect=1.5");
		TEST_MAIN_FAIL(
			test, "doc_groups_run_test", "--aspect=1.5", "--width=1024");
	}
	return test;
}
