@echo off

for %%I in (.) do set CurrDirName=%%~nxI
if %CurrDirName% neq "Build" (
  mkdir Build
)
cd Build

cmake ..
cmake --build .

cd ..