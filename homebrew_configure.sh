#!/bin/sh

GIT_HASH=`git log -1 --pretty=format:%h`
PREFIX_ROOT="$HOME/software"
PREFIX="$PREFIX_ROOT/ridl-$GIT_HASH"

rm -rf build
mkdir build
cd build

cmake -DReadline_INCLUDE_DIR=/usr/local/Cellar/readline/6.2.1/include \
      -DReadline_LIBRARY=/usr/local/Cellar/readline/6.2.1/lib/libreadline.dylib \
      -DCMAKE_INSTALL_PREFIX=$PREFIX \
      ..
