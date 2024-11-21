#! /bin/bash

cd "${BASH_SOURCE%/*}"
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
        '-h | '--help) echo "Usage : $(basename "$0") [ucrt] \nOutputs a newline-separated list of pacman packages" ;;
        *) mode=$var    
    esac
done

cat ./dependencies/pkglist_${mode}64.txt
if $devpkgs; then
    printf "\n"
    cat ./dependencies/pkglist_${mode}64_opt.txt
fi