#!/bin/bash

mkdir -p build

gcc music.c -o build/music -Wall -Werror

./build/music

