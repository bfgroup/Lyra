// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_RESULT_HPP
#define LYRA_DETAIL_RESULT_HPP

#include <string>

namespace lyra
{
namespace detail
{
	class result_base
	{
		public:
		enum Type
		{
			Ok,
			LogicError,
			RuntimeError
		};

		result_base const& base() const { return *this; }

		explicit operator bool() const
		{
			return this->m_type == result_base::Ok;
		}

		Type type() const { return m_type; }

		std::string errorMessage() const { return m_errorMessage; }

		protected:
		Type m_type;
		std::string m_errorMessage; // Only populated if resultType is an error

		explicit result_base(Type type, const std::string& message = "")
			: m_type(type)
			, m_errorMessage(message)
		{
		}

		virtual ~result_base() = default;
	};

	template <typename T>
	class result_value_base : public result_base
	{
		public:
		T const& value() const { return m_value; }

		protected:
		union
		{
			T m_value;
		};

		explicit result_value_base(Type type, const std::string& message = "")
			: result_base(type, message)
		{
		}

		explicit result_value_base(Type type, T const& value)
			: result_base(type)
		{
			new (&m_value) T(value);
		}

		explicit result_value_base(result_value_base const& other)
			: result_base(other)
		{
			if (this->m_type == result_base::Ok)
				new (&m_value) T(other.m_value);
		}

		result_value_base& operator=(result_value_base const& other)
		{
			if (this->m_type == result_base::Ok) m_value.~T();
			result_base::operator=(other);
			if (this->m_type == result_base::Ok)
				new (&m_value) T(other.m_value);
			return *this;
		}

		~result_value_base() override
		{
			if (this->m_type == Ok) m_value.~T();
		}
	};

	template <>
	class result_value_base<void> : public result_base
	{
		protected:
		using result_base::result_base;
	};

	template <typename T = void>
	class basic_result : public result_value_base<T>
	{
		public:
		explicit basic_result(result_base const& other)
			: result_value_base<T>(other.type(), other.errorMessage())
		{
		}

		template <typename U>
		static basic_result ok(U const& value)
		{
			return basic_result(result_base::Ok, value);
		}

		static basic_result ok() { return basic_result(result_base::Ok); }

		static basic_result logicError(std::string const& message)
		{
			return basic_result(result_base::LogicError, message);
		}

		static basic_result runtimeError(std::string const& message)
		{
			return basic_result(result_base::RuntimeError, message);
		}

		protected:
		using result_value_base<T>::result_value_base;
	};
} // namespace detail
} // namespace lyra

#endif
