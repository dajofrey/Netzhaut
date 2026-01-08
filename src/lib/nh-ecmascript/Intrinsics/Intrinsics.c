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

#include "../Common/Log.h"

#include <string.h>
#include <dlfcn.h>

// FUNCTIONS =======================================================================================

nh_core_List nh_ecmascript_parseIntrinsicTemplates()
{
    nh_core_List Templates = nh_core_initList(NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP_COUNT);

    for (int i = 0; i < NH_ECMASCRIPT_INTRINSIC_TEMPLATE_NAMES_PP_COUNT; ++i) {
        nh_core_Array Tokens = nh_ecmascript_tokenizeIntrinsicTemplate(NULL, NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP[i]);
        nh_ecmascript_IntrinsicTemplate *Template_p = nh_ecmascript_parseIntrinsicTemplate(Tokens.p, Tokens.length);
        nh_ecmascript_logIntrinsicTemplate(Template_p);
        nh_core_freeArray(&Tokens);
        nh_core_appendToList(&Templates, Template_p);
    }

    return Templates;
}   

void *nh_ecmascript_getNativeSteps(
    char *name_p)
{
    void *p = dlsym(RTLD_DEFAULT, name_p);
    return p;
}

nh_ecmascript_Value nh_ecmascript_resolveTemplateValue(
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
