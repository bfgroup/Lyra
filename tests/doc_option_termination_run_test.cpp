/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "main_test.hpp"

#include "option_termination.cpp"

#include "main_test.hpp"

int main()
{
	bfg::mini_test::scope test;
	{
		TEST_MAIN(test, "doc_option_termination_run_test", "--help");
		TEST_MAIN(test, "doc_option_termination_run_test", "--echo", "--",
			"one", "two");
		TEST_MAIN_FAIL(
			test, "doc_option_termination_run_test", "--echo", "one", "two");
	}
	return test;
}
