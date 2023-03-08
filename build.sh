#!/bin/bash
source scripts/env.sh
mkdir -p build && cd build && cmake .. -Wdev -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE && cmake --build . -j
cd ..
