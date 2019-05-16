#!/bin/bash
mkdir -p Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make 
cd -

mkdir -p Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make 
cd -
