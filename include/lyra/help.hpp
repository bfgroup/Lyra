// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_HELP_HPP
#define LYRA_HELP_HPP

#include "lyra/opt.hpp"

namespace lyra
{
/* tag::reference[]

[#lyra_help]
= `lyra::help`

Utility function that defines a default `--help` option. You can specify a
`bool` flag to indicate if the help option was specified and that you could
display a help message.

The option accepts `-?`, `-h`, and `--help` as allowed option names.

*/ // end::reference[]
class help : public opt
{
	public:
	help(bool& showHelpFlag)
		: opt([&](bool flag) {
			showHelpFlag = flag;
			return parser_result::ok(parser_result_type::short_circuit_all);
		})
	{
		static_cast<opt&> (*this)(
			"Display usage information.")["-?"]["-h"]["--help"]
			.optional();
	}

	help & description(const std::string &text);

	virtual std::string get_description_text() const override
	{
		return description_text;
	}

	virtual std::unique_ptr<parser_base> clone() const override
	{
		return std::unique_ptr<parser_base>(new help(*this));
	}

	private:
	std::string description_text;
};

/* tag::reference[]
[source]
----
help & help::description(const std::string &text)
----

Sets the given `text` as the general description to show with the help and
usage output for CLI parser. This text is displayed between the "Usage"
and "Options, arguments" sections.

end::reference[] */
inline help & help::description(const std::string &text)
{
	description_text = text;
	return *this;
}
} // namespace lyra


#endif
