#!/bin/bash
KURIDIR="$( realpath $( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/..)"

(
cd $KURIDIR &&
cd c++ &&
make NOCLEAR=true &&
cd .. &&
cp c++/kuribrawl.exe release/files/kuribrawl.exe &&
cp res/data.twl release/files/data.twl &&
makensis build/installer.nsi
)