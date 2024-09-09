#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <input>"
    exit 1
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Download

glslang_path=${SCRIPT_DIR}/../../external/glslang-main
if [ -d "$glslang_path" ]; then
    echo "$glslang_path already exists!"
else
    cd ${SCRIPT_DIR}/../../external
    wget -O tmp.zip https://github.com/KhronosGroup/glslang/archive/main.zip
    unzip -o tmp.zip
    rm tmp.zip
    cd glslang-main
    cmake -DENABLE_OPT=0 . && make
fi

# Compile

cd ${SCRIPT_DIR}/../../external/glslang-main/StandAlone

if [ "$1" = "nh-renderer" ]; then
    ./glslangValidator -V -x -o ../../../src/lib/nh-renderer/Common/Data/GLSL/Main/Color.vert.inc ../../../build/data/glsl/nh-renderer/Main/Color.vert
    ./glslangValidator -V -x -o ../../../src/lib/nh-renderer/Common/Data/GLSL/Main/Color.frag.inc ../../../build/data/glsl/nh-renderer/Main/Color.frag
    ./glslangValidator -V -x -o ../../../src/lib/nh-renderer/Common/Data/GLSL/Main/BackgroundImage.vert.inc ../../../build/data/glsl/nh-renderer/Main/BackgroundImage.vert
    ./glslangValidator -V -x -o ../../../src/lib/nh-renderer/Common/Data/GLSL/Main/BackgroundImage.frag.inc ../../../build/data/glsl/nh-renderer/Main/BackgroundImage.frag
    ./glslangValidator -V -x -o ../../../src/lib/nh-renderer/Common/Data/GLSL/Main/Image.vert.inc ../../../build/data/glsl/nh-renderer/Main/Image.vert
    ./glslangValidator -V -x -o ../../../src/lib/nh-renderer/Common/Data/GLSL/Main/Image.frag.inc ../../../build/data/glsl/nh-renderer/Main/Image.frag
    ./glslangValidator -V -x -o ../../../src/lib/nh-renderer/Common/Data/GLSL/Main/TextSDF.vert.inc ../../../build/data/glsl/nh-renderer/Main/TextSDF.vert
    ./glslangValidator -V -x -o ../../../src/lib/nh-renderer/Common/Data/GLSL/Main/TextSDF.frag.inc ../../../build/data/glsl/nh-renderer/Main/TextSDF.frag
fi

if [ "$1" = "nhterminal" ]; then
    cd external/glslang-main/StandAlone
    ./glslangValidator -V -x -o ../../../src/lib/nhterminal/Common/Data/GLSL/TextSDF.vert.inc ../../../build/data/glsl/nhterminal/TextSDF.vert
    ./glslangValidator -V -x -o ../../../src/lib/nhterminal/Common/Data/GLSL/TextSDF.frag.inc ../../../build/data/glsl/nhterminal/TextSDF.frag
fi
