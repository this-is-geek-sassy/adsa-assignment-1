#!/bin/bash

rm -rf record2.txt
rm -rf ./binary_tree_gui
gcc -o binary_tree_gui program2.c `pkg-config --cflags --libs gtk+-3.0` -lgsl -lgslcblas -lm
./binary_tree_gui
