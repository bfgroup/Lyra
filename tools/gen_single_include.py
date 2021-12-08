# Copyright 2019-2020 René Ferdinand Rivera Morell
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import argparse
import os.path
import re


class GenSingleInclude(object):

	def __init__(self):
		self.pp_re = re.compile(r'''#\s*include\s+["<]([^">]+)[">]\s*''')
		parser = argparse.ArgumentParser()
		parser.add_argument('--src-include-dir')
		parser.add_argument('--src-include')
		parser.add_argument('--dst-include')
		self.args = parser.parse_args()
		if not os.path.isabs(self.args.src_include_dir):
			self.args.src_include_dir = os.path.abspath(
				os.path.join(os.curdir, self.args.src_include_dir))
		# print("src-include-dir: %s"%(self.args.src_include_dir))
		# print("src-include: %s"%(self.args.src_include))
		# print("dst-include: %s"%(self.args.dst_include))
		self.parsed = set()
		self.run()

	def run(self):
		with open(self.args.dst_include, "w", encoding="UTF8") as dst_file:
			dst_file.write('''\
// Copyright 2018-2022 René Ferdinand Rivera Morell
// Copyright 2021 Max Ferger
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
''')
			self.cpp(dst_file, None, self.args.src_include)

	def resolve_include(self, cur_dir, include_name):
		if os.path.isabs(include_name):
			return include_name
		elif cur_dir and os.path.exists(os.path.join(cur_dir, include_name)):
			return os.path.abspath(os.path.join(cur_dir, include_name))
		elif os.path.exists(os.path.join(self.args.src_include_dir, include_name)):
			return os.path.abspath(os.path.join(self.args.src_include_dir, include_name))
		else:
			return None

	def cpp(self, dst_file, cur_include_dir, src_include):
		src_i = self.resolve_include(cur_include_dir, src_include)
		if src_i and src_i not in self.parsed:
			self.parsed.add(src_i)
			with open(src_i, "r", encoding="UTF8") as src_file:
				for line in src_file:
					if line.strip().startswith('//'):
						continue
					pp_match = self.pp_re.fullmatch(line)
					src_n = None
					if pp_match:
						src_n = self.cpp(dst_file, os.path.dirname(
							src_i), pp_match.group(1))
					if not src_n:
						dst_file.writelines([line])
		return src_i


if __name__ == "__main__":
	GenSingleInclude()
