#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f src/lib/nhurl/Common/Data/default.conf.inc
    rm -fr src/lib/nhurl/Common/Data
else
    mkdir -p src/lib/nhurl/Common/Data
    ./external/xxd/xxd -i build/data/misc/IdnaMappingTable.txt src/lib/nhurl/Common/Data/IdnaMappingTable.txt.inc idna_mapping_table_txt_inc
fi
