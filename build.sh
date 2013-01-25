#!/bin/bash

mkdir -p build
mkdir -p inst
cd build
if [ -f Makefile ]
then
    make distclean
fi
../configure --prefix=/home/kbrown/src/toddcox/toddcox.git/inst \
    && make && make install
cd ..
