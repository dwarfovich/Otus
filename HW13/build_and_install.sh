#! /bin/bash

sudo rm -rf build/
cmake --preset linux-release
cmake --build --preset build-linux-release
cpack --preset deb-package-release
sudo dpkg -i build/linux-release/deb/cv-0.0.1-Linux.deb