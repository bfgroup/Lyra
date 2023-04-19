// Copyright 2023 René Ferdinand Rivera Morell
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_PRINTER_HPP
#define LYRA_PRINTER_HPP

#include <memory>
#include <ostream>
#include <string>

namespace lyra {

/* tag::reference[]

[#lyra_printer]
= `lyra::printer`

A `printer` is an interface to manage formatting for output. Mostly the output
is for the help text of lyra cli parsers. The interface abstracts away specifics
of the output device and any visual arrangement, i.e. padding, coloring, etc.

[source]
----
virtual printer & printer::heading(
	const std::string & txt) = 0;
virtual printer & printer::paragraph(
	const std::string & txt,
	std::size_t indent = 0) = 0;
virtual printer & printer::option(
	const std::string & opt,
	const std::string & description,
	std::size_t indent = 0) = 0;
----

You can customize the printing output by implementing a subclass of
`lyra::printer` and implementing a corresponding `make_printer` factory
function which matches the output to the printer. For example:

[source]
----
inline std::unique_ptr<my_printer> make_printer(my_output & os_)
{
	return std::unique_ptr<my_printer>(new my_output(os_));
}
----

*/ // end::reference[]
class printer
{
	public:
	virtual ~printer() = default;
	virtual printer & heading(const std::string & txt) = 0;
	virtual printer & paragraph(const std::string & txt, std::size_t indent = 0)
		= 0;
	virtual printer & option(const std::string & opt,
		const std::string & description,
		std::size_t indent = 0)
		= 0;
};

/* tag::reference[]

[#lyra_ostream_printer]
= `lyra::ostream_printer`

A <<lyra_printer>> that uses `std::ostream` for output. This is the one used in
the case of printing to the standard output channels of `std::cout` and
`std::cerr`.

*/ // end::reference[]
class ostream_printer : public printer
{
	public:
	explicit ostream_printer(std::ostream & os_)
		: os(os_)
	{}
	virtual printer & heading(const std::string & txt) override
	{
		os << txt << "\n";
		return *this;
	}
	virtual printer & paragraph(
		const std::string & txt, std::size_t indent = 0) override
	{
		const std::string indent_str(indent, ' ');
		os << indent_str << txt << "\n\n";
		return *this;
	}
	virtual printer & option(const std::string & opt,
		const std::string & description,
		std::size_t indent = 0) override
	{
		const std::string indent_str(indent, ' ');
		const std::string opt_pad(26 - indent - 1, ' ');
		if (opt.size() > opt_pad.size())
			os << indent_str << opt << "\n"
			   << indent_str << opt_pad << " " << description << "\n";
		else
			os << indent_str << opt
			   << opt_pad.substr(0, opt_pad.size() - opt.size()) << " "
			   << description << "\n";
		return *this;
	}

	protected:
	std::ostream & os;
};

inline std::unique_ptr<printer> make_printer(std::ostream & os_)
{
	return std::unique_ptr<printer>(new ostream_printer(os_));
}

} // namespace lyra

#endif
