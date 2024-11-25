#! /bin/bash

: '
set MapMeanPath="../build/msvc-debug/bin/map_mean.exe"
set ReduceMeanPath="../build/msvc-debug/bin/reduce_mean.exe"
set SourceDataFolderPath="../source_data/test_data"
set ResultsFolderPath="../results/tests"
'

MapMeanPath="../build/linux-debug/bin/map_mean.exe"
ReduceMeanPath="../build/linux-debug/bin/reduce_mean.exe"
SourceDataFolderPath="../source_data/test_data"
ResultsFolderPath="../results/tests"

# if not exist %ResultsFolderPath% mkdir %ResultsFolderPath%

for %%f in (%SourceDataFolderPath%/*.txt) do (
  %MapMeanPath% 0 < %SourceDataFolderPath%/%%f | %ReduceMeanPath% > %ResultsFolderPath%/m_result_%%f
)

