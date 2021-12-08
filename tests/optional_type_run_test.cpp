/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include "mini_test.hpp"

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	#if defined(LYRA_CONFIG_OPTIONAL_TYPE)
	LYRA_CONFIG_OPTIONAL_TYPE<std::string> name;
	auto p = cli()
		| opt(name, "name")
			["-n"]["--name"]
			("the name to use");
	{
		auto result = p.parse({ "TestApp" });
		test
			(REQUIRE( result ))
			(REQUIRE( !name.has_value() ));
	}
	{
		auto result = p.parse({ "TestApp", "-n", "Pixie" });
		test
			(REQUIRE( result ))
			(REQUIRE( name.has_value() ))
			(REQUIRE( name.value() == "Pixie" ));
	}
	#endif

	return test;
}
