#!/bin/bash
source scripts/env.sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DBUILD_DOC=ON
cmake --build build -j $(nproc)

# To build the documentation files, set -DBUILD_DOC=ON and
# cd build/docs/latex
# make -j $(nproc)
cd -
