#!/usr/bin/env sh
cd docs
make
rst2man.py ridl.rst > ridl.1
