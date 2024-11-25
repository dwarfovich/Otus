@echo off
setlocal enabledelayedexpansion

set MapMeanPath="../build/msvc-debug/bin/map_dispersion.exe"
set ReduceMeanPath="../build/msvc-debug/bin/reduce_dispersion.exe"
set SourceDataFolderPath="../source_data/test_data"
set ResultsFolderPath="../results/tests"

if not exist %ResultsFolderPath% mkdir %ResultsFolderPath%

for %%f in (%SourceDataFolderPath%/*.txt) do (
  %MapMeanPath% 0 < %SourceDataFolderPath%/%%f | %ReduceMeanPath% > %ResultsFolderPath%/d_result_%%f
)

