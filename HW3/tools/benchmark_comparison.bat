@echo off

python google_benchmark_tools/compare.py filters ../build-msvc-release/bin/benchmark.json BM_Std BM_MM
