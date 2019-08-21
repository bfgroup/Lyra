project('Lyra','cpp',
        version: '0.0.1',
        default_options : ['cpp_std=c++17'],
        license: 'BSL-1.0')

cpp_args = ['-Wall', '-Wextra', '-Wpedantic', '-flto', '-Wimplicit-fallthrough']

# if meson.get_compiler('cpp').get_id() == 'clang'
#   cpp_args += []
# endif
# if meson.get_compiler('cpp').get_id() == 'gcc'
#   cpp_args += []
# endif

message('Build Type: ' + get_option('buildtype'))
if get_option('buildtype').startswith('debug')
  cpp_args += ['-g3']
endif
if get_option('buildtype').startswith('release')
  cpp_args += ['-O3', '-frename-registers']
endif

src = ['include/lyra/arg.hpp',
      'include/lyra/args.hpp',
      'include/lyra/cli_parser.hpp',
      'include/lyra/exe_name.hpp',
      'include/lyra/help.hpp',
      'include/lyra/lyra.hpp',
      'include/lyra/opt.hpp',
      'include/lyra/parser_result.hpp',
      'include/lyra/parser.hpp',
      
      'include/lyra/detail/bound.hpp',
      'include/lyra/detail/choices.hpp',
      'include/lyra/detail/from_string.hpp',
      'include/lyra/detail/invoke_lambda.hpp',
      'include/lyra/detail/parse.hpp',
      'include/lyra/detail/result.hpp',
      'include/lyra/detail/tokens.hpp',
      'include/lyra/detail/unary_lambda_traits.hpp'
]

inc_dir = [include_directories('include'), include_directories('include/lyra/detail')]

lyra_lib = shared_library('lyra',
        sources : src,
        include_directories: inc_dir,
        cpp_args : cpp_args,
        install: true,
        install_dir : '/usr/lib')

lyra_dep = declare_dependency(include_directories : inc_dir, link_with : lyra_lib)