#!/bin/bash
bash patch.sh

mkdir ../build
cd ../build
../configure CFLAGS="-g -O0" CXXFLAGS="-g -O0"
make -j 40
cd -
