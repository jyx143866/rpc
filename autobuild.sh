###
 # @Author: jyx
 # @Date: 2025-04-10 05:36:59
 # @LastEditors: jyx
 # @Description: 
### 
#! /bin/bash
set -e
rm -rf `pwd`/build/*
mkdir -p bin
mkdir -p build
cd `pwd`/build &&
    cmake .. &&
    make

cd ..
cp -r `pwd`/src/include `pwd`/lib