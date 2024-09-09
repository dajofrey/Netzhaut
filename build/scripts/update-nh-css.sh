#!/bin/sh

if [ "$1" = "clean" ]; then
    rm -f src/lib/nh-css/Common/Data/default.css.inc
    rm -fr src/lib/nh-css/Common/Data
else
    mkdir -p src/lib/nh-css/Common/Data
    xxd -i -n default_css_inc build/data/misc/default.css src/lib/nh-css/Common/Data/default.css.inc
fi
