/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "main_test.hpp"

#include "doc_commands.cpp"

#include "main_test.hpp"

int main()
{
	bfg::mini_test::scope test;
	{
		TEST_MAIN(test, "doc_commands_run_test", "--help");
		TEST_MAIN(test, "doc_commands_run_test", "run", "foo");
		TEST_MAIN(test, "doc_commands_run_test", "kill", "foo");
		TEST_MAIN(test, "doc_commands_run_test", "run", "-v", "foo");
		TEST_MAIN(test, "doc_commands_run_test", "kill", "-s", "9", "foo");
		TEST_MAIN_FAIL(test, "doc_commands_run_test", "jump");
		TEST_MAIN_FAIL(test, "doc_commands_run_test", "run");
		TEST_MAIN_FAIL(test, "doc_commands_run_test", "kill");
		TEST_MAIN_FAIL(test, "doc_commands_run_test", "kill", "-s", "foo");
		TEST_MAIN(test, "doc_commands_run_test", "kill", "--help");
	}
	return test;
}
