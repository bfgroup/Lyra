/*
Copyright 2021-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

void test_style(bfg::mini_test::scope & test, const lyra::option_style & style)
{
	using namespace lyra;

	bool a = false, b = false, c = false;
	int x = 10;
	float y = 3.14f;
	bool sdf = false;

	auto cli = lyra::cli().style(style);
	auto opt_a = opt(a);
	auto opt_b = opt(b);
	auto opt_c = opt(c);
	auto opt_x = opt(x, "int-x");
	auto opt_y = opt(y, "float-y");
	auto opt_sdf = opt(sdf);
	if (style.short_option_size > 0)
	{
		const std::string p(
			style.short_option_size, style.short_option_prefix[0]);
		opt_a["-a"];
		opt_b["-b"];
		opt_c["-c"];
		opt_x["-x"];
		opt_y["-y"];
		opt_sdf["-sdf"];
	}
	if (style.long_option_size > 0)
	{
		const std::string p(
			style.long_option_size, style.long_option_prefix[0]);
		opt_a["--flag-a"];
		opt_b["--flag-b"];
		opt_c["--flag-c"];
		opt_x["--int-x"];
		opt_y["--float-y"];
	}
	if (style.short_option_size > 0 && style.long_option_size > 0)
	{
		cli |= opt_a | opt_b | opt_c | opt_x | opt_y | opt_sdf;
	}
	else if (style.short_option_size > 0)
	{
		cli |= opt_a | opt_b | opt_c | opt_x | opt_y | opt_sdf;
	}
	else if (style.long_option_size > 0)
	{
		cli |= opt_a | opt_b | opt_c | opt_x | opt_y;
	}

	const std::string d(1, style.value_delimiters[0]);
	if (style.short_option_size > 0)
	{
		const std::string p(
			style.short_option_size, style.short_option_prefix[0]);
		const std::string i = "'" + style.short_option_prefix + "' '"
			+ style.value_delimiters + "'";
		a = false, b = false, c = false;
		test(
			i,
			REQUIRE(cli.parse(
				{ "TestApp",
				  p + "a",
				  p + "b",
				  p + "c",
				  p + "x",
				  "3",
				  p + "y",
				  "6.6" },
				style)));
		test(i, REQUIRE(a))(i, REQUIRE(b))(i, REQUIRE(c));
		a = false, b = false, c = false;
		test(
			i,
			REQUIRE(cli.parse(
				{ "TestApp", p + "abc", p + "x", "3", p + "y", "6.6" },
				style)));
		test(i, REQUIRE(a))(i, REQUIRE(b))(i, REQUIRE(c));
		x = 0;
		test(i, REQUIRE(cli.parse({ "TestApp", p + "x", "3" })));
		test(i, REQUIRE(x == 3));
		x = 0;
		test(i, REQUIRE(cli.parse({ "TestApp", p + "x" + d + "3" })));
		test(i, REQUIRE(x == 3));
		x = 0;
		test(i, REQUIRE(cli.parse({ "TestApp", p + "x3" })));
		test(i, REQUIRE(x == 3));
		x = 0;
		test(i, REQUIRE(!cli.parse({ "TestApp", p + "x3garbage" })));
		test(i, REQUIRE(x == 0));
		x = 0;
		test(
			i,
			REQUIRE(
				!cli.parse({ "TestApp", p + "x" + d + "3garbage" })));
		test(i, REQUIRE(x == 0));
		x = 0;
		test(i, REQUIRE(!cli.parse({ "TestApp", p + "x", "3garbage" })));
		test(i, REQUIRE(x == 0));
		test(i, REQUIRE(cli.parse({ "TestApp", p + "s" })));
		test(i, REQUIRE(cli.parse({ "TestApp", p + "d" })));
		test(i, REQUIRE(cli.parse({ "TestApp", p + "f" })));
	}
	if (style.long_option_size > 0)
	{
		const std::string p(
			style.long_option_size, style.long_option_prefix[0]);
		const std::string i = "'" + style.long_option_prefix + "' '"
			+ style.value_delimiters + "'";
		x = 0;
		test(i, REQUIRE(cli.parse({ "TestApp", p + "int-x", "3" })));
		test(i, REQUIRE(x == 3));
		x = 0;
		test(
			i, REQUIRE(cli.parse({ "TestApp", p + "int-x" + d + "3" })));
		test(i, REQUIRE(x == 3));
		x = 0;
		test(i, REQUIRE(!cli.parse({ "TestApp", p + "int-x3" })));
		test(i, REQUIRE(x == 0));
	}
}

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;
	test_style(test, option_style::posix());
	test_style(test, option_style::posix_brief());
	test_style(test, option_style::windows());
	return test;
}
