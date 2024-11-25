@echo off

../build/msvc-debug/bin/map_dispersion.exe < ../source_data/AB_NYC_2019.csv | ../build/msvc-debug/bin/reduce_dispersion.exe > ../results/mean_output.txt
