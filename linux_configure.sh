#!/bin/sh

rm -rf build
mkdir build
cd build

cmake -DReadline_INCLUDE_DIR=/usr/include \
      -DReadline_LIBRARY=/usr/lib64/libreadline.so \
      -DCMAKE_INSTALL_PREFIX=~/software/ridl \
      ..
