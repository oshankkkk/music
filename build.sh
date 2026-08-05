#!/bin/bash

mkdir -p build

gcc -g $(find src -name "*.c") \
    -o build/music \
    -Wall \
    -Werror \
    -lsqlite3 \
    -lcjson

gdb ./build/music
# ./build/music
