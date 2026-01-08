#ifndef NH_ECMASCRIPT_INTRINSICS_INTRINSICS_H
#define NH_ECMASCRIPT_INTRINSICS_INTRINSICS_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Engine/Object.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/String.h"

// ENUMS =====================================================================

typedef enum NH_ECMASCRIPT_TEMPLATE_NODE_E {
    NH_ECMASCRIPT_TEMPLATE_NODE_STRING = 0,      // "toString"
    NH_ECMASCRIPT_TEMPLATE_NODE_NUMBER,      // 1.0
    NH_ECMASCRIPT_TEMPLATE_NODE_BOOLEAN,     // true
    NH_ECMASCRIPT_TEMPLATE_NODE_IDENTIFIER,  // writable
    NH_ECMASCRIPT_TEMPLATE_NODE_NULL,        // null
    NH_ECMASCRIPT_TEMPLATE_NODE_INTRINSIC,  // writable
    NH_ECMASCRIPT_TEMPLATE_NODE_NODES,        // { a, b, c }
    NH_ECMASCRIPT_TEMPLATE_NODE_PAIRS,      // { key: value, ... }
} NH_ECMASCRIPT_TEMPLATE_NODE_E;

// STRUCTS ========================================================

typedef struct nh_ecmascript_TemplateNode {
    NH_ECMASCRIPT_TEMPLATE_NODE_E kind;
    union {
        char *string_p; 
        double number;
        bool boolean;
        nh_core_List Nodes;
        nh_core_List Pairs;
    } data;
} nh_ecmascript_TemplateNode;

typedef struct nh_ecmascript_KeyValuePair {
    char *key;
    bool key_is_symbol; // true if [Symbol] or @@
    nh_ecmascript_TemplateNode *value;
} nh_ecmascript_KeyValuePair;

typedef struct nh_ecmascript_IntrinsicTemplate {
    char *name; // intrinsic identifier (without %)
    nh_core_List InternalSlots;
    nh_core_List Properties;
} nh_ecmascript_IntrinsicTemplate;

// FUNCTIONS ====================================================================

nh_core_List nh_ecmascript_parseIntrinsicTemplates(
);

void *nh_ecmascript_getNativeSteps(
    char *objectName_p, char *functionName_p
);

nh_ecmascript_Value nh_ecmascript_resolveInternalSlot(
    nh_ecmascript_TemplateNode *Node_p,
    nh_ecmascript_Realm *Realm_p
);

nh_ecmascript_Value nh_ecmascript_resolveProperty(
    char *parentName_p,
    nh_ecmascript_KeyValuePair *Pair_p,
    nh_ecmascript_Realm *Realm_p
);

uint8_t nh_ecmascript_getAttributesFromNode(
    nh_ecmascript_TemplateNode *node
);

#endif
