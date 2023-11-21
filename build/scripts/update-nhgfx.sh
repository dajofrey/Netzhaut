#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f src/lib/nhgfx/Common/Data/default.conf.inc
    rm -fr src/lib/nhgfx/Common/Data
else
    mkdir -p src/lib/nhgfx/Common/Data
    ./external/xxd/xxd -i  external/fonts/SourceCodePro/static/SourceCodePro-Regular.ttf src/lib/nhgfx/Common/Data/SourceCodePro-Regular.ttf.inc source_code_pro_ttf_inc
fi
