// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_PARSER_HPP
#define LYRA_PARSER_HPP

#include "lyra/args.hpp"
#include "lyra/detail/bound.hpp"
#include "lyra/detail/choices.hpp"
#include "lyra/detail/from_string.hpp"
#include "lyra/detail/result.hpp"
#include "lyra/detail/tokens.hpp"
#include "lyra/detail/trait_utils.hpp"
#include "lyra/parser_result.hpp"

#include <memory>
#include <string>
#include <type_traits>

namespace lyra
{

/* tag::reference[]

[#lyra_parser_customization]
= `lyra::parser_customization`

Customization interface for parsing of options.

[source]
----
virtual std::string token_delimiters() const = 0;
----

Specifies the characters to use for splitting a cli argument into the option
and its value (if any).

[source]
----
virtual std::string option_prefix() const = 0;
----

Specifies the characters to use as possible prefix, either single or double,
for all options.

end::reference[] */
struct parser_customization
{
	virtual std::string token_delimiters() const = 0;
	virtual std::string option_prefix() const = 0;
};

/* tag::reference[]

[#lyra_default_parser_customization]
= `lyra::default_parser_customization`

Is-a `lyra::parser_customization` that defines token delimiters as space (" ")
or equal ("="). And specifies the option prefix character as dash ("-")
resulting in long options with `--` and short options with `-`.

This customization is used as the default if none is given.

end::reference[] */
struct default_parser_customization : parser_customization
{
	std::string token_delimiters() const override { return " ="; }
	std::string option_prefix() const override { return "-"; }
};

namespace detail
{
	class parse_state
	{
		public:
		parse_state(
			parser_result_type type, token_iterator const& remainingTokens)
			: m_type(type)
			, m_remainingTokens(remainingTokens)
		{
		}

		auto type() const -> parser_result_type { return m_type; }
		auto remainingTokens() const -> token_iterator
		{
			return m_remainingTokens;
		}

		private:
		parser_result_type m_type;
		token_iterator m_remainingTokens;
	};

	struct parser_cardinality
	{
		size_t minimum = 0;
		size_t maximum = 0;

		parser_cardinality() = default;

		parser_cardinality(size_t a, size_t b)
			: minimum(a)
			, maximum(b)
		{
		}

		// If zero or more are accepted, it's optional.
		bool is_optional() const { return (minimum == 0); }

		// Anything that doesn't have an upper bound is considered unbounded.
		bool is_unbounded() const { return (maximum == 0); }

		bool is_bounded() const { return !is_unbounded(); }

		// If one or more values are expected, it's required.
		bool is_required() const { return (minimum > 0); }
	};
} // namespace detail

/* tag::reference[]

[#lyra_parser_result]
= `lyra::parser_result`

The result of parsing arguments.

end::reference[] */
class parse_result : public detail::basic_result<detail::parse_state>
{
	public:
	using base = detail::basic_result<detail::parse_state>;
	using base::basic_result;
	using base::logicError;
	using base::ok;
	using base::runtimeError;

	parse_result(const base& other)
		: base(other)
	{
	}
};

/* tag::reference[]

[#lyra_parser_base]
= `lyra::parser_base`

Base for all argument parser types.

end::reference[] */
class parser_base
{
	public:
	struct help_text_item
	{
		std::string option;
		std::string description;
	};

	using help_text = std::vector<help_text_item>;

	virtual ~parser_base() = default;

	virtual auto validate() const -> result { return result::ok(); }

	virtual parse_result parse(
		std::string const& exe_name, detail::token_iterator const& tokens,
		parser_customization const& customize) const = 0;

	virtual detail::parser_cardinality cardinality() const { return { 0, 1 }; }

	auto is_optional() const -> bool { return cardinality().is_optional(); }

	virtual std::string get_usage_text() const { return ""; }

	virtual help_text get_help_text() const { return {}; }

	virtual std::string get_description_text() const { return ""; }

	virtual std::unique_ptr<parser_base> clone() const { return nullptr; }
};

/* tag::reference[]

[#lyra_composable_parser]
= `lyra::composable_parser`

A parser that can be composed with other parsers using `operator|`.

end::reference[] */
template <typename DerivedT>
class composable_parser : public parser_base
{
};

// Common code and state for args and Opts
/* tag::reference[]

[#lyra_bound_parser]
= `lyra::bound_parser`

Parser that binds a variable reference or callback to the value of an argument.

end::reference[] */
template <typename Derived>
class bound_parser : public composable_parser<Derived>
{
	protected:
	std::shared_ptr<detail::BoundRef> m_ref;
	std::string m_hint;
	std::string m_description;
	detail::parser_cardinality m_cardinality;
	std::shared_ptr<detail::choices_base> value_choices;

	explicit bound_parser(std::shared_ptr<detail::BoundRef> const& ref)
		: m_ref(ref)
	{
		if (m_ref->isContainer())
			m_cardinality = { 0, 0 };
		else
			m_cardinality = { 0, 1 };
	}

	public:
	template <typename Reference>
	bound_parser(Reference& ref, std::string const& hint);

	template <typename Lambda>
	bound_parser(Lambda const& ref, std::string const& hint);

	Derived& help(const std::string& text);
	Derived& operator()(std::string const& description);
	Derived& optional();
	Derived& required(size_t n = 1);
	Derived& cardinality(size_t n);
	Derived& cardinality(size_t n, size_t m);
	detail::parser_cardinality cardinality() const override
	{
		return m_cardinality;
	}
	std::string hint() const { return m_hint; }

	template <
		typename T,
		typename... Rest,
		typename std::enable_if<
			!detail::is_invocable<T>::value,
			int>::type = 0>
	Derived& choices(T val0, Rest... rest);
	template <
		typename Lambda,
		typename std::enable_if<
			detail::is_invocable<Lambda>::value,
			int>::type = 1>
	Derived& choices(Lambda const& check_choice);
};

/* tag::reference[]

[#lyra_bound_parser_ctor]
== Construction

end::reference[] */

/* tag::reference[]
[source]
----
template <typename Derived>
template <typename Reference>
bound_parser<Derived>::bound_parser(Reference& ref, std::string const& hint);

template <typename Derived>
template <typename Lambda>
bound_parser<Derived>::bound_parser(Lambda const& ref, std::string const& hint);
----

Constructs a value option with a target typed variable or callback. These are
options that take a value as in `--opt=value`. In the first form the given
`ref` receives the value of the option after parsing. The second form the
callback is called during the parse with the given value. Both take a
`hint` that is used in the help text. When the option can be specified
multiple times the callback will be called consecutively for each option value
given. And if a container is given as a reference on the first form it will
contain all the specified values.

end::reference[] */
template <typename Derived>
template <typename Reference>
bound_parser<Derived>::bound_parser(Reference& ref, std::string const& hint)
	: bound_parser(std::make_shared<detail::BoundValueRef<Reference>>(ref))
{
	m_hint = hint;
}

template <typename Derived>
template <typename Lambda>
bound_parser<Derived>::bound_parser(Lambda const& ref, std::string const& hint)
	: bound_parser(std::make_shared<detail::BoundLambda<Lambda>>(ref))
{
	m_hint = hint;
}

/* tag::reference[]

[#lyra_bound_parser_specification]
== Specification

end::reference[] */

/* tag::reference[]

[#lyra_bound_parser_help]
=== `lyra::bound_parser::help`, `lyra::bound_parser::operator(help)`

[source]
----
template <typename Derived>
Derived& bound_parser<Derived>::help(std::string const& text);
template <typename Derived>
Derived& bound_parser<Derived>::operator()(std::string const& help);
----

Defines the help description of an argument.

end::reference[] */
template <typename Derived>
Derived& bound_parser<Derived>::help(const std::string& text)
{
	m_description = text;
	return static_cast<Derived&>(*this);
}
template <typename Derived>
Derived& bound_parser<Derived>::operator()(std::string const& help_text)
{
	return this->help(help_text);
}

/* tag::reference[]

[#lyra_bound_parser_optional]
=== `lyra::bound_parser::optional`

[source]
----
template <typename Derived>
Derived& bound_parser<Derived>::optional();
----

Indicates that the argument is optional. This is equivalent to specifying
`cardinality(0, 1)`.

end::reference[] */
template <typename Derived>
Derived& bound_parser<Derived>::optional()
{
	return this->cardinality(0, 1);
}

/* tag::reference[]

[#lyra_bound_parser_required]
=== `lyra::bound_parser::required(n)`

[source]
----
template <typename Derived>
Derived& bound_parser<Derived>::required(size_t n);
----

Specifies that the argument needs to given the number of `n` times
(defaults to *1*).

end::reference[] */
template <typename Derived>
Derived& bound_parser<Derived>::required(size_t n)
{
	if (m_ref->isContainer())
		return this->cardinality(1, 0);
	else
		return this->cardinality(n);
}

/* tag::reference[]

[#lyra_bound_parser_cardinality]
=== `lyra::bound_parser::cardinality(n, m)`

[source]
----
template <typename Derived>
Derived& bound_parser<Derived>::cardinality(size_t n);

template <typename Derived>
Derived& bound_parser<Derived>::cardinality(size_t n, size_t m);
----

Specifies the number of times the argument can and needs to appear in the list
of arguments. In the first form the argument can appear exactly `n` times. In
the second form it specifies that the argument can appear from `n` to `m` times
inclusive.

end::reference[] */
template <typename Derived>
Derived& bound_parser<Derived>::cardinality(size_t n)
{
	m_cardinality = { n, n };
	return static_cast<Derived&>(*this);
}

template <typename Derived>
Derived& bound_parser<Derived>::cardinality(size_t n, size_t m)
{
	m_cardinality = { n, m };
	return static_cast<Derived&>(*this);
}

/* tag::reference[]

[#lyra_bound_parser_choices]
=== `lyra::bound_parser::choices`

[source]
----
template <typename Derived>
template <typename T, typename... Rest>
lyra::opt& lyra::bound_parser<Derived>::choices(T val0, Rest... rest)

template <typename Derived>
template <typename Lambda>
lyra::opt& lyra::bound_parser<Derived>::choices(Lambda const &check_choice)
----

Limit the allowed values of an argument. In the first form the value is
limited to the ones listed in the call (two or more values). In the second
form the `check_choice` function is called with the parsed value and returns
`true` if it's an allowed value.

end::reference[] */
template <typename Derived>
template <
	typename T,
	typename... Rest,
	typename std::enable_if<
		!detail::is_invocable<T>::value,
		int>::type>
Derived& bound_parser<Derived>::choices(T val0, Rest... rest)
{
	value_choices
		= std::make_shared<detail::choices_set<T>>(val0, rest...);
	return static_cast<Derived&>(*this);
}

template <typename Derived>
template <
	typename Lambda,
	typename std::enable_if<
		detail::is_invocable<Lambda>::value,
		int>::type>
Derived& bound_parser<Derived>::choices(Lambda const& check_choice)
{
	value_choices
		= std::make_shared<detail::choices_check<Lambda>>(check_choice);
	return static_cast<Derived&>(*this);
}

} // namespace lyra

#endif
