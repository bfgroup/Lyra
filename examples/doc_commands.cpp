/*
Copyright Rene Rivera 2019
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/* tag::doc[]

= Sub-commands

A common program structure is to have a "wrapper" or "shell" that performs
various sub-commands. Even though Lyra doesn't have a direct specification
for sub-commands, it is possible to support this use case. In this example
we double parse arguments. Once for the sub-command, and a second time for
the sub-command arguments. This is possible because of Lyra's composition
of argument specifications.

[source]
----
*/ // end::doc[]

// tag::doc[]
#include <iostream>
#include <lyra/lyra.hpp>
#include <string>
#include <vector>

// Run a process, sub-command data.
struct run_command // <1>
{
	static const char* name() { return "run"; } // <2>
	std::vector<std::string> command; // <3>
	bool verbose = false;

	run_command(lyra::cli_parser& cli) // <4>
	{
		cli.add_argument(
			lyra::opt(verbose)
				.name("-v")
				.name("--verbose")
				.optional()
				.help("Show additional output as to what we are doing."));
		cli.add_argument(
			lyra::arg(command, "command")
				.required()
				.help("The command, and arguments, to attempt to run."));
	}
};

// Kill a named process, sub-command data.
struct kill_command // <5>
{
	static const char* name() { return "kill"; }
	std::string process_name;
	int signal = 9;

	kill_command(lyra::cli_parser& cli)
	{
		cli.add_argument(
			lyra::opt(signal, "signal")
				.name("-s")
				.name("--signal")
				.optional()
				.help("The signal integer to post to the running process."));
		cli.add_argument(
			lyra::arg(process_name, "process_name")
				.required()
				.help("The name of the process to search and signal."));
	}
};

int main(int argc, const char** argv)
{
	auto cli = lyra::cli_parser();
	std::string command;
	bool show_help = false;
	cli.add_argument(lyra::help(show_help));
	cli.add_argument(lyra::arg(command, "command") // <6>
						 .choices(run_command::name(), kill_command::name())
						 .required()
						 .help("Command to perform."));
	auto result = cli.parse({ argc, argv }); // <7>
	if (show_help)
	{
		std::cout << cli;
		return 0;
	}
	if (command == run_command::name())
	{
		run_command run_cmd(cli); // <8>
		result = cli.parse({ argc, argv });
	}
	else if (command == kill_command::name())
	{
		kill_command kill_cmd(cli);
		result = cli.parse({ argc, argv });
	}
	if (!result) // <9>
	{
		std::cerr << result.errorMessage() << "\n";
	}
	return result ? 0 : 1;
}
// end::doc[]
/* tag::doc[]
----
<1> A simple `struct` for information on the sub-commands. First for our `run`
	sub-command.
<2> The sub-command name, for use in the choices for the main program.
<3> The arguments for the sub-command.
<4> The constructor defines the additional arguments for the sub-command in
	the given `cli_parser`
<5> And now the information for our `kill` sub-comand.
<6> The top-level command, i.e. program, just defines the set of possible
	sub-command choices.
<7> At first we only parse with the sub-commands argument (and help). This
	limits what users see to just the sub-commands when they ask for help.
<8> Once we have a valid sub-command we set, and hence add, the sub-command
	arguments. And re-parse with the full recognized sub-command arguments.
<9> At the end we can do the regular error handling.
*/ // end::doc[]
