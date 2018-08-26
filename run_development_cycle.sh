#!/bin/bash

set -o errexit

git submodule init
git submodule update

cd QuadProgpp
cmake .
make
cd ..

mkdir -p build
rm -rf build/*
cd build

g++ -I../QuadProgpp/src -c ../src/QP/*.cpp

g++ -I../src ../src/test_qp.cpp *.o ../QuadProgpp/src/libquadprog.a -o test_qp

./test_qp

g++ -I../src ../src/spring_chain_example.cpp *.o ../QuadProgpp/src/libquadprog.a -o spring_chain_example

./spring_chain_example
