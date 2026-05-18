# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Copyright René Ferdinand Rivera Morell.

import argparse
import json


class CXXPipelines(object):

    matrix_compilers = {
        "gcc-16": {
            "NAME": "GCC 16",
            "EXE": "g++-16",
            "TOOLSET": "gcc-16",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/gcc-ubuntu:16",
            "GROUP": "Linux",
        },
        "gcc-15": {
            "NAME": "GCC 15",
            "EXE": "g++-15",
            "TOOLSET": "gcc-15",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/gcc-ubuntu:15",
            "GROUP": "Linux",
        },
        "gcc-14": {
            "NAME": "GCC 14",
            "EXE": "g++-14",
            "TOOLSET": "gcc-14",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/gcc-ubuntu:14",
            "GROUP": "Linux",
        },
        "gcc-13": {
            "NAME": "GCC 13",
            "EXE": "g++-13",
            "TOOLSET": "gcc-13",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/gcc-ubuntu:13",
            "GROUP": "Linux",
        },
        "gcc-12": {
            "NAME": "GCC 12",
            "EXE": "g++-12",
            "TOOLSET": "gcc-12",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/gcc-ubuntu:12",
            "GROUP": "Linux",
        },
        "gcc-11": {
            "NAME": "GCC 11",
            "EXE": "g++-11",
            "TOOLSET": "gcc-11",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/gcc-ubuntu:11",
            "GROUP": "Linux",
        },
        "gcc-10": {
            "NAME": "GCC 10",
            "EXE": "g++-10",
            "TOOLSET": "gcc-10",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:10",
            "GROUP": "Linux",
        },
        "gcc-9": {
            "NAME": "GCC 9",
            "EXE": "g++-9",
            "TOOLSET": "gcc-9",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:9",
            "GROUP": "Linux",
        },
        "gcc-8": {
            "NAME": "GCC 8",
            "EXE": "g++-8",
            "TOOLSET": "gcc-8",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:8",
            "GROUP": "Linux",
        },
        "gcc-7": {
            "NAME": "GCC 7",
            "EXE": "g++-7",
            "TOOLSET": "gcc-7",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:7",
            "GROUP": "Linux",
        },
        "gcc-6": {
            "NAME": "GCC 6",
            "EXE": "g++-6",
            "TOOLSET": "gcc-6",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:6",
            "GROUP": "Linux",
        },
        "gcc-5": {
            "NAME": "GCC 5",
            "EXE": "g++-5",
            "TOOLSET": "gcc-5",
            "CXXSTD": "11,14",
            "CONTAINER": "teeks99/gcc-ubuntu:5",
            "GROUP": "Linux",
        },
        "gcc-4.9": {
            "NAME": "GCC 4.9",
            "EXE": "g++-4.9",
            "TOOLSET": "gcc-4.9",
            "CXXSTD": "11",
            "CONTAINER": "teeks99/gcc-ubuntu:4.9",
            "GROUP": "Linux",
        },
        "gcc-4.8": {
            "NAME": "GCC 4.8",
            "EXE": "g++-4.8",
            "TOOLSET": "gcc-4.8",
            "CXXSTD": "11",
            "CONTAINER": "teeks99/gcc-ubuntu:4.8",
            "GROUP": "Linux",
        },
        "clang-23": {
            "NAME": "Clang 23",
            "EXE": "clang++-23",
            "TOOLSET": "clang-23",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:23",
            "GROUP": "Linux",
        },
        "clang-22": {
            "NAME": "Clang 22",
            "EXE": "clang++-22",
            "TOOLSET": "clang-22",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:22",
            "GROUP": "Linux",
        },
        "clang-21": {
            "NAME": "Clang 21",
            "EXE": "clang++-21",
            "TOOLSET": "clang-21",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:21",
            "GROUP": "Linux",
        },
        "clang-20": {
            "NAME": "Clang 20",
            "EXE": "clang++-20",
            "TOOLSET": "clang-20",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:20",
            "GROUP": "Linux",
        },
        "clang-19": {
            "NAME": "Clang 19",
            "EXE": "clang++-19",
            "TOOLSET": "clang-19",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:19",
            "GROUP": "Linux",
        },
        "clang-18": {
            "NAME": "Clang 18",
            "EXE": "clang++-18",
            "TOOLSET": "clang-18",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:18",
            "GROUP": "Linux",
        },
        "clang-17": {
            "NAME": "Clang 17",
            "EXE": "clang++-17",
            "TOOLSET": "clang-17",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:17",
            "GROUP": "Linux",
        },
        "clang-16": {
            "NAME": "Clang 16",
            "EXE": "clang++-16",
            "TOOLSET": "clang-16",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:16",
            "GROUP": "Linux",
        },
        "clang-15": {
            "NAME": "Clang 15",
            "EXE": "clang++-15",
            "TOOLSET": "clang-15",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:15",
            "GROUP": "Linux",
        },
        "clang-14": {
            "NAME": "Clang 14",
            "EXE": "clang++-14",
            "TOOLSET": "clang-14",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:14",
            "GROUP": "Linux",
        },
        "clang-13": {
            "NAME": "Clang 13",
            "EXE": "clang++-13",
            "TOOLSET": "clang-13",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:13",
            "GROUP": "Linux",
        },
        "clang-12": {
            "NAME": "Clang 12",
            "EXE": "clang++-12",
            "TOOLSET": "clang-12",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:12",
            "GROUP": "Linux",
        },
        "clang-11": {
            "NAME": "Clang 11",
            "EXE": "clang++-11",
            "TOOLSET": "clang-11",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:11",
            "GROUP": "Linux",
        },
        "clang-10": {
            "NAME": "Clang 10",
            "EXE": "clang++-10",
            "TOOLSET": "clang-10",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:10",
            "GROUP": "Linux",
        },
        "clang-9": {
            "NAME": "Clang 9",
            "EXE": "clang++-9",
            "TOOLSET": "clang-9",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:9",
            "GROUP": "Linux",
        },
        "clang-8": {
            "NAME": "Clang 8",
            "EXE": "clang++-8",
            "TOOLSET": "clang-8",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:8",
            "GROUP": "Linux",
        },
        "clang-7": {
            "NAME": "Clang 7",
            "EXE": "clang++-7",
            "TOOLSET": "clang-7",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:7",
            "GROUP": "Linux",
        },
        "clang-6": {
            "NAME": "Clang 6",
            "EXE": "clang++-6.0",
            "TOOLSET": "clang-6.0",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:6",
            "GROUP": "Linux",
        },
        "clang-5": {
            "NAME": "Clang 5",
            "EXE": "clang++-5.0",
            "TOOLSET": "clang-5.0",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:5",
            "GROUP": "Linux",
        },
        "clang-4": {
            "NAME": "Clang 4",
            "EXE": "clang++-4.0",
            "TOOLSET": "clang-4.0",
            "CXXSTD": "11,14",
            "CONTAINER": "teeks99/clang-ubuntu:4",
            "GROUP": "Linux",
        },
        "clang-3.9": {
            "NAME": "Clang 3.9",
            "EXE": "clang++-3.9",
            "TOOLSET": "clang-3.9",
            "CXXSTD": "11",
            "CONTAINER": "teeks99/clang-ubuntu:3.9",
            "GROUP": "Linux",
        },
        "tcc-0.9.27": {
            "NAME": "TCC 0.9.27",
            "EXE": "tcc",
            "TOOLSET": "cc",
            "B2_CONFIG": '<cflags>"-std=c11" <-soname>-soname',
            "CONTAINER": "ubuntu:24.04",
            "GROUP": "Linux",
            "EXCLUDE": True,
        },
        "clang-darwin-26.0": {
            "NAME": "Xcode 26.0.1",
            "GROUP": "macOS",
            "EXE": "clang++",
            "TOOLSET": "clang",
            "XCODE_APP": "/Applications/Xcode_26.0.1.app",
            "VM_IMAGE": "macOS-15",
            "CXXSTD": "11,14,17,20,23",
        },
        "clang-darwin-16.4": {
            "NAME": "Xcode 16.4",
            "GROUP": "macOS",
            "EXE": "clang++",
            "TOOLSET": "clang",
            "XCODE_APP": "/Applications/Xcode_16.4.app",
            "VM_IMAGE": "macOS-15",
            "CXXSTD": "11,14,17,20",
        },
        "clang-darwin-15.4": {
            "NAME": "Xcode 15.4",
            "GROUP": "macOS",
            "EXE": "clang++",
            "TOOLSET": "clang",
            "XCODE_APP": "/Applications/Xcode_15.4.app",
            "VM_IMAGE": "macOS-14",
            "CXXSTD": "11,14,17,20",
        },
        "msvc-14.5": {
            "NAME": "VS 2025",
            "GROUP": "Windows",
            "TOOLSET": "msvc",
            "TOOLSET_VERSION": "14.5",
            "VM_IMAGE": "windows-2025-vs2026",
            "CXXSTD": "14,17,20,latest",
        },
        "msvc-14.3": {
            "NAME": "VS 2022",
            "GROUP": "Windows",
            "TOOLSET": "msvc",
            "TOOLSET_VERSION": "14.3",
            "VM_IMAGE": "windows-2022",
            "CXXSTD": "14,17,20",
        },
        "mingw-16": {
            "NAME": "MinGW 16",
            "GROUP": "Windows",
            "TOOLSET": "gcc",
            "TOOLSET_VERSION": "16.1.0",
            "VM_IMAGE": "windows-latest",
            "CXXSTD": "11,14,17,20,23",
            "MINGW_RT": "rt_v14-rev0",
        },
        "mingw-15": {
            "NAME": "MinGW 15",
            "GROUP": "Windows",
            "TOOLSET": "gcc",
            "TOOLSET_VERSION": "15.2.0",
            "VM_IMAGE": "windows-latest",
            "CXXSTD": "11,14,17,20,23",
            "MINGW_RT": "rt_v13-rev1",
        },
        "mingw-14": {
            "NAME": "MinGW 14",
            "GROUP": "Windows",
            "TOOLSET": "gcc",
            "TOOLSET_VERSION": "14.2.0",
            "VM_IMAGE": "windows-latest",
            "CXXSTD": "11,14,17,20,23",
            "MINGW_RT": "rt_v12-rev2",
        },
        "mingw-13": {
            "NAME": "MinGW 13",
            "TOOLSET": "gcc",
            "TOOLSET_VERSION": "13.2.0",
            "VM_IMAGE": "windows-latest",
            "CXXSTD": "11,14,17,20",
            "MINGW_RT": "rt_v11-rev1",
            "GROUP": "Windows",
        },
        "mingw-12": {
            "NAME": "MinGW 12",
            "GROUP": "Windows",
            "TOOLSET": "gcc",
            "TOOLSET_VERSION": "12.2.0",
            "VM_IMAGE": "windows-latest",
            "CXXSTD": "11,14,17,20",
            "MINGW_RT": "rt_v10-rev2",
        },
    }

    group_steps = {
        "Linux": [],
        "macOS": [],
        "Windows": [],
    }

    def __init__(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("--type")
        parser.add_argument("--include", default="")
        parser.add_argument("--exclude", default="")
        parser.add_argument("--groups", default="")
        parser.add_argument("--b2tests", default="[]")
        parser.add_argument("--cmaketests", default="[]")
        self.args = parser.parse_args()

        # All the groups we know about from the compilers.
        self.known_groups = set()
        for mc in self.matrix_compilers.items():
            self.known_groups.add(mc[1]["GROUP"])
        # Compute teh groups to generate from the CLI or the known toolsets.
        self.groups = set()
        if self.args.groups:
            self.groups = set(self.args.groups.split(","))
        else:
            self.groups |= self.known_groups
        # B2 & Cmake Tests are serialized json. Decode for convenience.
        self.b2tests = json.loads(self.args.b2tests)
        self.cmaketests = json.loads(self.args.cmaketests)

        if self.args.type == "matrix":
            self.gen_matrix()

    def gen_matrix(self):
        # Generate the toolsets to include.
        include = set()
        if self.args.include:
            include = set(self.args.include.split(","))
        else:
            for mc in self.matrix_compilers.items():
                if "EXCLUDE" in mc[1] and mc[1]["EXCLUDE"]:
                    continue
                if mc[1]["GROUP"] not in self.groups:
                    continue
                include.add(mc[0])
        include -= set(self.args.exclude.split(","))
        for group in self.groups:
            result = {}
            for toolset in include:
                if toolset not in self.matrix_compilers:
                    continue
                matrix_compiler = self.matrix_compilers[toolset]
                if group != matrix_compiler["GROUP"]:
                    continue
                result[matrix_compiler["NAME"]] = matrix_compiler
            result_vso = "##vso[task.setVariable variable={0};isOutput=true]".format(
                group
            )
            result_vso += json.dumps(
                result, ensure_ascii=True, sort_keys=True, indent=None
            )
            print("***", group, "Matrix...")
            print(json.dumps(result, ensure_ascii=True, sort_keys=True, indent=2))
            print(result_vso)
            print(
                "##vso[task.setVariable variable={0}Len;isOutput=true]{1}".format(
                    group, len(result)
                )
            )
        self.gen_matrix_steps(include)

    def gen_matrix_steps(self, toolsets):
        need_mingw = False
        need_b2 = False
        need_cmake = False
        for toolset in toolsets:
            if toolset.startswith("mingw-"):
                need_mingw = True
                break
        need_b2 = len(self.b2tests) > 0
        need_cmake = len(self.cmaketests) > 0
        for group in self.groups:
            result = []
            if group == "Windows":
                if need_mingw:
                    result.append(
                        {
                            "script": """\
cd %BUILD_SOURCESDIRECTORY%/..
curl "https://github.com/niXman/mingw-builds-binaries/releases/download/%TOOLSET_VERSION%-%MINGW_RT%/x86_64-%TOOLSET_VERSION%-release-posix-seh-ucrt-%MINGW_RT%.7z" -L -o mingw.7z
""",
                            "displayName": "Download MinGW",
                            "condition": "eq(variables['TOOLSET'], 'gcc')",
                        }
                    )
                    result.append(
                        {
                            "task": "ExtractFiles@1",
                            "inputs": {
                                "archiveFilePatterns": "$(Build.SourcesDirectory)/../mingw.7z",
                                "destinationFolder": "C:/",
                                "cleanDestinationFolder": False,
                                "overwriteExistingFiles": True,
                            },
                            "displayName": "Extract MinGW",
                            "condition": "eq(variables['TOOLSET'], 'gcc')",
                        }
                    )
                if need_b2:
                    result.append(
                        {
                            "script": """\
cd %BUILD_SOURCESDIRECTORY%/..
curl "https://github.com/bfgroup/b2/archive/main.zip" -L -o b2.zip
""",
                            "displayName": "Download B2",
                        }
                    )
                    result.append(
                        {
                            "task": "ExtractFiles@1",
                            "inputs": {
                                "archiveFilePatterns": "$(Build.SourcesDirectory)/../b2.zip",
                                "destinationFolder": "$(Build.SourcesDirectory)/..",
                                "cleanDestinationFolder": False,
                            },
                            "displayName": "Extract B2",
                        }
                    )
                    for b2test in self.b2tests:
                        result.append(
                            {
                                "script": """\
set BOOST_BUILD_PATH=%BUILD_SOURCESDIRECTORY%/../b2-main
cd "%BUILD_SOURCESDIRECTORY%/{dir}"
%BUILD_SOURCESDIRECTORY%/../b2-main/b2.exe toolset=%TOOLSET% cxxstd=%CXXSTD% {args}
""".format(
                                    **b2test
                                ),
                                "displayName": b2test["name"],
                            }
                        )
            print("***", group, "Steps...")
            print(json.dumps(result, ensure_ascii=True, sort_keys=True, indent=2))
            print(
                "##vso[task.setVariable variable={0}Steps;isOutput=true]{1}".format(
                    group, json.dumps(result, ensure_ascii=True, sort_keys=True)
                )
            )


if __name__ == "__main__":
    CXXPipelines()
