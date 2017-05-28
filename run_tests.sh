#!/bin/bash

build_dir=build
tests="game matrix neuralnet test"

for t in $tests
do
    echo
    echo -e "-- \x1b[1;36mRunning test_$t\x1b[0m --"
    "./$build_dir/test_$t"
done
