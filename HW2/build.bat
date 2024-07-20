@echo off

cmake -DBuildTests=ON --preset msvc-release
cmake --build --preset msvc-build-release