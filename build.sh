#!/bin/bash

mkdir -p build

gcc -g src/main.c \
    -o build/music \
    -Wall \
    -Werror \
    -lsqlite3 \
    -lcjson

gdb ./build/music
