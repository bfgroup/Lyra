/*
Copyright René Ferdinand Rivera Morell
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
		auto cli = lyra::cli();
		auto do_command
			= [&](lyra::group const &) { args.emplace_back("command"); };
		auto do_subcommand
			= [&](lyra::group const &) { args.emplace_back("subcommand"); };
		cli.add_argument(
			lyra::command("command", do_command)
				.cardinality(0, 1)
				.add_argument(lyra::command { "subcommand", do_subcommand }));

		{
			args.clear();
			auto result = cli.parse({ "test_app", "command" });
			test(REQUIRE(result));
			test(REQUIRE(args.size() == 1));
		}
		{
			args.clear();
			auto result = cli.parse({ "test_app", "command", "subcommand" });
			test(REQUIRE(result));
			test(REQUIRE(args.size() == 2));
		}
	}
	return test;
}
