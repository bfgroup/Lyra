/*
Copyright 2020-2021 Max Ferger
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>
#include <sstream>
#include <string>

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		int i = -1;
		bool show_help = false;
		auto opt_i_parser = opt(i, "dummy")["-i"];
		test(REQUIRE(opt_i_parser.hint() == "dummy"));

		opt_i_parser.hint("integer"); // DUT

		test(REQUIRE(opt_i_parser.hint() == "integer"));

		auto cli = lyra::cli() | opt_i_parser | help(show_help);

		{
			std::ostringstream help_text;
			auto result = cli.parse({ "TestApp", "-?" });
			help_text << cli;
			test(REQUIRE(result));
			test(REQUIRE(help_text.str().find("dummy") == std::string::npos));
			test(REQUIRE(help_text.str().find("integer") != std::string::npos));
		}

		{
			auto result = cli.parse({ "TestApp", "-i=42" });
			test(REQUIRE(result));

			test(REQUIRE(i == 42));

			int i_via_name = static_cast<int>(cli["-i"]);
			test(REQUIRE(i_via_name == 42));

			int i_via_hint = static_cast<int>(cli["integer"]);
			test(REQUIRE(i_via_hint == 42));

			int i_via_dummy = static_cast<int>(cli["dummy"]);
			test(REQUIRE(i_via_dummy == 0)); // also, not -1
		}
	}

	return test;
}
