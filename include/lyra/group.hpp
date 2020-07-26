// Copyright 2020 René Ferdinand Rivera Morell
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_GROUP_HPP
#define LYRA_GROUP_HPP

#include "lyra/arguments.hpp"
#include <functional>

namespace lyra
{
/* tag::reference[]

[#lyra_group]
= `lyra::group`

Is-a <<lyra_arguments>>.

end::reference[] */
class group : public arguments
{
	public:
	group() { }

	group(const group& other)
		: arguments(other)
		, success_signal(other.success_signal)
	{
	}

	group& on_success(const std::function<void(const group&)>& f);

	virtual bool is_group() const override { return true; }

	parse_result parse(
		detail::token_iterator const& tokens,
		parser_customization const& customize) const override
	{
		parse_result result = arguments::parse(tokens, customize);
		if (result && result.value().type() != parser_result_type::no_match
			&& success_signal)
		{
			// Trigger any success signal for parsing the argument as the group.
			// This allows for executing handlers for commands.
			this->success_signal(*this);
		}
		return result;
	}

	virtual std::unique_ptr<parser> clone() const override
	{
		return make_clone<group>(this);
	}

	private:
	std::function<void(const group&)> success_signal;
};

/* tag::reference[]

[#lyra_group_specification]
== Specification

end::reference[] */

/* tag::reference[]

[#lyra_group_on_success]
=== `lyra::group::on_success`

[source]
----
group& group::on_success(const std::function<void(const group&)>& f)
----

Registers a function to call when the group is successfully parsed. The
function is called with the group to facilitate customization.

end::reference[] */
inline group& group::on_success(const std::function<void(const group&)>& f)
{
	success_signal = f;
	return *this;
}

} // namespace lyra

#endif
