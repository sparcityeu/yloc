#!/bin/bash
source scripts/env.sh
mkdir -p build && cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE && make && cd ..

