#!/bin/bash

rm -rf record.txt
rm -rf program
gcc program1.c -o program -lgsl -lgslcblas -lm
./program

