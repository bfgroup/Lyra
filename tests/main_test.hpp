/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#undef main

#ifndef BFG_MAIN_TEST_HPP
#define BFG_MAIN_TEST_HPP

#include "mini_test.hpp"

int test_main_f(int argc, const char** argv);

namespace bfg
{
namespace mini_test
{
	template <class... A>
	scope & test_main(scope & s, char const* file, int line, bool success, A... args)
	{
		const char * main_args[] = { args..., nullptr };
		int main_argc = 0;
		while (main_args[main_argc]) main_argc += 1;
		std::string condition = "main(" + std::to_string(main_argc) + ", {";
		for (int a = 0; main_args[a]; ++a)
		{
			condition += "\"";
			condition += main_args[a];
			condition += "\", ";
		}
		condition += "0})";
		s(test_main_f(main_argc, main_args) == 0 ? success : !success, condition.c_str(), file, line);
		return s;
	}
}
}
#define main test_main_f
#define TEST_MAIN(S,...) test_main(S, __FILE__, __LINE__, true, __VA_ARGS__)
#define TEST_MAIN_FAIL(S,...) test_main(S, __FILE__, __LINE__, false, __VA_ARGS__)

#endif
