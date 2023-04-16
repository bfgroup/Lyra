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
	{
		bool show_help = false;
		bool show_version = false;
		bool got_args = false;
		std::vector<std::string> args;

		auto cli = lyra::help(show_help)
			| (lyra::group() | lyra::opt(show_version)["--version"].required())
			| (lyra::group().sequential()
				| lyra::opt(got_args)["--args"].required()
				| lyra::arg(args, "args"));

		{
			show_help = false;
			show_version = false;
			args.clear();
			auto result = cli.parse({ "test_app" });
			test(REQUIRE(result));
			test(REQUIRE(!show_help));
			test(REQUIRE(!got_args));
			test(REQUIRE(!show_version));
			test(REQUIRE(args.empty()));
		}
		{
			show_help = false;
			show_version = false;
			args.clear();
			auto result = cli.parse({ "test_app", "--version" });
			test(REQUIRE(result));
			test(REQUIRE(!show_help));
			test(REQUIRE(!got_args));
			test(REQUIRE(show_version));
			test(REQUIRE(args.empty()));
		}
		{
			show_help = false;
			show_version = false;
			args.clear();
			auto result = cli.parse({ "test_app", "--args" });
			test(REQUIRE(result));
			test(REQUIRE(!show_help));
			test(REQUIRE(got_args));
			test(REQUIRE(!show_version));
			test(REQUIRE(args.empty()));
		}
		{
			show_help = false;
			show_version = false;
			args.clear();
			auto result
				= cli.parse({ "test_app", "--version", "--args", "a0" });
			test(REQUIRE(result));
			test(REQUIRE(!show_help));
			test(REQUIRE(got_args));
			test(REQUIRE(show_version));
			test(REQUIRE(!args.empty()));
		}
	}
	{
		bool show_version = false;
		bool got_args = false;
		std::vector<std::string> args;
		auto cli = lyra::cli()
			| (lyra::group() | lyra::opt(show_version)["--version"].required())
			| (lyra::group().sequential()
				| lyra::opt(got_args)["--args"].required()
				| lyra::arg(args, "args"));
		{
			show_version = false;
			args.clear();
			auto result = cli.parse({ "test_app" });
			test(REQUIRE(result));
			test(REQUIRE(!got_args));
			test(REQUIRE(!show_version));
			test(REQUIRE(args.empty()));
		}
		{
			show_version = false;
			args.clear();
			auto result = cli.parse({ "test_app", "--version" });
			test(REQUIRE(result));
			test(REQUIRE(!got_args));
			test(REQUIRE(show_version));
			test(REQUIRE(args.empty()));
		}
		{
			show_version = false;
			args.clear();
			auto result = cli.parse({ "test_app", "--args" });
			test(REQUIRE(result));
			test(REQUIRE(got_args));
			test(REQUIRE(!show_version));
			test(REQUIRE(args.empty()));
		}
		{
			show_version = false;
			args.clear();
			auto result
				= cli.parse({ "test_app", "--version", "--args", "a0" });
			test(REQUIRE(result));
			test(REQUIRE(got_args));
			test(REQUIRE(show_version));
			test(REQUIRE(!args.empty()));
		}
	}
	{
		bool show_help = false;
		auto name = std::string {};
		bool use_grade_id = false;
		int grade = 0;
		int id = 0;

		auto cli = lyra::help(show_help)
			| lyra::opt(name, "name")["-n"]["--name"].required()
			| (lyra::group([&](const lyra::group &) {
				  use_grade_id = true;
			  }) | lyra::opt(id, "id")["--id"].required()
				| lyra::opt(grade, "grade")["--grade"].required());
		{
			show_help = false;
			name = "";
			use_grade_id = false;
			grade = 0;
			id = 0;
			auto result = cli.parse(
				{ "test_app", "--name=name", "--id=13", "--grade=4" });
			test(REQUIRE(result));
			test(REQUIRE(name == "name"));
			test(REQUIRE(use_grade_id));
			test(REQUIRE(id == 13));
			test(REQUIRE(grade == 4));
		}
	}
	return test;
}
