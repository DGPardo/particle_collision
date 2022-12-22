#!/bin/sh

set -e
set -x

sudo apt-get update
sudo apt-get satisfy -y "cmake (>= 3.22.0)"
sudo apt-get satisfy -y "g++ (>= 11)"
sudo apt-get install -y build-essential
sudo apt-get install -y pkg-config
sudo apt-get -y install python3 python3-dev python3-pip
sudo apt-get install -y libglfw3-dev mesa-common-dev \
                        libx11-xcb-dev libfontenc-dev libice-dev libsm-dev libxaw7-dev libxcomposite-dev libxcursor-dev libxdamage-dev \
                        libxfixes-dev libxi-dev libxinerama-dev libxkbfile-dev libxmu-dev libxmuu-dev libxpm-dev libxres-dev libxss-dev \
                        libxt-dev libxtst-dev libxv-dev libxvmc-dev libxxf86vm-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev \
                        libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev \
                        libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-dri3-dev uuid-dev libxcb-util-dev libxcb-util0-dev libglu1-mesa-dev
pip install conan

rm -rf build
mkdir build
cd build

mode=Release

conan install .. --build mising \
                 -s build_type=$mode \
                 -s compiler.libcxx=libstdc++11 \
                 -s compiler=gcc \
                 -s compiler.version=11 \
                 -c tools.system.package_manager:mode=install

cmake .. -DCMAKE_BUILD_TYPE=$mode
cmake --build .
ctest -n --show-progress
