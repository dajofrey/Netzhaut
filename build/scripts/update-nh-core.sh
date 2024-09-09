#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f src/lib/nh-core/Common/Data/default.conf.inc
    rm -fr src/lib/nh-core/Common/Data
else
    mkdir -p src/lib/nh-core/Common/Data
    ./external/xxd/xxd -i build/data/misc/default.conf src/lib/nh-core/Common/Data/default.conf.inc default_conf_inc
fi
