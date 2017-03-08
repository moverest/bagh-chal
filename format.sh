#!/bin/bash

for file in $(find src -name '*.c' -o -name '*.h')
do
    uncrustify -c format.cfg --no-backup -l c $file
done
