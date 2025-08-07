/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

#include <iostream>
#include <vector>

template <typename Value>
void test_val(bfg::mini_test::scope & test,
	Value test_value,
	Value default_value = Value())
{
	using namespace lyra;
	std::string test_value_s;
	lyra::detail::to_string(test_value, test_value_s);
	{
		Value arg_value { default_value };
		auto cli = lyra::cli() | arg(arg_value, "value");
		char * args[] = { (char *)"TestApp", (char *)test_value_s.c_str() };
		auto result = cli.parse({ 2, args });
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(arg_value == test_value, "arg_value == " + test_value_s, CONTEXT);
	}
	{
		Value arg_value { default_value };
		auto cli = lyra::cli() | opt(arg_value, "value").name("-v");
		char * args[]
			= { (char *)"TestApp", (char *)"-v", (char *)test_value_s.c_str() };
		auto result = cli.parse({ 3, args });
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(arg_value == test_value, "arg_value == " + test_value_s, CONTEXT);
	}
	{
		Value arg_value { default_value };
		auto cli = lyra::cli() | opt(arg_value, "value").name("-v");
		auto arg_s = "-v" + test_value_s;
		char * args[] = { (char *)"TestApp", (char *)arg_s.c_str() };
		auto result = cli.parse({ 2, args });
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(arg_value == test_value, "arg_value == " + test_value_s, CONTEXT);
	}
	{
		std::string arg_value = "VALUE=" + test_value_s;
		auto cli = lyra::cli() | opt(arg_value, "value").name("-v");
		std::string value_arg = "-v" + arg_value;
		char * args[] = { (char *)"TestApp", (char *)value_arg.c_str() };
		auto result = cli.parse({ 2, args });
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(arg_value == ("VALUE=" + test_value_s),
			"arg_value == VALUE=" + test_value_s, CONTEXT);
	}
	{
		std::string v_value = "value=" + test_value_s;
		std::string v_arg = "-v" + v_value;
		std::string u_value = "value@" + test_value_s;
		std::string u_arg = "-u" + u_value;
		std::vector<std::string> values;
		auto cli = lyra::cli() | opt(values, "value").name("-v").name("-u");
		char * args[] = { (char *)"TestApp", (char *)u_arg.c_str(),
			(char *)v_arg.c_str() };
		auto result = cli.parse({ 3, args });
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(REQUIRE(values.size() == 2));
		test(REQUIRE(values.size() == 2 && values[0] == u_value));
		test(REQUIRE(values.size() == 2 && values[1] == v_value));
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
