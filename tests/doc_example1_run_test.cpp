/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "main_test.hpp"

#include "doc_example1.cpp"

#include "main_test.hpp"

int main()
{
	bfg::mini_test::scope test;
	{
		TEST_MAIN(test, "doc_example1", "-w", "100");
		TEST_MAIN(test, "doc_example1", "--width=100");
		TEST_MAIN_FAIL(test, "doc_example1", "what");
	}
	return test;
}
