/*
Copyright 2021-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

#include <iostream>
#include <string>

int main()
{
	bfg::mini_test::scope test;

	bool someFlag = false;
	std::string requiredArgInGroup;
	std::string optionalArgInGroup;
	std::string positionalArg;

	auto cli = lyra::cli();
	cli |= lyra::opt(someFlag).name("-f");
	cli |= lyra::group([](const lyra::group &) {})
			   .add_argument(lyra::opt(requiredArgInGroup, "req")
								 .name("--req-in-group")
								 .required())
			   .add_argument(
				   lyra::opt(optionalArgInGroup, "opt").name("--opt-in-group"));
	cli |= lyra::arg(positionalArg, "positional");

	{
		someFlag = false;
		requiredArgInGroup = "";
		optionalArgInGroup = "";
		positionalArg = "";
		auto result = cli.parse(
			{ "test_app", "-f", "--req-in-group", "test1", "test2" });
		test(REQUIRE(result));
		test(REQUIRE(someFlag));
		test(REQUIRE(requiredArgInGroup == "test1"));
		test(REQUIRE(positionalArg == "test2"));
	}
	{
		someFlag = false;
		requiredArgInGroup = "";
		optionalArgInGroup = "";
		positionalArg = "";
		auto result = cli.parse(
			{ "test_app", "--req-in-group", "test1", "-f", "test2" });
		test(REQUIRE(result));
		test(REQUIRE(someFlag));
		test(REQUIRE(requiredArgInGroup == "test1"));
		test(REQUIRE(positionalArg == "test2"));
	}
	{
		someFlag = false;
		requiredArgInGroup = "";
		optionalArgInGroup = "";
		positionalArg = "";
		auto result
			= cli.parse({ "test_app", "--req-in-group", "test1", "-f" });
		test(REQUIRE(result));
		test(REQUIRE(requiredArgInGroup == "test1"));
		test(REQUIRE(someFlag));
		test(REQUIRE(positionalArg == ""));
	}

	return test;
}
