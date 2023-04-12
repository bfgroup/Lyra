/*
Copyright 2023 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

#include <vector>

int main()
{
	bfg::mini_test::scope test;

	bool gotArgs = false;
	std::vector<std::string> args;
	{
		auto cli = lyra::cli().add_argument(lyra::group().sequential()
			| lyra::opt(gotArgs)["--args"] | lyra::arg(args, "args"));

		{
			gotArgs = false;
			args.clear();
			auto result = cli.parse({ "test_app" });
			test(REQUIRE(result));
			test(REQUIRE(!gotArgs));
			test(REQUIRE(args.empty()));
		}
		{
			gotArgs = false;
			args.clear();
			auto result = cli.parse({ "test_app", "--args", "a0", "a1" });
			test(REQUIRE(result));
			test(REQUIRE(gotArgs));
			test(REQUIRE(!args.empty()));
			test(REQUIRE(args.size() == 2));
		}
		{
			gotArgs = false;
			args.clear();
			auto result = cli.parse({ "test_app", "--args" });
			test(REQUIRE(result));
			test(REQUIRE(gotArgs));
			test(REQUIRE(args.empty()));
		}
		{
			gotArgs = false;
			args.clear();
			auto result = cli.parse({ "test_app", "a0", "a1" });
			test(REQUIRE(result));
			test(REQUIRE(!gotArgs));
			test(REQUIRE(args.size() == 2));
		}
	}
	{
		auto cli = lyra::cli().add_argument(lyra::group().sequential()
			| lyra::opt(gotArgs)["--args"].required()
			| lyra::arg(args, "args"));

		{
			gotArgs = false;
			args.clear();
			auto result = cli.parse({ "test_app" });
			test(REQUIRE(result));
			test(REQUIRE(!gotArgs));
			test(REQUIRE(args.empty()));
		}
		{
			gotArgs = false;
			args.clear();
			auto result = cli.parse({ "test_app", "--args", "a0", "a1" });
			test(REQUIRE(result));
			test(REQUIRE(gotArgs));
			test(REQUIRE(!args.empty()));
			test(REQUIRE(args.size() == 2));
		}
		{
			gotArgs = false;
			args.clear();
			auto result = cli.parse({ "test_app", "--args" });
			test(REQUIRE(result));
			test(REQUIRE(gotArgs));
			test(REQUIRE(args.empty()));
		}
		{
			gotArgs = false;
			args.clear();
			auto result = cli.parse({ "test_app", "a0", "a1" });
			test(REQUIRE(!result));
			test(REQUIRE(!gotArgs));
			test(REQUIRE(args.empty()));
		}
	}
	return test;
}
