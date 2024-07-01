@echo off

for %%I in (.) do set CurrDirName=%%~nxI
if %CurrDirName% neq "Build" (
  mkdir Build
)
rem cd Build

rem cmake ..
rem cmake --build .
cmake -BBuild-MSVC
cmake --build Build-MSVC
rem -S/path/to/my/source/folder

rem cd ..