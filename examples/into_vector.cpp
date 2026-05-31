/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <lyra/lyra.hpp>

int main(int argc, const char ** argv)
{
	using namespace lyra;

	bool show_help = false;
	std::vector<int> input_shape;

	auto parse_shape_value = [&](std::string shape) {
		std::istringstream s(shape);
		std::string i;
		while (std::getline(s, i, ','))
		{
			int x = 0;
			if (lyra::detail::from_string(i, x)) input_shape.push_back(x);
		}
	};

	lyra::cli cli;
	cli.add_argument(
		help(show_help).description("Parse many values into a vector."));
	cli.add_argument(
		opt(parse_shape_value, "input-shape").name("--input-shape"));

	auto result = cli.parse({ argc, argv });

	if (!result) std::cerr << result.message() << "\n\n";
	if (show_help or !result)
		std::cout << cli << "\n";
	else
	{
		std::cout << "input shape : ";
		for (auto x : input_shape) std::cout << x << " ";
		std::cout << "\n";
	}

	return 0;
}
