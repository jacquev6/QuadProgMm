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

make tst

rm -rf docs
sphinx-build doc docs

diff <(tail -4 README.rst | sed "s|^    ||") doc/quick_start.out

show_in_browser "Documentation" $PROJECT_ROOT/docs/index.html

make post_doc

echo
echo "Development cycle OK"
