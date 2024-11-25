#! /bin/bash

MapDispersionPath="../build/linux-debug/bin/map_dispersion"
ReduceDispersionPath="../build/linux-debug/bin/reduce_dispersion"
SourceDataFolderPath="../source_data/test_data"
ResultsFolderPath="../results/tests"

mkdir -p $ResultsFolderPath
for filePath in $SourceDataFolderPath/*; do
	filename=$(basename ${filePath})
	$MapDispersionPath 0 < $filePath | $ReduceDispersionPath > $ResultsFolderPath/d_result_$filename
done

