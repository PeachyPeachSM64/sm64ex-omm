@echo off
set mingw64exe="%appdata%\msys2\mingw64.exe"
if exist %mingw64exe% (
	%mingw64exe% bash omm_builder.sh
) else (
	echo mingw64.exe not found. Please run the batch script 'omm_builder_setup.bat' first.
	pause
)
