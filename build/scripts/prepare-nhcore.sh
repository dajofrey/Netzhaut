#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f src/lib/nhcore/Common/Data/default.conf.inc
    rm -fr src/lib/nhcore/Common/Data
else
    mkdir -p src/lib/nhcore/Common/Data
    ./external/xxd/xxd -i build/data/misc/default.conf src/lib/nhcore/Common/Data/default.conf.inc default_conf_inc
fi
