#
# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Copyright 2019 Rémi Gastaldi.
# Copyright 2020-2021 René Ferdinand Rivera Morell.

# This Meson file provides support for using the Lyra library externally as
# a standalone project or by embedding in your Meson project tree. To use
# externally you would need a 'dependency' declaration and to use the
# project as a dependency where needed:
#
# ----
# lyra_dep = dependency('lyra');
#
# executable('yourproject',
#   ...
#   dependencies : lyra_dep)
# ----
#
# To use in your project tree you would only need to place the Lyra tree
# in a subdirectory named 'subprojects' and add a dependency fallback
#
# ----
# lyra_dep = dependency('lyra', fallback : ['Lyra', 'lyra_dep'])
#
# executable('yourproject',
#   ...
#   dependencies : lyra_dep)
# ----

project('Lyra','cpp',
	version: '1.6',
	default_options : ['cpp_std=c++11', 'cpp_eh=none', 'b_lto=true', 'warning_level=3'],
	license: 'BSL-1.0')

src = [
	'include/lyra/arg.hpp',
	'include/lyra/args.hpp',
	'include/lyra/arguments.hpp',
	'include/lyra/cli.hpp',
	'include/lyra/cli_parser.hpp',
	'include/lyra/command.hpp',
	'include/lyra/detail/bound.hpp',
	'include/lyra/detail/choices.hpp',
	'include/lyra/detail/deprecated_parser_customization.hpp',
	'include/lyra/detail/from_string.hpp',
	'include/lyra/detail/invoke_lambda.hpp',
	'include/lyra/detail/parse.hpp',
	'include/lyra/detail/print.hpp',
	'include/lyra/detail/result.hpp',
	'include/lyra/detail/tokens.hpp',
	'include/lyra/detail/trait_utils.hpp',
	'include/lyra/detail/unary_lambda_traits.hpp',
	'include/lyra/exe_name.hpp',
	'include/lyra/group.hpp',
	'include/lyra/help.hpp',
	'include/lyra/literal.hpp',
	'include/lyra/lyra.hpp',
	'include/lyra/main.hpp',
	'include/lyra/opt.hpp',
	'include/lyra/option_style.hpp',
	'include/lyra/parser.hpp',
	'include/lyra/parser_result.hpp',
	'include/lyra/printer.hpp',
	'include/lyra/val.hpp',
	'include/lyra/version.hpp'
]

inc_dir = [include_directories('include')]

lyra_lib = shared_library('lyra',
	sources : src,
	include_directories: inc_dir,
	install: true,
	install_dir : '/usr/lib')

lyra_dep = declare_dependency(include_directories: inc_dir, link_with: lyra_lib)
