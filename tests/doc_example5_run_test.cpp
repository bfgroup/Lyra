/*
Copyright Rene Rivera 2020
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "main_test.hpp"

#include "doc_example5.cpp"

#include "main_test.hpp"

int main()
{
	bfg::mini_test::scope test;
	{
		TEST_MAIN(test, "doc_example5", "--choice=red");
		TEST_MAIN_FAIL(test, "doc_example5", "--choice=purple");
	}
	return test;
}
