#!/bin/bash

#gcc simplex.c -I/usr/local/inlcude -o simplexExec -std=c99

gcc -Wall -I/usr/local/include -c simplex.c -o simplex.o -std=c99
gcc -L/usr/local/lib simplex.o -lgslcblas -lgsl -o simplexExec
