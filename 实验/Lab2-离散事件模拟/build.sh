#!/bin/sh
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make -j