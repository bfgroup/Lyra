/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

const char * custom_error = "Custom error message w/ context.";

static lyra::parser_result always_error_cb(std::string const &)
{
	return lyra::parser_result::error(
		lyra::parser_result_type::no_match, custom_error);
}

struct subcommand
{
	subcommand(lyra::cli & cli)
	{
		cli.add_argument(
			lyra::command("subcommand", [this](lyra::group const &) { exec(); })
				.add_argument(lyra::opt(
					[](std::string const & s) { return always_error_cb(s); },
					"arg")
						.name("--arg")));
	}

	private:
	void exec() { std::cout << "Subcommand\n"; }
};

int main()
{
	bfg::mini_test::scope test;

	bool show_help { false };
	auto cli = lyra::cli()
				   .add_argument(lyra::help(show_help))
				   .add_argument(lyra::opt(
					   [](std::string const & s) { return always_error_cb(s); },
					   "arg")
						   .name("--arg"));
	subcommand subcmd { cli };
	{
		auto result = cli.parse({ "test_app", "--arg", "value" });
		test(REQUIRE(!result));
		test(custom_error == result.message(),
			"\"" + std::string(custom_error) + "\" == \"" + result.message()
				+ "\"",
			CONTEXT);
	}
	{
		auto result = cli.parse({ "test_app", "subcommand", "--arg", "value" });
		test(REQUIRE(!result));
		test(custom_error == result.message(),
			"\"" + std::string(custom_error) + "\" == \"" + result.message()
				+ "\"",
			CONTEXT);
	}
	return test;
}
