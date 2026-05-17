@echo OFF

REM Usage:
REM VSWHERE_LEGACY: Version range of legacy (earlier than VS 2017) VS install to set up.
REM VSWHERE_VERSION: Version range of new VS install to set up.
REM SETUP: Specific setup command to run instead of vswhere determined VCVARSALL.BAT.

if not "%VSWHERE_LEGACY%"=="" (
	for /f "usebackq tokens=*" %%i in (`vswhere -legacy -latest -property installationPath -version "%VSWHERE_LEGACY%"`) do (
		set "VCVARSALL=%%i\VC\vcvarsall.bat"
	)
)
if not "%VSWHERE_VERSION%"=="" (
	for /f "usebackq tokens=*" %%i in (`vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -version "%VSWHERE_VERSION%"`) do (
		set "VCVARSALL=%%i\VC\Auxiliary\Build\vcvarsall.bat"
	)
)
if not "%SETUP%"=="" (
	set SETUP="echo No setup available"
)
if not "%VCVARSALL%"=="" (
	set SETUP="%VCVARSALL%" %PLATFORM%
)
%SETUP%


REM Use, modification, and distribution are
REM subject to the Boost Software License, Version 1.0. (See accompanying
REM file LICENSE.txt)
REM
REM Copyright René Ferdinand Rivera Morell 2020-2021.
