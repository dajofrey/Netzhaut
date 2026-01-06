#!/usr/bin/env bash
set -e

INPUT_DIR="build/data/intrinsics"
OUT_DIR="src/lib/nh-ecmascript/Intrinsics"

HEADER="$OUT_DIR/InternalMethods.gen.h"
DISPATCH_C="$OUT_DIR/InternalMethods.gen.c"

mkdir -p "$OUT_DIR"


# Temp file for collected method names
TMP_METHODS=$(mktemp)

echo "Scanning intrinsic files..."


# 1. Extract method implementation names
grep -R "internalMethods" -n "$INPUT_DIR"/*.is | cut -d: -f1 | sort -u | while read -r file; do
    awk '
        /internalMethods[[:space:]]*\{/ { inBlock=1; next }
        inBlock && /\}/ { inBlock=0 }
        inBlock && /\[\[/ {
            gsub(/.*\]\]:[[:space:]]*/, "", $0)
            gsub(/[[:space:]]*/, "", $0)
            print
        }
    ' "$file"
done >> "$TMP_METHODS"

sort -u "$TMP_METHODS" > "$TMP_METHODS.sorted"

echo
echo "Found internal method implementations:"
while read -r name; do
    echo "  $name"
done < "$TMP_METHODS.sorted"
echo


# 2. Generate header
cat > "$HEADER" <<EOF
#pragma once

#include "../Engine/Object.h"
#include "../Engine/Value.h"
#include "../Engine/Completion.h"
#include "../Engine/Realm.h"

typedef nh_ecmascript_Completion (*nh_InternalMethodFn)(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);
EOF

while read -r name; do
first=$(echo "${name:0:1}" | tr '[:upper:]' '[:lower:]')
# Get the rest of the string
rest="${name:1}"
# Recombine
c_name="${first}${rest}"
cat >> "$HEADER" <<EOF
nh_ecmascript_Completion nh_ecmascript_${c_name}(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);

EOF
done < "$TMP_METHODS.sorted"

cat >> "$HEADER" <<EOF
extern nh_InternalMethodFn
    nh_internalMethodDispatch[NH_IMPL_COUNT];
EOF


# 3. Generate dispatch + name tables
cat > "$DISPATCH_C" <<EOF
/* AUTO-GENERATED — DO NOT EDIT */
#include "InternalMethods.h"
#include <stddef.h>

const char *nh_internalMethodNames[] = {
EOF

while read -r name; do
    echo "    \"$name\"," >> "$DISPATCH_C"
done < "$TMP_METHODS.sorted"

cat >> "$DISPATCH_C" <<EOF
};

nh_InternalMethodFn nh_internalMethodDispatch[] = {
EOF

while read -r name; do
    first=$(echo "${name:0:1}" | tr '[:upper:]' '[:lower:]')
    rest="${name:1}"
    c_name="${first}${rest}"
    echo "    nh_ecmascript_$c_name," >> "$DISPATCH_C"
done < "$TMP_METHODS.sorted"

cat >> "$DISPATCH_C" <<EOF
};

const size_t nh_internalMethodCount =
    sizeof(nh_internalMethodNames) / sizeof(nh_internalMethodNames[0]);
EOF


rm "$TMP_METHODS" "$TMP_METHODS.sorted"

echo "Generated:"
echo "  $HEADER"
echo "  $DISPATCH_C"
