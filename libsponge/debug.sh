#!/bin/sh
echo Begin compiling...
g++ debug.cpp wrapping_integers.cc
echo Compiled.
./a.out
