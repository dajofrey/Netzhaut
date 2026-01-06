#!/usr/bin/env bash
set -e

INPUT_DIR="build/data/intrinsics"
OUT_DIR="src/lib/nh-ecmascript/Intrinsics"

HEADER="$OUT_DIR/PropertyMethods.gen.h"
DISPATCH_C="$OUT_DIR/PropertyMethods.gen.c"

mkdir -p "$OUT_DIR"
TMP_PROPS=$(mktemp)

echo "Scanning intrinsic files..."

for file in "$INPUT_DIR"/*.is; do
    awk '
        # 1. Capture Intrinsic Name
        /%[A-Za-z0-9]+%/ && /intrinsic/ {
            start = index($0, "%") + 1
            end = index(substr($0, start), "%")
            currentIntrinsic = substr($0, start, end - 1)
        }

        # 2. Detect the start of the properties block
        /properties[[:space:]]*\{/ { inPropsBlock = 1; next }

        # 3. If inside properties, look for a new property definition: "name": {
        inPropsBlock && /^[[:space:]]*"[^"]+"[[:space:]]*:[[:space:]]*\{/ {
            start = index($0, "\"") + 1
            end = index(substr($0, start), "\"")
            currentPropName = substr($0, start, end - 1)
            inSingleProp = 1
            hasLength = 0
            hasValue = 0
            next
        }

        # 4. Check contents of the current property block
        inSingleProp {
            if ($0 ~ /length:/) hasLength = 1
            if ($0 ~ /value:/)  hasValue = 1
            
            # 5. Detect end of the specific property block
            if ($0 ~ /\}/) {
                if (hasLength && !hasValue) {
                    print tolower(currentIntrinsic) "_" currentPropName
                }
                inSingleProp = 0
            }
        }

        # 6. Detect end of the whole properties block
        inPropsBlock && /^\}/ { inPropsBlock = 0 }

    ' "$file" >> "$TMP_PROPS"
done

sort -u "$TMP_PROPS" > "$TMP_PROPS.sorted"

# --- Header Generation ---
cat > "$HEADER" <<EOF
/* AUTO-GENERATED — DO NOT EDIT */
#pragma once
#include "../Engine/Object.h"
#include "../Engine/Value.h"
#include "../Engine/Completion.h"
#include "../Engine/Realm.h"

typedef nh_ecmascript_Completion (*nh_PropertyMethodFn)(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p
);
EOF

while read -r fullName; do
    echo "nh_ecmascript_Completion nh_ecmascript_${fullName}(nh_ecmascript_Object *Object_p, nh_ecmascript_Value *Arguments_p, size_t argumentCount, nh_ecmascript_Realm *Realm_p);" >> "$HEADER"
done < "$TMP_PROPS.sorted"

cat >> "$HEADER" <<EOF

nh_PropertyMethodFn nh_ecmascript_lookupPropertyMethod(const char *fullName); 

EOF

# --- C File Generation ---
cat > "$DISPATCH_C" <<EOF
/* AUTO-GENERATED — DO NOT EDIT */
#include "PropertyMethods.gen.h"
#include <stddef.h>

const char *nh_propertyMethodFullNames[] = {
EOF

while read -r fullName; do
    echo "    \"$fullName\"," >> "$DISPATCH_C"
done < "$TMP_PROPS.sorted"

cat >> "$DISPATCH_C" <<EOF
};

nh_PropertyMethodFn nh_propertyMethodDispatch[] = {
EOF

while read -r fullName; do
    echo "    nh_ecmascript_$fullName," >> "$DISPATCH_C"
done < "$TMP_PROPS.sorted"

cat >> "$DISPATCH_C" <<EOF
};

const size_t nh_propertyMethodCount = sizeof(nh_propertyMethodFullNames) / sizeof(nh_propertyMethodFullNames[0]);

nh_PropertyMethodFn nh_ecmascript_lookupPropertyMethod(const char *fullName) {
    for (size_t i = 0; i < nh_propertyMethodCount; i++) {
        if (strcmp(nh_propertyMethodFullNames[i], fullName) == 0) {
            return nh_propertyMethodDispatch[i];
        }
    }
    return NULL;
}

EOF

rm "$TMP_PROPS" "$TMP_PROPS.sorted"
echo "Done. Check $DISPATCH_C for the full list."
