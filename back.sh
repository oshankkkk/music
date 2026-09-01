#!/bin/bash

mkdir -p build

gcc -g $(find src -name '*.c') -o build/back -Wall -Werror -lsqlite3 -lcjson

gdb ./build/back

