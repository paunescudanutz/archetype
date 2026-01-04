#!/bin/bash

BIN_NAME="your_binary_name_here"
BIN_PATH="bin/$BIN_NAME"

# compile
./build.sh
gdb $BIN_PATH
