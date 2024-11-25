#! /bin/bash

echo Calculating mean...
./map_mean < AB_NYC_2019.csv | ./reduce_mean > output_mean.txt
echo Result written into /opt/map_reduce/output_mean.txt

echo Calculating dispersion...
./map_dispersion < AB_NYC_2019.csv | ./reduce_dispersion > output_dispersion.txt
echo Result written into /opt/map_reduce/output_dispersion.txt