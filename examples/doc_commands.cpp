/*
Copyright 2019 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/* tag::doc[]

= Sub-commands

A common program structure is to have a "wrapper" or "shell" that performs
various sub-commands. Even though Lyra doesn't have a direct specification
for sub-commands, it is possible to support this use case. In this example
we use group parameters to specify the sub-commands and a lambda to execute
the subcommands.

[source]
----
*/ // end::doc[]

// tag::doc[]
#include <lyra/lyra.hpp>

#include <iostream>
#include <string>
#include <vector>

// Run a process, sub-command data.
struct run_command // <1>
{
	std::vector<std::string> command; // <2>
	bool verbose = false;
	bool show_help = false;

	run_command(lyra::cli & cli) // <3>
	{
		cli.add_argument(
			lyra::group(
				[this](const lyra::group & g) { this->do_command(g); }) // <4>
				.add_argument( // <5>
					lyra::literal("run").help("Execute the given command."))
				.add_argument(lyra::help(show_help))
				.add_argument(
					lyra::opt(verbose)
						.name("-v")
						.name("--verbose")
						.optional()
						.help(
							"Show additional output as to what we are doing."))
				.add_argument(
					lyra::arg(command, "command")
						.required()
						.help(
							"The command, and arguments, to attempt to run.")));
	}

	void do_command(const lyra::group & g)
	{
		if (show_help) std::cout << g; // <6>
		else
		{
			std::cout << "RUN: "
					  << "verbose=" << (verbose ? "true" : "false");
			for (auto c : command) std::cout << " " << c;
			std::cout << "\n";
		}
	}
};

// Kill a named process, sub-command data.
struct kill_command // <7>
{
	std::string process_name;
	int signal = 9;
	bool show_help = false;

	kill_command(lyra::cli & cli)
	{
		cli.add_argument(
			lyra::group([this](const lyra::group & g) { this->do_command(g); })
				.add_argument(lyra::literal("kill").help(
					"Terminate the process with the given name."))
				.add_argument(lyra::help(show_help))
				.add_argument(
					lyra::opt(signal, "signal")
						.name("-s")
						.name("--signal")
						.optional()
						.help(
							"The signal integer to post to the running process."))
				.add_argument(
					lyra::arg(process_name, "process_name")
						.required()
						.help(
							"The name of the process to search and signal.")));
	}

	void do_command(const lyra::group & g)
	{
		if (show_help) std::cout << g;
		else
			std::cout << "KILL:"
					  << " signal=" << signal << " process=" << process_name
					  << "\n";
	}
};

int main(int argc, const char ** argv)
{
	auto cli = lyra::cli();
	std::string command;
	bool show_help = false;
	cli.add_argument(lyra::help(show_help));
	kill_command kill { cli };
	run_command run { cli };
	auto result = cli.parse({ argc, argv }); // <8>
	if (show_help)
	{
		std::cout << cli;
		return 0;
	}
	if (!result) // <9>
	{ std::cerr << result.errorMessage() << "\n"; }
	return result ? 0 : 1;
}
// end::doc[]
/* tag::doc[]
----
<1> A simple `struct` for information on the sub-commands. First for our `run`
	sub-command.
<2> The arguments for the sub-command.
<3> The constructor defines the additional arguments for the sub-command in
	the given `cli`
<4> Each sub-command is added as a parameter sub-group. Which indicates that
	the all the arguments within it get parsed together instead of as part of
	the `cli` itself. We set a callback for when the group is successfully
	parsed which tells us we have a valid command to respond to.
<5> For the name of the command we use a `literal`. Which will only match is
	that specific argument value is present.
<6> We specified a sub-command specific help option. Here we can check for it
	and print out the help for the `group` only. This help will look similar
	to the full help output, but only contains the `group` arguments.
<7> And now the information for our `kill` sub-comand.
<8> We go ahead and parse the top-level `cli` which will also parse the
	sub-command groups.
<9> At the end we can do the regular error handling.
*/ // end::doc[]
