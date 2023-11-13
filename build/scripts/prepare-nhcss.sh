#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f src/lib/nhcss/Common/Data/default.css.inc
    rm -fr src/lib/nhcss/Common/Data
else
    mkdir -p src/lib/nhcss/Common/Data
    ./external/xxd/xxd -i build/data/misc/default.css src/lib/nhcss/Common/Data/default.css.inc default_css_inc
fi
