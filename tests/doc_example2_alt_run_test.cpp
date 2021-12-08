/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "main_test.hpp"

#include "doc_example2_alt.cpp"

#include "main_test.hpp"

int main()
{
	bfg::mini_test::scope test;
	{
		TEST_MAIN(test, "doc_example2_alt", "-w", "100");
		TEST_MAIN(test, "doc_example2_alt", "--width=100");
		TEST_MAIN(test, "doc_example2_alt", "-d", "-n", "thing");
		TEST_MAIN_FAIL(test, "doc_example2_alt", "what");
	}
	return test;
}
