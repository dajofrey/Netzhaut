#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -fr src/lib/nh-renderer/Common/Data
else
    mkdir -p src/lib/nh-renderer/Common/Data
    mkdir -p src/lib/nh-renderer/Common/Data/GLSL
    mkdir -p src/lib/nh-renderer/Common/Data/GLSL/Main
    build/scripts/compile-glsl.sh nh-renderer
fi
