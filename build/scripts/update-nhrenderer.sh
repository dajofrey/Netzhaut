#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -fr src/lib/nhrenderer/Common/Data
else
    mkdir -p src/lib/nhrenderer/Common/Data
    mkdir -p src/lib/nhrenderer/Common/Data/GLSL
    mkdir -p src/lib/nhrenderer/Common/Data/GLSL/Main
    build/scripts/compile-glsl.sh nhrenderer
fi
