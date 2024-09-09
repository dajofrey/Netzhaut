#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f src/lib/nh-gfx/Common/Data/default.conf.inc
    rm -fr src/lib/nh-gfx/Common/Data
else
    mkdir -p src/lib/nh-gfx/Common/Data
    ./external/xxd/xxd -i  external/fonts/SourceCodePro/static/SourceCodePro-Regular.ttf src/lib/nh-gfx/Common/Data/SourceCodePro-Regular.ttf.inc source_code_pro_ttf_inc
fi
