#!/bin/bash

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
