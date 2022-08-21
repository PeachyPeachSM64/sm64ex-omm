@echo off
setlocal enableDelayedExpansion
set root_path=C:
if exist "%root_path%/msys64/mingw64.exe" (
	set mingw64_path=%root_path%/msys64/mingw64.exe
) else (
	set /p mingw64_path=< mingw64_path
	if "!mingw64_path!" == "" (
		cls
		echo Trying to locate mingw64.exe in %root_path%...
		where /R %root_path% mingw64.exe > mingw64_path
		set /p mingw64_path=< mingw64_path
		if "!mingw64_path!" == "" (
			cls
			echo Couldn't find mingw64.exe, maybe it's not located in %root_path%?
			echo To change the search path, open omm_builder.bat in a text editor and change the value of root_path.
			pause
			exit
		)
	)
)
start !mingw64_path! bash omm_builder.sh
