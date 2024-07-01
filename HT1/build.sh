#!/bin/sh

cmake -DPATCH_VERSION=9999 -DUSE_BOOST_TEST=ON -BBuild-Wsl
cmake --build Build-Wsl
cpack -G DEB
