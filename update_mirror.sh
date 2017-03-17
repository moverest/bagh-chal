#!/bin/bash

git clone 'git@github.com:/moverest/cshellgame'
cd cshellgame || exit
git remote add forge 'https://forge.telecomnancy.univ-lorraine.fr/git/martine96u_c'
git push --mirror forge
cd ..
rm -rf cshellgame
