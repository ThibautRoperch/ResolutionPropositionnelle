#!/bin/sh

# Compilation des sources

make clean
make

gnuplot "matrix.gps"
