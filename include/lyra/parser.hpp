// Copyright René Ferdinand Rivera Morell
// Copyright 2021 Max Ferger
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_PARSER_HPP
#define LYRA_PARSER_HPP

#include "lyra/detail/bound.hpp"
#include "lyra/detail/choices.hpp"
#include "lyra/detail/result.hpp"
#include "lyra/detail/tokens.hpp"
#include "lyra/detail/trait_utils.hpp"
#include "lyra/option_style.hpp"
#include "lyra/parser_result.hpp"
#include "lyra/printer.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <numeric>
#include <string>

namespace lyra {

namespace detail {

class parse_state
{
	public:
	parse_state(parser_result_type type,
		token_iterator const & remaining_tokens,
		std::size_t parsed_tokens = 0)
		: result_type(type)
		, tokens(remaining_tokens)
	{
		(void)parsed_tokens;
	}

	parser_result_type type() const { return result_type; }
	token_iterator remainingTokens() const { return tokens; }
	bool have_tokens() const { return bool(tokens); }

	private:
	parser_result_type result_type;
	token_iterator tokens;
};

struct parser_cardinality
{
	std::size_t minimum = 0;
	std::size_t maximum = 0;

	parser_cardinality() = default;

	parser_cardinality(std::size_t a, std::size_t b)
		: minimum(a)
		, maximum(b)
	{}

	// If zero or more are accepted, it's optional.
	bool is_optional() const { return (minimum == 0); }

	// Anything that doesn't have an upper bound is considered unbounded.
	bool is_unbounded() const { return (maximum == 0); }

	bool is_bounded() const { return !is_unbounded(); }

	// If one or more values are expected, it's required.
	bool is_required() const { return (minimum > 0); }

	void optional()
	{
		minimum = 0;
		maximum = 1;
	}
	void required(std::size_t n = 1)
	{
		minimum = n;
		maximum = n;
	}
	void counted(std::size_t n)
	{
		minimum = n;
		maximum = n;
	}
	void bounded(std::size_t n, std::size_t m)
	{
		minimum = n;
		maximum = m;
	}
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
	using base::error;
	using base::ok;

	parse_result(const base & other)
		: base(other)
	{}
};

/* tag::reference[]

[#lyra_parser]
= `lyra::parser`

Base for all argument parser types.

end::reference[] */
class parser
{
	public:
	virtual std::string get_usage_text(const option_style &) const
	{
		return "";
	}
	virtual std::string get_description_text(const option_style &) const
	{
		return "";
	}

	virtual ~parser() = default;

	virtual detail::parser_cardinality cardinality() const { return { 0, 1 }; }
	bool is_optional() const { return cardinality().is_optional(); }
	virtual bool is_group() const { return false; }
	virtual result validate() const { return result::ok(); }
	virtual std::unique_ptr<parser> clone() const { return nullptr; }
	virtual bool is_named(const std::string & n) const
	{
		(void)n;
		return false;
	}
	virtual const parser * get_named(const std::string & n) const
	{
		if (is_named(n)) return this;
		return nullptr;
	}
	virtual std::size_t get_value_count() const { return 0; }
	virtual std::string get_value(std::size_t i) const
	{
		(void)i;
		return "";
	}

	virtual parse_result parse(
		detail::token_iterator const & tokens, const option_style & style) const
		= 0;

	virtual std::string get_print_order_key(const option_style &) const
	{
		return "";
	}

	virtual void print_help_text_details(printer &, const option_style &) const
	{}

	protected:
	virtual void print_help_text(printer & p, const option_style & style) const
	{
		print_help_text_summary(p, style);
		p.heading(style, "OPTIONS, ARGUMENTS:");
		p.indent();
		print_help_text_details(p, style);
		p.dedent();
	}

	virtual void print_help_text_summary(
		printer & p, const option_style & style) const
	{
		std::string usage_text = get_usage_text(style);
		if (!usage_text.empty())
			p.heading(style, "USAGE:")
				.indent()
				.paragraph(style, usage_text)
				.dedent();

		std::string description_text = get_description_text(style);
		if (!description_text.empty()) p.paragraph(style, description_text);
	}

	template <typename I, typename F>
	void for_each_print_ordered_parser(
		const option_style & style, I b, I e, F f) const
	{
		if (style.options_print_order
			!= option_style::opt_print_order::per_declaration)
		{
			std::vector<std::size_t> order_index(std::distance(b, e));
			std::iota(order_index.begin(), order_index.end(), 0);
			std::stable_sort(order_index.begin(), order_index.end(),
				[&](std::size_t i, std::size_t j) {
					const parser & pa = **(b + i);
					const parser & pb = **(b + j);
					return style.opt_print_order_less(
						pa.get_print_order_key(style),
						pb.get_print_order_key(style));
				});
			for (auto i : order_index) f(style, **(b + i));
		}
		else
		{
			while (b != e) f(style, **(b++));
		}
	}
};

/* tag::reference[]

[#lyra_parser_specification]
== Specification

[#lyra_parser_get_usage_text]
=== `lyra::parser::get_usage_text`

[source]
----
virtual std::string get_usage_text(const option_style &) const;
----

Returns the formatted `USAGE` text for this parser, and any contained
sub-parsers. This is called to print out the help text from the stream operator.

[#lyra_parser_get_description_text]
=== `lyra::parser::get_description_text`

[source]
----
virtual std::string get_description_text(const option_style &) const;
----

Returns the description text for this, and any contained sub-parsers. This is
called to print out the help text from the stream operator.

end::reference[] */

template <typename T, typename U>
std::unique_ptr<parser> make_clone(const U * source)
{
	return std::unique_ptr<parser>(new T(*static_cast<const T *>(source)));
}

/* tag::reference[]

[#lyra_composable_parser]
= `lyra::composable_parser`

A parser that can be composed with other parsers using `operator|`. Composing
two `composable_parser` instances generates a `cli` parser.

end::reference[] */
template <typename Derived>
class composable_parser : public parser
{};

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

	explicit bound_parser(std::shared_ptr<detail::BoundRef> const & ref)
		: m_ref(ref)
	{
		if (m_ref->isContainer())
			m_cardinality = { 0, 0 };
		else
			m_cardinality = { 0, 1 };
	}
	bound_parser(
		std::shared_ptr<detail::BoundRef> const & ref, std::string const & hint)
		: m_ref(ref)
		, m_hint(hint)
	{
		if (m_ref->isContainer())
			m_cardinality = { 0, 0 };
		else
			m_cardinality = { 0, 1 };
		if (!m_ref->isFlag() && !m_ref->isContainer()
			&& m_ref->get_value_count() == 1)
		{
			auto value_zero = m_ref->get_value(0);
			if (!value_zero.empty())
				m_description = "[default: " + value_zero + "]";
		}
	}

	public:
	enum class ctor_lambda_e
	{
		val
	};

	template <typename Reference>
	bound_parser(Reference & ref, std::string const & hint);

	template <typename Lambda>
	bound_parser(Lambda const & ref,
		std::string const & hint,
		typename std::enable_if<detail::is_invocable<Lambda>::value,
			ctor_lambda_e>::type
		= ctor_lambda_e::val);

	template <typename T>
	explicit bound_parser(detail::BoundVal<T> && val)
		: bound_parser(val.move_to_shared())
	{}
	template <typename T>
	bound_parser(detail::BoundVal<T> && val, std::string const & hint)
		: bound_parser(val.move_to_shared(), hint)
	{}

	Derived & help(const std::string & text);
	Derived & operator()(std::string const & description);
	Derived & optional();
	Derived & required(std::size_t n = 1);
	Derived & cardinality(std::size_t n);
	Derived & cardinality(std::size_t n, std::size_t m);
	detail::parser_cardinality cardinality() const override
	{
		return m_cardinality;
	}
	std::string hint() const;
	Derived & hint(std::string const & hint);

	template <typename T,
		typename... Rest,
		typename std::enable_if<!detail::is_invocable<T>::value, int>::type = 0>
	Derived & choices(T val0, Rest... rest);
	template <typename Lambda,
		typename std::enable_if<detail::is_invocable<Lambda>::value, int>::type
		= 1>
	Derived & choices(Lambda const & check_choice);
	template <typename T, std::size_t N>
	Derived & choices(const T (&choice_values)[N]);

	std::unique_ptr<parser> clone() const override
	{
		return make_clone<Derived>(this);
	}

	bool is_named(const std::string & n) const override { return n == m_hint; }
	std::size_t get_value_count() const override
	{
		return m_ref->get_value_count();
	}
	std::string get_value(std::size_t i) const override
	{
		return m_ref->get_value(i);
	}
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
bound_parser<Derived>::bound_parser(Reference & ref, std::string const & hint)
	: bound_parser(
		  std::make_shared<detail::BoundValueRef<Reference>>(ref), hint)
{}

template <typename Derived>
template <typename Lambda>
bound_parser<Derived>::bound_parser(Lambda const & ref,
	std::string const & hint,
	typename std::enable_if<detail::is_invocable<Lambda>::value,
		ctor_lambda_e>::type)
	: bound_parser(std::make_shared<detail::BoundLambda<Lambda>>(ref), hint)
{}

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
Derived& bound_parser<Derived>::help(std::string const& help_description_text);
template <typename Derived>
Derived& bound_parser<Derived>::operator()(std::string const&
help_description_text);
----

Defines the help description of an argument.

end::reference[] */
template <typename Derived>
Derived & bound_parser<Derived>::help(const std::string & help_description_text)
{
	m_description = help_description_text
		+ (m_description.empty() ? "" : (" " + m_description));
	return static_cast<Derived &>(*this);
}
template <typename Derived>
Derived & bound_parser<Derived>::operator()(
	std::string const & help_description_text)
{
	return this->help(help_description_text);
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
Derived & bound_parser<Derived>::optional()
{
	return this->cardinality(0, 1);
}

/* tag::reference[]

[#lyra_bound_parser_required]
=== `lyra::bound_parser::required(n)`

[source]
----
template <typename Derived>
Derived& bound_parser<Derived>::required(std::size_t n);
----

Specifies that the argument needs to given the number of `n` times
(defaults to *1*).

end::reference[] */
template <typename Derived>
Derived & bound_parser<Derived>::required(std::size_t n)
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
Derived& bound_parser<Derived>::cardinality(std::size_t n);

template <typename Derived>
Derived& bound_parser<Derived>::cardinality(std::size_t n, std::size_t m);
----

Specifies the number of times the argument can and needs to appear in the list
of arguments. In the first form the argument can appear exactly `n` times. In
the second form it specifies that the argument can appear from `n` to `m` times
inclusive.

end::reference[] */
template <typename Derived>
Derived & bound_parser<Derived>::cardinality(std::size_t n)
{
	m_cardinality = { n, n };
	return static_cast<Derived &>(*this);
}

template <typename Derived>
Derived & bound_parser<Derived>::cardinality(std::size_t n, std::size_t m)
{
	m_cardinality = { n, m };
	return static_cast<Derived &>(*this);
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
template <typename T,
	typename... Rest,
	typename std::enable_if<!detail::is_invocable<T>::value, int>::type>
Derived & bound_parser<Derived>::choices(T val0, Rest... rest)
{
	value_choices = std::make_shared<detail::choices_set<T>>(val0, rest...);
	return static_cast<Derived &>(*this);
}

template <typename Derived>
template <typename Lambda,
	typename std::enable_if<detail::is_invocable<Lambda>::value, int>::type>
Derived & bound_parser<Derived>::choices(Lambda const & check_choice)
{
	value_choices
		= std::make_shared<detail::choices_check<Lambda>>(check_choice);
	return static_cast<Derived &>(*this);
}

template <typename Derived>
template <typename T, std::size_t N>
Derived & bound_parser<Derived>::choices(const T (&choice_values)[N])
{
	value_choices = std::make_shared<detail::choices_set<T>>(
		std::vector<T> { choice_values, choice_values + N });
	return static_cast<Derived &>(*this);
}

/* tag::reference[]

[#lyra_bound_parser_hint]
=== `lyra::bound_parser::hint`

[source]
----
template <typename Derived>
std::string lyra::bound_parser<Derived>::hint() const

template <typename Derived>
Derived & lyra::bound_parser<Derived>::hint(std::string const & hint)
----

Selectors to read and write the hint of a variable-bound parser.

The hint should not be modified anymore, once the parser is applied to arguments
or used in a `lyra::composable_parser`.

end::reference[] */
template <typename Derived>
std::string bound_parser<Derived>::hint() const
{
	return m_hint;
}

template <typename Derived>
Derived & bound_parser<Derived>::hint(std::string const & hint)
{
	m_hint = hint;
	return static_cast<Derived &>(*this);
}

} // namespace lyra

#endif
