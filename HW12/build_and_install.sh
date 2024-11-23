#! /bin/bash

sudo rm -rf build/
cmake --preset linux-debug
cmake --build --preset build-linux-debug
cpack --preset deb-package-debug
sudo dpkg -i build/linux-debug/deb/map_reduce-0.0.1-Linux.deb