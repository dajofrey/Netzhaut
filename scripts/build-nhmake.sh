#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo -e "Attempting \e[1;33mnhmake\e[0m build ..."
    CC=gcc
    LIBS="-ldl"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo -e "Attempting \e[1;33mnhmake\e[0m build ..."
    CC="clang -g -D_POSIX_C_SOURCE -D_C99_SOURCE"
    LIBS=""
fi

PROJ_DIR="$(dirname $( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd ))"

SRC_LIB="
    $PROJ_DIR/src/lib/nhmake/Common/About.c \
    $PROJ_DIR/src/lib/nhmake/Common/Result.c \
    $PROJ_DIR/src/lib/nhmake/Core/Runtime.c \
    $PROJ_DIR/src/lib/nhmake/Core/Utils.c \
    $PROJ_DIR/src/lib/nhmake/Core/Thread.c \
    $PROJ_DIR/src/lib/nhmake/Core/File.c \
    $PROJ_DIR/src/lib/nhmake/Core/Configure.c \
    $PROJ_DIR/src/lib/nhmake/Core/Build.c \
    $PROJ_DIR/src/lib/nhmake/Core/Options.c \
    $PROJ_DIR/src/lib/nhmake/Core/Source.c \
    $PROJ_DIR/src/lib/nhmake/Parser/Tokenizer.c \
    $PROJ_DIR/src/lib/nhmake/Parser/Parser.c \
    $PROJ_DIR/src/lib/nhmake/Parser/Variables.c \
    $PROJ_DIR/src/lib/nhmake/Parser/Functions.c \
    $PROJ_DIR/src/lib/nhmake/Test/Channel.c \
    $PROJ_DIR/src/lib/nhmake/Test/Process.c \
    $PROJ_DIR/src/lib/nhmake/Test/Library.c \
    $PROJ_DIR/src/lib/nhmake/Test/Test.c \
    $PROJ_DIR/src/lib/nhmake/UI/Message.c \
"

SRC_BIN="
    $PROJ_DIR/src/bin/nhmake/Main.c \
    $PROJ_DIR/src/bin/nhmake/UnicodeData.c \
    $PROJ_DIR/src/bin/nhmake/WebIDL.c \
    $PROJ_DIR/src/bin/nhmake/About.c \
"

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    $CC -std=gnu99 -shared -o$PROJ_DIR/lib/libnhmake.so.0.0.0.0 -lpthread $LIBS -fPIC $SRC_LIB
elif [[ "$OSTYPE" == "darwin"* ]]; then
    $CC -std=gnu11 -shared -o$PROJ_DIR/lib/libnhmake.so.0.0.0.0 -lpthread $LIBS -fPIC $SRC_LIB
fi

if [ $? = 0 ]; then
    echo -e "Build $PROJ_DIR/lib/\e[1;33mnhmake\e[0m \e[1;32mSUCCESS\e[0m"
else
    echo -e "Build $PROJ_DIR/lin/\e[1;33mnhmake\e[0m \e[1;31mFAILURE\e[0m"
fi

ln -s $PROJ_DIR/lib/libnhmake.so.0.0.0.0 $PROJ_DIR/lib/libnhmake.so.0
ln -s $PROJ_DIR/lib/libnhmake.so.0.0.0.0 $PROJ_DIR/lib/libnhmake.so

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    $CC -std=gnu99 -no-pie -Wl,-rpath=$PROJ_DIR/lib -L$PROJ_DIR/lib/ -o$PROJ_DIR/bin/nhmake $SRC_BIN -lnhmake
elif [[ "$OSTYPE" == "darwin"* ]]; then
    $CC -std=gnu11 -Wl -L$PROJ_DIR/lib/ -o$PROJ_DIR/bin/nhmake $SRC_BIN -lnhmake
fi

if [ $? = 0 ]; then
    echo -e "Build $PROJ_DIR/bin/\e[1;33mnhmake\e[0m \e[1;32mSUCCESS\e[0m"
else
    echo -e "Build $PROJ_DIR/bin/\e[1;33mnhmake\e[0m \e[1;31mFAILURE\e[0m"
fi

