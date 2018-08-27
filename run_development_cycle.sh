#!/bin/bash

set -o errexit

PROJECT_ROOT=$(pwd)

SHOW_IN_BROWSER=false
function show_in_browser {
  echo
  echo "$1: $2"
  echo
  if $SHOW_IN_BROWSER
  then
    python -m webbrowser -t $2
  fi
}

while [[ "$#" > 0 ]]
do
  case $1 in
    -wb|--web-browser)
      SHOW_IN_BROWSER=true
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1;;
  esac
  shift
done


if ! [ -f QuadProgpp/src/libquadprog.a ]
then
  git submodule init
  git submodule update

  cd QuadProgpp
  cmake .
  make
  cd ..
fi

mkdir -p build
rm -rf build/*
cd build
g++ -I../QuadProgpp/src -c ../src/QP/*.cpp
cd ..

sphinx-build doc docs

show_in_browser "Documentation" $PROJECT_ROOT/docs/index.html

cd build

g++ -I../src ../doc/user_guide/artifacts/spring_chain_example.cpp *.o ../QuadProgpp/src/libquadprog.a -o spring_chain_example
./spring_chain_example >../doc/user_guide/spring_chain_example.out

g++ -I../src ../doc/user_guide/artifacts/quick_start.cpp *.o ../QuadProgpp/src/libquadprog.a -o quick_start
./quick_start >../doc/user_guide/quick_start.out

cd ..
