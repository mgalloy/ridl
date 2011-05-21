#!/usr/bin/env sh
cd docs
make
rst2man.py manual.rst > ridl.1
