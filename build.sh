#!/bin/bash

BIN_NAME="your_binary_name_here"
BIN_PATH="bin/$BIN_NAME"

# remove old binary
rm $BIN_PATH

# compile
gcc -g src/*.c lib/*.c test/*.c -lm -o $BIN_PATH

