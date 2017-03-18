#!/bin/bash

function print_usage {
    echo "Usage: $0 [init|sync|is_init|--help|-h]"
    echo
    echo '   init        Initialise the mirror remote'
    echo '   sync        Update the mirror'
    echo '   is_init     Check whether the mirror is configured or not'
    echo '   --help, -h  Show help'
    echo
    echo "Without any arguments, $0 first checks if the remote is configured and then updates the mirror."
}

function init {
    git remote add mirror 'https://forge.telecomnancy.univ-lorraine.fr/git/martine96u_c'
}

function sync {
    git push --mirror mirror
}

function is_init {
    git remote | grep mirror > /dev/null
}

case "$1" in
    --help )
        print_usage
        ;;
    -h )
        print_usage
        ;;

    init )
        init
        ;;

    sync )
        sync
        ;;

    is_init )
        is_init && echo initialised || echo not initialised && false
        ;;

    * )
        is_init || init
        sync
        ;;

esac
