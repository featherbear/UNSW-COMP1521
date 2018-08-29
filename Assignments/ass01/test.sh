#!/bin/bash
export app="spim -f exe.s"

make clean > /dev/null
make

cat chars.s scroll.s > exe.s

echo

if [ $# -eq 0 ]
  then
    $app test
  else
    $app "${@:1}"
fi
