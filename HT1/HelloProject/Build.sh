#!/bin/sh

mkdir -p BuildWsl
cd BuildWsl
cmake -DPATCH_VERSION=9999 -DUSE_BOOST_TEST=ON ..
cmake --build . 
cpack -G DEB
cd ..
