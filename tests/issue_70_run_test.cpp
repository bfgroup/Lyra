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

	std::vector<std::string> args;
	{
		auto cli = lyra::cli().add_argument(lyra::arg(args, "args"));
		{
			args.clear();
			auto result = cli.parse({ "test_app" });
			test(REQUIRE(result));
			test(REQUIRE(args.empty()));
		}
		{
			args.clear();
			auto result = cli.parse({ "test_app", "zero" });
			test(REQUIRE(result));
			test(REQUIRE(!args.empty()));
		}
		{
			args.clear();
			auto result = cli.parse({});
			test(REQUIRE(result));
			test(REQUIRE(args.empty()));
		}
	}
	return test;
}
