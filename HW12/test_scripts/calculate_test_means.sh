#! /bin/bash

MapMeanPath="../build/linux-debug/bin/map_mean"
ReduceMeanPath="../build/linux-debug/bin/reduce_mean"
SourceDataFolderPath="../source_data/test_data"
ResultsFolderPath="../results/tests"

mkdir -p $ResultsFolderPath
for filePath in $SourceDataFolderPath/*; do
	filename=$(basename ${filePath})
	$MapMeanPath 0 < $filePath | $ReduceMeanPath > $ResultsFolderPath/m_result_$filename
done

