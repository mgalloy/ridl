#!/bin/sh

cmake -DReadline_INCLUDE_DIR=/usr/include \
      -DReadline_LIBRARY=/usr/lib64/libreadline.so \
      -DCMAKE_INSTALL_PREFIX=install \
      .
