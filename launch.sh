#! /bin/bash

cd "${BASH_SOURCE%/*}";
case "$1" in
    "dfm") "./tools/gamefile manager/DFM" -v ./res -o ./res ;;
    "editor") java -jar tools/editor/javaEditor/kuribrawl-editor.jar ./res
esac
