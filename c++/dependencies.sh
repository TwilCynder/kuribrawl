#! /bin/bash

cat ./dependencies/pkglist_base.txt
printf "\n"

mode="mingw"
devpkgs=false

for var in "$@"
do
    case "$var" in
        '-d' | '--dev' | '--opt') 
            devpkgs=true  
            ;;
        *) mode=$var    
    esac
done

cat ./dependencies/pkglist_${mode}64.txt
if $devpkgs; then
    printf "\n"
    cat ./dependencies/pkglist_${mode}64_opt.txt
fi