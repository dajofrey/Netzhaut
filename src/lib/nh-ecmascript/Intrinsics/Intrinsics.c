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

#include <string.h>

// FUNCTIONS =======================================================================================

nh_core_List nh_ecmascript_parseIntrinsicTemplates()
{
    nh_core_List Templates = nh_core_initList(NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP_COUNT);

    for (int i = 0; i < NH_ECMASCRIPT_INTRINSIC_TEMPLATE_NAMES_PP_COUNT; ++i) {
        nh_core_Array Tokens = nh_ecmascript_tokenizeIntrinsicTemplate(NULL, NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP[i]);
        nh_ecmascript_IntrinsicTemplate *Template_p = nh_ecmascript_parseIntrinsicTemplate(Tokens.p, Tokens.length);
        nh_core_freeArray(&Tokens);
        nh_core_appendToList(&Templates, Template_p);
    }

    return Templates;
}   

nh_ecmascript_TemplateNode* nh_ecmascript_lookupTemplateValue(
    nh_core_List *pairs,
    const char *key)
{
    for (int i = 0; i < pairs->size; i++) {
        nh_ecmascript_KeyValuePair *pair = nh_core_getFromList(pairs, i);
        if (strcmp(pair->key, key) == 0) {
            return pair->value;
        }
    }
    return NULL;
}

bool nh_ecmascript_isTemplateCallable(
    nh_ecmascript_IntrinsicTemplate *T)
{
    return nh_ecmascript_lookupTemplateValue(&T->InternalSlots, "[[Call]]") != NULL;
}

bool nh_ecmascript_isTemplateArray(
    nh_ecmascript_IntrinsicTemplate *T)
{
    return nh_ecmascript_lookupTemplateValue(&T->InternalSlots, "[[ArrayLength]]") != NULL;
}

nh_ecmascript_Value nh_ecmascript_resolveToValue(
    nh_ecmascript_TemplateNode *node,
    nh_ecmascript_Realm *realm)
{
    switch (node->kind) {
        case NH_ECMASCRIPT_TEMPLATE_NODE_NUMBER:
            return nh_ecmascript_makeNumber(node->data.number);
            
        case NH_ECMASCRIPT_TEMPLATE_NODE_STRING:
            return nh_ecmascript_makeString(node->data.string_p);
            
        case NH_ECMASCRIPT_TEMPLATE_NODE_BOOLEAN:
            return nh_ecmascript_makeBoolean(node->data.boolean);
            
        case NH_ECMASCRIPT_TEMPLATE_NODE_NULL:
            return nh_ecmascript_makeNull();
            
        case NH_ECMASCRIPT_TEMPLATE_NODE_INTRINSIC:
            // Crucial: Pass 3 of bootstrapping. Resolves "%ObjectPrototype%" to a live pointer.
//            return nh_realm_get_intrinsic(realm, node->data.string_p);
            
        case NH_ECMASCRIPT_TEMPLATE_NODE_IDENTIFIER:
            // Used for resolving C function symbols via dlsym
//            void* sym = nh_sym_resolve(node->data.string_p);
//            return nh_ecmascript_makeNativeFunction(sym);

        default:
            return nh_ecmascript_makeUndefined();
    }
}

//uint8_t nh_ecmascritp_resolveAttributes(
//    nh_ecmascript_TemplateNode *node)
//{
//    uint8_t attrs = 0; // Default: non-writable, non-enumerable, non-configurable
//    if (node->kind != NH_ECMASCRIPT_TEMPLATE_NODE_NODES) return attrs;
//
//    for (int i = 0; i < node->data.Nodes.size; i++) {
//        nh_ecmascript_TemplateNode *item = nh_core_list_get(&node->data.Nodes, i);
//        if (item->kind == NH_ECMASCRIPT_TEMPLATE_NODE_IDENTIFIER) {
//            if (strcmp(item->data.string_p, "writable") == 0)     attrs |= NH_ATTR_WRITABLE;
//            if (strcmp(item->data.string_p, "enumerable") == 0)   attrs |= NH_ATTR_ENUMERABLE;
//            if (strcmp(item->data.string_p, "configurable") == 0) attrs |= NH_ATTR_CONFIGURABLE;
//        }
//    }
//    return attrs;
//}
//
//NH_PROP_TYPE_E nh_compute_property_type(
//    nh_ecmascript_TemplateNode *node)
//{
//    if (node->kind != NH_ECMASCRIPT_TEMPLATE_NODE_PAIRS) {
//        // If it's just a raw value like "length": 1, it's a Data Property
//        return NH_PROP_TYPE_DATA;
//    }
//
//    bool has_value = nh_lookup_template_value(&node->data.Pairs, "value") != NULL;
//    bool has_writable = nh_lookup_template_value(&node->data.Pairs, "writable") != NULL;
//    bool has_get = nh_lookup_template_value(&node->data.Pairs, "get") != NULL;
//    bool has_set = nh_lookup_template_value(&node->data.Pairs, "set") != NULL;
//
//    if (has_get || has_set) {
//        // Spec violation check: Accessors cannot have 'value' or 'writable'
//        if (has_value || has_writable) {
//            // You might want to log a warning or abort here
//        }
//        return NH_PROP_TYPE_ACCESSOR;
//    }
//
//    return NH_PROP_TYPE_DATA;
//}
