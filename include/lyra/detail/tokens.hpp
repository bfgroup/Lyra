// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_TOKENS_HPP
#define LYRA_DETAIL_TOKENS_HPP

#include <string>
#include <vector>

namespace lyra
{
namespace detail
{
	// Wraps a token coming from a token stream. These may not directly
	// correspond to strings as a single string may encode an option + its
	// argument if the : or = form is used
	enum class token_type
	{
		unknown,
		option,
		argument
	};

	template <typename Char, class Traits = std::char_traits<Char>>
	class basic_token_name
	{
		public:
		using traits_type = Traits;
		using value_type = Char;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using const_iterator = const_pointer;
		using iterator = const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;
		using string_type = std::basic_string<value_type, traits_type>;

		basic_token_name() noexcept
			: str { nullptr }
			, len { 0 }
		{
		}

		basic_token_name(const basic_token_name&) noexcept = default;

		basic_token_name(const_pointer s) noexcept
			: str { s }
			, len { traits_type::length(s) }
		{
		}

		basic_token_name(const_pointer s, size_type count) noexcept
			: str { s }
			, len { count }
		{
		}

		basic_token_name& operator=(const basic_token_name&) noexcept = default;

		void swap(basic_token_name& other) noexcept
		{
			auto tmp = *this;
			*this = other;
			other = tmp;
		}

		const_iterator begin() const noexcept { return this->str; }
		const_iterator end() const noexcept { return this->str + this->len; }
		const_iterator cbegin() const noexcept { return this->str; }
		const_iterator cend() const noexcept { return this->str + this->len; }

		size_type size() const noexcept { return this->len; }
		size_type length() const noexcept { return this->len; }
		bool empty() const noexcept { return this->len == 0; }

		friend string_type to_string(const basic_token_name& t)
		{
			return { t.str, t.len };
		}

		friend string_type
		operator+(const_pointer lhs, const basic_token_name& rhs)
		{
			return lhs + to_string(rhs);
		}

		private:
		const_pointer str;
		size_type len;
	};

	// using token_name = basic_token_name<std::string::value_type>;
	using token_name = std::string;

	struct token
	{
		token_type type;
		token_name name;

		token()
			: type(token_type::unknown)
		{
		}
		token(const token& other) = default;
		token(token_type t, const token_name& n)
			: type(t)
			, name(n)
		{
		}
	};

	// Abstracts iterators into args with option arguments uniformly handled
	class token_iterator
	{
		public:
		template <typename Span>
		explicit token_iterator(
			Span const& args, std::string const& dels,
			std::string const& opt_prefix)
			: token_iterator(args.begin(), args.end(), dels, opt_prefix)
		{
		}

		token_iterator(
			std::vector<std::string>::const_iterator it,
			std::vector<std::string>::const_iterator itEnd,
			std::string const& dels, std::string const& opt_prefix)
			: it(it)
			, itEnd(itEnd)
			, delimiters(dels)
			, optionPrefix(opt_prefix)
		{
			loadBuffer();
		}

		explicit operator bool() const
		{
			return !m_tokenBuffer.empty() || it != itEnd;
		}

		auto count() const -> size_t
		{
			return m_tokenBuffer.size() + (itEnd - it);
		}

		// UB if boot(*this) == false.
		auto operator*() const -> token { return *(operator->()); }

		auto operator-> () const -> token const*
		{
			return !m_tokenBuffer.empty() ? &m_tokenBuffer.front() : nullptr;
		}

		auto operator++() -> token_iterator&
		{
			if (m_tokenBuffer.size() >= 2)
			{
				m_tokenBuffer.erase(m_tokenBuffer.begin());
			}
			else
			{
				if (it != itEnd) ++it;
				loadBuffer();
			}
			return *this;
		}

		private:
		std::vector<std::string>::const_iterator it;
		std::vector<std::string>::const_iterator itEnd;
		std::vector<token> m_tokenBuffer;
		std::string delimiters;
		std::string optionPrefix;

		void loadBuffer()
		{
			m_tokenBuffer.resize(0);

			// Skip any empty strings
			while (it != itEnd && it->empty()) ++it;

			if (it != itEnd)
			{
				auto const& next = *it;
				if (isOptPrefix(next[0]))
				{
					auto delimiterPos = next.find_first_of(delimiters);
					if (delimiterPos != std::string::npos)
					{
						m_tokenBuffer.emplace_back(
							token_type::option, next.substr(0, delimiterPos));
						m_tokenBuffer.emplace_back(
							token_type::argument,
							next.substr(delimiterPos + 1));
					}
					else
					{
						if (!isOptPrefix(next[1]) && next.size() > 2)
						{
							std::string opt;
							opt += optionPrefix[0];
							opt += " ";
							for (size_t i = 1; i < next.size(); ++i)
							{
								opt[1] = next[i];
								m_tokenBuffer.emplace_back(
									token_type::option, opt);
							}
						}
						else
						{
							m_tokenBuffer.emplace_back(
								token_type::option, next);
						}
					}
				}
				else
				{
					m_tokenBuffer.emplace_back(token_type::argument, next);
				}
			}
		}

		inline auto isOptPrefix(char c) -> bool
		{
			auto r = optionPrefix.find(c) != std::string::npos;
			return r;
		}
	};
} // namespace detail
} // namespace lyra

#endif
