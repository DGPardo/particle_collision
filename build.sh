#!/bin/sh

set -e
set -x

rm -rf build
mkdir build
cd build

mode=Release

conan install .. -s build_type=$mode --build mising

cmake .. -DCMAKE_BUILD_TYPE=$mode
cmake --build .
ctest -n --show-progress
