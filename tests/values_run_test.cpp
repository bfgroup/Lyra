/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>
#include <iostream>

template <typename Value>
void test_val(bfg::mini_test::scope& test, Value test_value, Value default_value = Value())
{
	using namespace lyra;
	std::string test_value_s;
	lyra::detail::to_string(test_value, test_value_s);
	{
		Value arg_value { default_value };
		auto cli = lyra::cli() | arg(arg_value, "value");
		char* args[] = { (char*)"TestApp", (char*)test_value_s.c_str() };
		auto result = cli.parse({ 2, args });
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(arg_value == test_value, "arg_value == " + test_value_s, CONTEXT);
	}
	{
		Value arg_value { default_value };
		auto cli = lyra::cli() | opt(arg_value, "value").name("--value");
		char* args[]
			= { (char*)"TestApp", (char*)"--value", (char*)test_value_s.c_str() };
		auto result = cli.parse({ 3, args });
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(arg_value == test_value, "arg_value == " + test_value_s, CONTEXT);
	}
	{
		Value arg_value { default_value };
		auto cli = lyra::cli() | opt(arg_value, "value").name("--value");
		std::string value_arg = "--value=" + test_value_s;
		char* args[]
			= { (char*)"TestApp", (char*)value_arg.c_str() };
		auto result = cli.parse({ 2, args });
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(arg_value == test_value, "arg_value == " + test_value_s, CONTEXT);
	}
}

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	test_val(test, unsigned(1));
	test_val(test, int(1));
	test_val(test, int(-1));

	return test;
}
