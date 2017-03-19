#!/bin/bash

file_path=src/

function usage {
    echo 'Format given file(s)'
    echo "    $0 <file name> [... <file name>]"
    echo
    echo "Format every files in $file_path"
    echo "    $0"
    echo
    echo 'Show help'
    echo "    $0 --help"
    echo "    $0 -h"
}

function format_all {
    for file in $(find "$file_path" -name '*.c' -o -name '*.h')
    do
        format_file "$file"
    done
}

function format_file {
    uncrustify -c format.cfg --no-backup -l c "$1"
}

if [ -z "$1" ]
then
    format_all
elif [ "$1" = '--help' ] || [ "$1" = '-h' ]
then
    usage
else
    for file in "$@"
    do
        format_file "$file"
    done
fi
