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

	{
		std::string first_arg;
		std::string second_arg;
		auto cli = lyra::cli().sequential()
			| lyra::arg(first_arg, "first_arg").required().choices("A", "B")
			| lyra::arg(second_arg, "second_arg").required();
		{
			first_arg = "";
			second_arg = "";
			auto result = cli.parse({ "test_app", "B", "second" });
			test(REQUIRE(result));
			test(REQUIRE(first_arg == "B"));
		}
		{
			first_arg = "";
			second_arg = "";
			auto result = cli.parse({ "test_app", "B" });
			test(REQUIRE(!result));
		}
		{
			first_arg = "";
			second_arg = "";
			auto result = cli.parse({ "test_app", "arg1", "B" });
			test(REQUIRE(!result));
		}
	}
	return test;
}
