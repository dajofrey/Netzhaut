#!/bin/bash

echo -e "BUILDING \e[1;32mNETZHAUT META BUILD SYSTEM\e[0m"
echo 

DIR="$(dirname $(dirname $( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )))"
gcc -std=gnu99 -o$DIR/build/linux -lpthread $DIR/source/Build/Source/Browser.c $DIR/source/Build/Source/Library.c $DIR/source/Build/Source/Docs.c $DIR/source/Build/Source/Main.c $DIR/source/Build/Source/Download.c $DIR/source/Build/Source/Utils.c $DIR/source/Build/Source/Externals.c $DIR/source/API/Source/Results.c

if [ $? = 0 ]; then
    echo
    echo -e "BUILD \e[1;32mSUCCESS\e[0m"
    echo -e "THE EXECUTABLE IS NAMED \e[1;32mLINUX\e[0m"
    echo YOU CAN FIND THE EXECUTABLE IN NETZHAUT/BUILD
else
    echo
    echo -e "BUILD \e[1;31mFAILURE\e[0m"
fi
