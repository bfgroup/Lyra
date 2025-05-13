#include "mini_test.hpp"
#include <lyra/lyra.hpp>
#include <iostream>

using ArgList = std::initializer_list<std::string>;

void test_terminator1(bfg::mini_test::scope& test, std::string test_value)
{
	using namespace lyra;
	{
		std::string default_value = "default1";
		std::string arg_value_s = default_value;
		std::string opt_value_s = default_value;

		auto cli = lyra::cli().style(lyra::option_style::posix_with_terminator())
			| arg(arg_value_s, "value")
			| opt(opt_value_s, "OPTION").name("--option");
		ArgList args = { "TestApp", "--", test_value.c_str() };
		auto result = cli.parse(args);
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(opt_value_s == default_value, "opt_value_s == " + default_value, CONTEXT);
		test(arg_value_s == test_value, "arg_value_s == " + test_value, CONTEXT);
	}
}


void test_terminator_no_terminator(bfg::mini_test::scope& test, std::string test_value)
{
	using namespace lyra;
	{
		std::string default_value = "default1";
		std::string arg_value_s = default_value;
		std::string opt_value_s = default_value;

		auto cli = lyra::cli().style(lyra::option_style::posix_with_terminator())
			| arg(arg_value_s, "value")
			| opt(opt_value_s, "OPTION").name("--option");
		ArgList args = { "TestApp", "--option", test_value.c_str() };
		auto result = cli.parse(args);
		if (!result) std::cerr << result.message() << '\n';
		test(REQUIRE(result));
		test(opt_value_s == test_value, "opt_value_s == " + test_value, CONTEXT);
		test(arg_value_s == default_value, "arg_value_s == " + default_value, CONTEXT);
	}
}


void test_terminator_unknown_option(bfg::mini_test::scope& test)
{
	using namespace lyra;
	{
		std::string default_value = "default1";
		std::string arg_value_s = default_value;
		std::string opt_value_s = default_value;

		auto cli = lyra::cli().style(lyra::option_style::posix_with_terminator())
			| arg(arg_value_s, "value")
			| opt(opt_value_s, "OPTION").name("--option");
		ArgList args = { "TestApp", "--unknown" };
		auto result = cli.parse(args);
		test(REQUIRE(!result));
	}
}


int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	test_terminator1(test, "test1");
	test_terminator1(test, "--test1");
	test_terminator1(test, "--option");
	test_terminator_no_terminator(test, "test1");
	test_terminator_no_terminator(test, "--test1"); // Q: should this also be an error?
	test_terminator_unknown_option(test); // unknown option must raise an error

	return test;
}
