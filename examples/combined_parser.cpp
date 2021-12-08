/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <iostream>
#include <lyra/lyra.hpp>

int main(int argc, const char** argv)
{
	using namespace lyra;
	bool show_help = false;
	struct Config
	{
		int seed = 0;
		std::string name;
		std::vector<std::string> tests;
		bool flag = false;
		double value = 0;
		int choice = 5;
		std::string color = "red";
	} config;
	auto parser
		= help(show_help).description(
			"This is a combined sample CLI parser. It takes varied options"
			"and arguments.")
		| opt(config.seed, "time|value")
			  ["--rng-seed"]["-r"]("Set a specific seed for random numbers.")
				  .required()
		| opt(config.name, "name")
			  ["-n"]["--name"]("The name to use.")
		| opt(config.flag)
			  ["-f"]["--flag"]("A flag to set.")
		| opt([&](double value) { config.value = value; }, "number")
			  ["-d"]["--double"]("Just some number.")
		| arg(config.tests, "test name|tags|pattern")("Which test or tests to use.").required()
		| opt(config.choice, "1-10")
			  ["-c"]["--choice"]("A choice from 1 to 10.")
				  .choices([](int value) -> bool { return 1 <= value && value <= 10; });
	parser.add_argument(
		opt(config.color, "red|green|blue")
			.name("-k")
			.name("--color")
			.help("A primary color.")
			.choices("red", "green", "blue"));

	auto result = parser.parse({ argc, argv });

	if (!result)
	{
		std::cerr << result.message() << "\n\n";
	}
	if (show_help or !result)
	{
		std::cout << parser << "\n";
	}

	return 0;
}
