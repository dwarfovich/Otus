@echo off

../build/msvc-debug/bin/map_mean.exe < ../source_data/AB_NYC_2019.csv | ../build/msvc-debug/bin/reduce_mean.exe > ../results/mean_output.txt
