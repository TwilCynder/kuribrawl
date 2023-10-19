#! /bin/bash

cat ./dependencies/pkglist_base.txt
printf "\n"

if [ "$1" = "UCRT" ]; then
    cat ./dependencies/pkglist_ucrt64.txt
else
    cat ./dependencies/pkglist_mingw64.txt
fi