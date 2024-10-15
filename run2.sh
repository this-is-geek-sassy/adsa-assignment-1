#!/bin/bash

rm -rf ./binary_tree_gui
gcc -o binary_tree_gui program2.c `pkg-config --cflags --libs gtk+-3.0`
./binary_tree_gui
