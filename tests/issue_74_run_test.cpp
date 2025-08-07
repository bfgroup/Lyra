/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

int main()
{
	bfg::mini_test::scope test;

	{
		auto cli = lyra::cli().add_argument(lyra::command("sub"));
		{
			auto result = cli.parse({ "test_app", "sub" });
			auto err = result.message();
			test(REQUIRE(result));
			test(REQUIRE(err == ""));
		}
		{
			auto result = cli.parse({ "test_app", "two" });
			auto err = result.message();
			test(REQUIRE(!result));
			test(REQUIRE(
				err == "Unrecognized argument 'two' while parsing: sub"));
		}
	}
	return test;
}
