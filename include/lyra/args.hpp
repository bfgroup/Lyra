// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_ARGS_HPP
#define LYRA_ARGS_HPP

#include <initializer_list>
#include <string>
#include <vector>

namespace lyra
{
/* tag::reference[]

[#lyra_args]
= `lyra::args`

Transport for raw args (copied from main args, or supplied via init list).

*/ // end::reference[]
class args
{
	public:
	// Construct from usual main() function arguments.
	args(int argc, char const* const* argv)
		: m_exeName(argv[0])
		, m_args(argv + 1, argv + argc)
	{
	}

	// Construct directly from an initializer '{}'.
	args(std::initializer_list<std::string> args)
		: m_exeName(*args.begin())
		, m_args(args.begin() + 1, args.end())
	{
	}

	// The executable name taken from argument zero.
	std::string exe_name() const { return m_exeName; }

	// Arguments, excluding the exe name.
	std::vector<std::string>::const_iterator begin() const
	{
		return m_args.begin();
	}

	std::vector<std::string>::const_iterator end() const
	{
		return m_args.end();
	}

	private:
	std::string m_exeName;
	std::vector<std::string> m_args;
};
} // namespace lyra

#endif
