#!/bin/bash
export app="/usr/bin/spim -f scroll.s"

make clean > /dev/null
make

echo

if [ $# -eq 0 ]
  then
    $app test
  else
    $app "${@:1}"
fi
