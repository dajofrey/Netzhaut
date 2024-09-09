#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f src/lib/nh-url/Common/Data/default.conf.inc
    rm -fr src/lib/nh-url/Common/Data
else
    mkdir -p src/lib/nh-url/Common/Data
    ./external/xxd/xxd -i build/data/misc/IdnaMappingTable.txt src/lib/nh-url/Common/Data/IdnaMappingTable.txt.inc idna_mapping_table_txt_inc
fi
