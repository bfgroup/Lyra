// Copyright René Ferdinand Rivera Morell

#ifndef LYRA_OPTION_STYLE_HPP
#define LYRA_OPTION_STYLE_HPP

#include <cstddef>
#include <string>
#include <utility>

namespace lyra {

/* tag::reference[]

[#lyra_option_style]
= `lyra::option_style`

Specify the syntax style for options to the parser.

[source]
----
std::string value_delimiters;
std::string long_option_prefix;
std::size_t long_option_size = 0;
std::string short_option_prefix;
std::size_t short_option_size = 0;
opt_print_order options_print_order = opt_print_order::per_declaration;
----

* `value_delimiters` -- Specifies a set of characters that are accepted as a
	delimiter/separator between an option name and an option value when a
	single argument is used for the option+value (i.e. "--option=value").
* `long_option_prefix` -- Specifies a set of characters that are accepted as a
	prefix for long options (i.e. multi-char single option name).
* `long_option_size` -- The number of prefix characters that indicates a long
	option. A value of zero (0) indicates that long options are not accepted.
* `short_option_prefix` -- Specifies a set of characters that are accepted as a
	prefix for short options (i.e. single-char multi-options).
* `short_option_size` -- The number of prefix characters that indicates a short
	option. A value of zero (0) indicates that short options are not accepted.
* `options_print_order` -- The order to print the options section of the help
	text. Possible values: `per_declaration`, `sorted_short_first`,
	`sorted_long_first`.
* `indent_size` -- The character count to indent argument detail description.
	This affects the indenting of usage detail, the arguments, and any
	sub-commands/arguments recursively. The default is "2".

end::reference[] */
struct option_style
{
	enum class opt_print_order : unsigned char
	{
		per_declaration = 0,
		sorted_short_first,
		sorted_long_first
	};

	std::string value_delimiters;
	std::string long_option_prefix;
	std::size_t long_option_size = 0;
	std::string short_option_prefix;
	std::size_t short_option_size = 0;
	opt_print_order options_print_order = opt_print_order::per_declaration;
	std::size_t indent_size = 2;

	// Construction..

	option_style(std::string && value_delimiters_chars,
		std::string && long_option_prefix_chars = {},
		std::size_t long_option_prefix_size = 0,
		std::string && short_option_prefix_chars = {},
		std::size_t short_option_prefix_size = 0,
		opt_print_order options_print_order_ = opt_print_order::per_declaration,
		std::size_t indent_size_ = 2)
		: value_delimiters(std::move(value_delimiters_chars))
		, long_option_prefix(std::move(long_option_prefix_chars))
		, long_option_size(long_option_prefix_size)
		, short_option_prefix(std::move(short_option_prefix_chars))
		, short_option_size(short_option_prefix_size)
		, options_print_order(options_print_order_)
		, indent_size(indent_size_)
	{}

	// Definitions..

	std::string long_option_string() const;
	std::string short_option_string() const;

	// Styles..

	static const option_style & posix();
	static const option_style & posix_brief();
	static const option_style & windows();

	// Comparisons for sorting print order.

	bool opt_print_order_less(
		const std::string & a, const std::string & b) const
	{
		const auto l = long_option_string();
		const auto s = short_option_string();
		const bool a_l = a.substr(0, l.size()) == l;
		const bool a_s = !a_l && (a.substr(0, s.size()) == s);
		const bool b_l = b.substr(0, l.size()) == l;
		const bool b_s = !b_l && (b.substr(0, s.size()) == s);
		if (!a_l && !a_s) return false;
		if (!b_l && !b_s) return true;
		if ((a_l == b_l) && (a_s == b_s)) return a < b;
		if (options_print_order == opt_print_order::sorted_short_first)
			return (a_s && b_l);
		else if (options_print_order == opt_print_order::sorted_long_first)
			return (b_s && a_l);
		return a < b;
	}
};

/* tag::reference[]

[#lyra_option_style_ctor]
== Construction

[source]
----
lyra::option_style::option_style(
	std::string && value_delimiters,
	std::string && long_option_prefix = {},
	std::size_t long_option_size = 0,
	std::string && short_option_prefix = {},
	std::size_t short_option_size = 0)
----

Utility constructor that defines all the settings.

end::reference[] */

/* tag::reference[]

[#lyra_option_style_def]
== Definitions

[source]
----
std::string lyra::option_style::long_option_string() const
std::string lyra::option_style::short_option_string() const
----

Gives the default long or short option string, or prefix, for this option
style. If the type of option is not available, i.e. size is zero, an empty
string is returned.

end::reference[] */

inline std::string option_style::long_option_string() const
{
	return long_option_size > 0
		? std::string(long_option_size, long_option_prefix[0])
		: "";
}

inline std::string option_style::short_option_string() const
{
	return short_option_size > 0
		? std::string(short_option_size, short_option_prefix[0])
		: "";
}

/* tag::reference[]

[#lyra_option_style_styles]
== Styles

[source]
----
static const option_style & lyra::option_style::posix();
static const option_style & lyra::option_style::posix_brief();
static const option_style & lyra::option_style::windows();
----

These provide definitions for common syntax of option styles:

`posix`:: The overall _default_ that is two dashes (`--`) for long option
	names and one dash (`-`) for short option names. Values for long options
	use equal (`=`) between the option and value.
`posix_brief`:: Variant that only allows for long option names with a single
	dash (`-`).
`windows`:: The common option style on Windows `CMD.EXE` shell. It only allows
	long name options that start with slash (`/`) where the value is
	specified after a colon (`:`). Single character flag style options are
	only available as individual long options, for example `/A`.

end::reference[] */

inline const option_style & option_style::posix()
{
	static const option_style style("= ", "-", 2, "-", 1);
	return style;
}

inline const option_style & option_style::posix_brief()
{
	static const option_style style("= ", "-", 1);
	return style;
}

inline const option_style & option_style::windows()
{
	static const option_style style(":", "/", 1);
	return style;
}

} // namespace lyra

#endif
