#!/bin/bash

set -o errexit

mkdir -p build
rm -rf build/*
cd build

g++ -c ../src/QP/*.cpp

g++ *.o -I../src ../src/test_qp.cpp -o test_qp

./test_qp

g++ *.o -I../src ../src/spring_chain_example.cpp -o spring_chain_example

./spring_chain_example
