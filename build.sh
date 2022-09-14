#!/bin/bash
source scripts/env.sh
mkdir -p build && cd build && cmake .. && make && cd ..

