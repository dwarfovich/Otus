#!/bin/sh

cmake -DPATCH_VERSION=9999 -DBUILD_BOOST_TEST=ON -DSETUP_DEB_PACKAGE=ON -BBuild-Wsl
cmake --build Build-Wsl

