#!/bin/bash

mkdir -p build-w32
mkdir -p inst-w32
cd build-w32
if [ -f Makefile ]
then
    make distclean
fi
../configure --prefix=/home/kbrown/src/toddcox/toddcox.git/inst-w32 \
    --build=i686-pc-cygwin --host=i686-pc-mingw32 CXXFLAGS=-static \
    && make && make install
cd ..
