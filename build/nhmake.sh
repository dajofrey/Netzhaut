#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo -e "BUILDING \e[1;32mnhmake\e[0m"
    echo
    CC=gcc
    LIBS="-ldl"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "BUILDING nhmake"
    CC="clang -g -D_POSIX_C_SOURCE -D_C99_SOURCE"
    LIBS=""
fi

DIR="$(dirname $( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd ))"

SRC_LIB="
    $DIR/src/lib/nhmake/Common/About.c \
    $DIR/src/lib/nhmake/Common/Result.c \
    $DIR/src/lib/nhmake/Core/Runtime.c \
    $DIR/src/lib/nhmake/Core/Utils.c \
    $DIR/src/lib/nhmake/Core/Thread.c \
    $DIR/src/lib/nhmake/Core/File.c \
    $DIR/src/lib/nhmake/Core/Configure.c \
    $DIR/src/lib/nhmake/Core/Build.c \
    $DIR/src/lib/nhmake/Core/Options.c \
    $DIR/src/lib/nhmake/Core/Source.c \
    $DIR/src/lib/nhmake/Parser/Tokenizer.c \
    $DIR/src/lib/nhmake/Parser/Parser.c \
    $DIR/src/lib/nhmake/Parser/Variables.c \
    $DIR/src/lib/nhmake/Parser/Functions.c \
    $DIR/src/lib/nhmake/Test/Channel.c \
    $DIR/src/lib/nhmake/Test/Process.c \
    $DIR/src/lib/nhmake/Test/Library.c \
    $DIR/src/lib/nhmake/Test/Test.c \
    $DIR/src/lib/nhmake/UI/Message.c \
"

SRC_BIN="
    $DIR/src/bin/nhmake/Main.c \
    $DIR/src/bin/nhmake/UnicodeData.c \
    $DIR/src/bin/nhmake/WebIDL.c \
    $DIR/src/bin/nhmake/About.c \
"

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    $CC -std=gnu99 -shared -o$DIR/lib/libnhmake.so.0.0.0.0 -lpthread $LIBS -fPIC $SRC_LIB
elif [[ "$OSTYPE" == "darwin"* ]]; then
    $CC -std=gnu11 -shared -o$DIR/lib/libnhmake.so.0.0.0.0 -lpthread $LIBS -fPIC $SRC_LIB
fi

ln -s $DIR/lib/libnhmake.so.0.0.0.0 $DIR/lib/libnhmake.so.0
ln -s $DIR/lib/libnhmake.so.0.0.0.0 $DIR/lib/libnhmake.so

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    $CC -std=gnu99 -no-pie -Wl,-rpath=$DIR/lib -L$DIR/lib/ -o$DIR/build/nhmake $SRC_BIN -lnhmake
elif [[ "$OSTYPE" == "darwin"* ]]; then
    $CC -std=gnu11 -Wl -L$DIR/lib/ -o$DIR/build/nhmake $SRC_BIN -lnhmake
fi

if [ $? = 0 ]; then
    echo
    echo -e "BUILD $DIR/lib/\e[1;32mnhmake\e[0m"
else
    echo
    echo -e "BUILD \e[1;31mFAILURE\e[0m"
fi

