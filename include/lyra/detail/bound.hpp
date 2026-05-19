// Copyright René Ferdinand Rivera Morell
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_BOUND_HPP
#define LYRA_DETAIL_BOUND_HPP

#include "lyra/detail/from_string.hpp"
#include "lyra/detail/invoke_lambda.hpp"
#include "lyra/detail/parse.hpp"
#include "lyra/detail/unary_lambda_traits.hpp"
#include "lyra/parser_result.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace lyra { namespace detail {

struct NonCopyable
{
	NonCopyable() = default;
	NonCopyable(NonCopyable const &) = delete;
	NonCopyable(NonCopyable &&) = delete;
	NonCopyable & operator=(NonCopyable const &) = delete;
	NonCopyable & operator=(NonCopyable &&) = delete;
};

struct BoundRef : NonCopyable
{
	virtual ~BoundRef() = default;
	virtual bool isContainer() const { return false; }
	virtual bool isFlag() const { return false; }

	virtual size_t get_value_count() const { return 0; }
	virtual std::string get_value(size_t) const { return ""; }
};

struct BoundValueRefBase : BoundRef
{
	virtual parser_result setValue(std::string const & arg) = 0;
};

struct BoundFlagRefBase : BoundRef
{
	virtual parser_result setFlag(bool flag) = 0;
	virtual bool isFlag() const { return true; }
};

template <typename T>
struct BoundValueRef : BoundValueRefBase
{
	T & m_ref;

	explicit BoundValueRef(T & ref)
		: m_ref(ref)
	{ }

	parser_result setValue(std::string const & arg) override
	{
		return parse_string(arg, m_ref);
	}

	size_t get_value_count() const override { return 1; }
	std::string get_value(size_t i) const override
	{
		if (i == 0)
		{
			std::string text;
			detail::to_string(m_ref, text);
			return text;
		}
		return "";
	}
};

template <typename T>
struct BoundValueRef<std::vector<T>> : BoundValueRefBase
{
	std::vector<T> & m_ref;

	explicit BoundValueRef(std::vector<T> & ref)
		: m_ref(ref)
	{ }

	bool isContainer() const override { return true; }

	parser_result setValue(std::string const & arg) override
	{
		T temp;
		auto str_result = parse_string(arg, temp);
		if (str_result) m_ref.push_back(temp);
		return str_result;
	}

	size_t get_value_count() const override { return m_ref.size(); }
	std::string get_value(size_t i) const override
	{
		if (i < m_ref.size())
		{
			std::string str_result;
			detail::to_string(m_ref[i], str_result);
			return str_result;
		}
		return "";
	}
};

struct BoundFlagRef : BoundFlagRefBase
{
	bool & m_ref;

	explicit BoundFlagRef(bool & ref)
		: m_ref(ref)
	{ }

	parser_result setFlag(bool flag) override
	{
		m_ref = flag;
		return parser_result::ok(parser_result_type::matched);
	}

	size_t get_value_count() const override { return 1; }
	std::string get_value(size_t i) const override
	{
		if (i == 0) return m_ref ? "true" : "false";
		return "";
	}
};

template <typename L>
struct BoundLambda : BoundValueRefBase
{
	L m_lambda;

	static_assert(unary_lambda_traits<L>::isValid,
		"Supplied lambda must take exactly one argument");
	static_assert(
		std::is_same<L, typename detail::remove_cvref<L>::type>::value,
		"Supplied lambda must not be a reference");

	explicit BoundLambda(L const & lambda)
		: m_lambda(lambda)
	{ }
	explicit BoundLambda(L && lambda)
		: m_lambda(std::move(lambda))
	{ }

	parser_result setValue(std::string const & arg) override
	{
		return invokeLambda<typename unary_lambda_traits<L>::ArgType>(
			m_lambda, arg);
	}
};

template <typename L>
struct BoundFlagLambda : BoundFlagRefBase
{
	L m_lambda;

	static_assert(unary_lambda_traits<L>::isValid,
		"Supplied lambda must take exactly one argument");
	static_assert(
		std::is_same<L, typename detail::remove_cvref<L>::type>::value,
		"Supplied lambda must not be a reference");
	static_assert(
		std::is_same<typename unary_lambda_traits<L>::ArgType, bool>::value,
		"flags must be boolean");

	explicit BoundFlagLambda(L const & lambda)
		: m_lambda(lambda)
	{ }

	explicit BoundFlagLambda(L && lambda)
		: m_lambda(std::move(lambda))
	{ }

	parser_result setFlag(bool flag) override
	{
		return LambdaInvoker<
			typename unary_lambda_traits<L>::ReturnType>::invoke(m_lambda,
			flag);
	}
};

template <typename T>
struct BoundVal : BoundValueRef<T>
{
	T value;

	BoundVal(T && v)
		: BoundValueRef<T>(value)
		, value(v)
	{ }

	BoundVal(BoundVal && other) noexcept
		: BoundValueRef<T>(value)
		, value(std::move(other.value))
	{ }

	std::shared_ptr<BoundRef> move_to_shared()
	{
		return std::shared_ptr<BoundRef>(new BoundVal<T>(std::move(*this)));
	}
};

}} // namespace lyra::detail

#endif
