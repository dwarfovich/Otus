#! /bin/bash

./map_mean < AB_NYC_2019.csv | ./reduce_mean > output_mean.txt
echo Result written into /opt/map_reduce/output_mean.txt