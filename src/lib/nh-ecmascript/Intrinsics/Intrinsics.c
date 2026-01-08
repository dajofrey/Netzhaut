// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Intrinsics.h"
#include "Templates.h"
#include "Parser.h"

#include "../Engine/PropertyDescriptor.h"
#include "../Common/Log.h"

#include <string.h>
#include <dlfcn.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

nh_core_List nh_ecmascript_parseIntrinsicTemplates()
{
    nh_core_List Templates = nh_core_initList(NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP_COUNT);

    for (int i = 0; i < NH_ECMASCRIPT_INTRINSIC_TEMPLATE_NAMES_PP_COUNT; ++i) {
        nh_core_Array Tokens = nh_ecmascript_tokenizeIntrinsicTemplate(NULL, NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP[i]);
        nh_ecmascript_IntrinsicTemplate *Template_p = nh_ecmascript_parseIntrinsicTemplate(Tokens.p, Tokens.length);
//        nh_ecmascript_logIntrinsicTemplate(Template_p);
        nh_core_freeArray(&Tokens);
        nh_core_appendToList(&Templates, Template_p);
    }

    return Templates;
}   

void *nh_ecmascript_getNativeSteps(
    char *objectName_p,
    char *functionName_p) 
{
    if (!objectName_p || !functionName_p) return NULL;

    // 1. Calculate length: "nh_ecmascript_" (14) + obj + "_" (1) + func + \0 (1)
    size_t objLen  = strlen(objectName_p);
    size_t funcLen = strlen(functionName_p);
    size_t totalLen = 14 + objLen + 1 + funcLen + 1;

    char *symbolName = malloc(totalLen);
    if (!symbolName) return NULL;

    // 2. Build the string: "nh_ecmascript_Object_assign"
    snprintf(symbolName, totalLen, "nh_ecmascript_%s_%s", objectName_p, functionName_p);

    // 3. Lowercase the first char of objectName (starts at index 14)
    if (objLen > 0) {
        symbolName[14] = tolower((unsigned char)symbolName[14]);
    }

    // 4. Lowercase the first char of functionName (starts after "nh_ecmascript_<obj>_")
    // The underscore is at index (14 + objLen)
    if (funcLen > 0) {
        symbolName[14 + objLen + 1] = tolower((unsigned char)symbolName[14 + objLen + 1]);
    }

    // 5. Look up the symbol
    // RTLD_DEFAULT looks in the main executable and shared libraries
    void *p = dlsym(RTLD_DEFAULT, symbolName);

    // Debug print to verify the generated string
    // printf("Looking for symbol: %s -> %p\n", symbolName, p);

    free(symbolName); // Free the temporary buffer
    return p;
}

uint8_t nh_ecmascript_getAttributesFromNode(
    nh_ecmascript_TemplateNode *node)
{
    if (node->kind != NH_ECMASCRIPT_TEMPLATE_NODE_PAIRS) {
        return 0x05; // Default: Writable | Configurable
    }

    for (int i = 0; i < node->data.Pairs.size; i++) {
        nh_ecmascript_KeyValuePair *pair = node->data.Pairs.pp[i];
        
        if (strcmp(pair->key, "attributes") == 0) {
            uint8_t flags = 0;
            // Assume attributes value is a list of nodes: ["writable", "configurable"]
            nh_core_List *list = &pair->value->data.Nodes;
            for (int j = 0; j < list->size; j++) {
                nh_ecmascript_TemplateNode *attr = list->pp[j];
                if (strcmp(attr->data.string_p, "writable") == 0)     flags |= 0x01;
                if (strcmp(attr->data.string_p, "enumerable") == 0)   flags |= 0x02;
                if (strcmp(attr->data.string_p, "configurable") == 0) flags |= 0x04;
            }
            return flags;
        }
    }
    
    return 0x05; // Fallback
}

static bool nh_ecmascript_pairsHasKey(
    nh_core_List *Pairs_p,
    const char *key)
{
    if (!Pairs_p || !key) return false;

    for (int i = 0; i < Pairs_p->size; i++) {
        // Cast the list element to your KeyValuePair type
        nh_ecmascript_KeyValuePair *pair = (nh_ecmascript_KeyValuePair *)Pairs_p->pp[i];
        
        if (pair && pair->key && strcmp(pair->key, key) == 0) {
            return true;
        }
    }

    return false;
}

static nh_ecmascript_Value nh_ecmascript_resolve(
    char *parentName_p,
    nh_ecmascript_KeyValuePair *Pair_p,
    nh_ecmascript_Realm *Realm_p
);

static void nh_ecmascript_applyTemplatePairs(
    nh_ecmascript_Object *O_p, 
    nh_core_List *Pairs_p, 
    nh_ecmascript_Realm *Realm_p) 
{
    if (!Pairs_p || !O_p) return;

    for (int i = 0; i < Pairs_p->size; i++) {
        nh_ecmascript_KeyValuePair *pair = (nh_ecmascript_KeyValuePair *)Pairs_p->pp[i];

        // 1. Skip "attributes" 
        // These are metadata for the PARENT object's property entry, 
        // not a property that belongs on the current object.
        if (strcmp(pair->key, "attributes") == 0) continue;

        // 2. Resolve the value of this pair
        // This handles recursion (e.g., if a function has a 'prototype' object)
        nh_ecmascript_Value value = nh_ecmascript_resolveInternalSlot(pair->value, Realm_p);

        // 3. Determine attributes for this sub-property
        // Most internal properties (length, name) are !enumerable in JS
        uint8_t attrBits = nh_ecmascript_getAttributesFromNode(pair->value);

        nh_ecmascript_PropertyDescriptor Descriptor = {
            .Value = value,
            .flags.hasWritable = true, 
            .flags.writable = (attrBits & 0x01) != 0,
            .flags.hasEnumerable = true, 
            .flags.enumerable = (attrBits & 0x02) != 0,
            .flags.hasConfigurable = true, 
            .flags.configurable = (attrBits & 0x04) != 0
        };

        // 4. Define the property
        // This evolves O_p->Shape_p and populates O_p->Properties
        O_p->Methods_p->defineOwnProperty(O_p, pair->key, &Descriptor);
    }
}

static nh_ecmascript_Value nh_ecmascript_resolve(
    char *parentName_p,
    nh_ecmascript_KeyValuePair *Pair_p,
    nh_ecmascript_Realm *Realm_p)
{
    // 1. Is this a function? In the spec, if it has 'length', it's a function.
    bool isFunction = nh_ecmascript_pairsHasKey(&Pair_p->value->data.Pairs, "length");

    if (isFunction) {
        void *native = nh_ecmascript_getNativeSteps(parentName_p, Pair_p->key);
        nh_ecmascript_Object *F = nh_ecmascript_createBuiltinFunction(native, Realm_p, NULL);
        nh_ecmascript_applyTemplatePairs(F, &Pair_p->value->data.Pairs, Realm_p);
        return nh_ecmascript_makeObject(F);
    }

    // Otherwise, it's just a plain object literal
    nh_ecmascript_Object *O_p = nh_ecmascript_ordinaryObjectCreate(NULL, Realm_p);
    nh_ecmascript_applyTemplatePairs(O_p, &Pair_p->value->data.Pairs, Realm_p);

    return nh_ecmascript_makeObject(O_p);
}

nh_ecmascript_Value nh_ecmascript_resolveProperty(
    char *parentName_p,
    nh_ecmascript_KeyValuePair *Pair_p,
    nh_ecmascript_Realm *Realm_p)
{
    if (Pair_p->value->kind != NH_ECMASCRIPT_TEMPLATE_NODE_PAIRS) {return nh_ecmascript_makeUndefined();}
    return nh_ecmascript_resolve(parentName_p, Pair_p, Realm_p);
}

nh_ecmascript_Value nh_ecmascript_resolveInternalSlot(
    nh_ecmascript_TemplateNode *Node_p,
    nh_ecmascript_Realm *Realm_p)
{
    switch (Node_p->kind) {
        case NH_ECMASCRIPT_TEMPLATE_NODE_NUMBER:
            return nh_ecmascript_makeNumber(Node_p->data.number);
            
        case NH_ECMASCRIPT_TEMPLATE_NODE_STRING:
            return nh_ecmascript_makeString(Node_p->data.string_p);
            
        case NH_ECMASCRIPT_TEMPLATE_NODE_BOOLEAN:
            return nh_ecmascript_makeBool(Node_p->data.boolean);
            
        case NH_ECMASCRIPT_TEMPLATE_NODE_NULL:
            return nh_ecmascript_makeNull();

        default:
            return nh_ecmascript_makeUndefined();
    }
}
