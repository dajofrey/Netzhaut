// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "ParameterLists.h"

#include "../Engine/Type.h"
#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <string.h>

// HAS INITIALIZER =================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-hasinitializer
static NH_BOOL nh_ecmascript_hasInitializer(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER :
            NH_ECMASCRIPT_END(nh_ecmascript_hasInitializer(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT :
            if (Node_p->Children.size == 1) {
                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN) {
                    NH_ECMASCRIPT_END(NH_FALSE)
                }
            }
            else {NH_ECMASCRIPT_END(NH_TRUE)}
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SINGLE_NAME_BINDING :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(NH_FALSE)
            }
            else {NH_ECMASCRIPT_END(NH_TRUE)}
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
        {
            NH_BOOL hasInitializer = nh_ecmascript_hasInitializer(Node_p->Children.pp[0]);
            if (!hasInitializer && Node_p->Children.size == 2) {
                hasInitializer = nh_ecmascript_hasInitializer(Node_p->Children.pp[1]);
            }
            NH_ECMASCRIPT_END(hasInitializer)
        }
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

// PARAMETER LISTS =================================================================================

int nh_ecmascript_getExpectedArgumentCount(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETERS :
            if (Node_p->Children.size == 2) {
                NH_ECMASCRIPT_END(nh_ecmascript_getExpectedArgumentCount(Node_p->Children.pp[0])) 
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
            if (Node_p->Children.size == 1) {
                if (nh_ecmascript_hasInitializer(Node_p->Children.pp[0])) {NH_ECMASCRIPT_END(0)}
                NH_ECMASCRIPT_END(1)
            }
            else {
                int count = nh_ecmascript_getExpectedArgumentCount(Node_p->Children.pp[0]);
                if (nh_ecmascript_hasInitializer(Node_p->Children.pp[0]) || nh_ecmascript_hasInitializer(Node_p->Children.pp[1])) {
                    NH_ECMASCRIPT_END(count)
                }
                NH_ECMASCRIPT_END(count + 1)
            }

// ArrowParameters : BindingIdentifier
//
//    Return 1.
//
//ArrowParameters : CoverParenthesizedExpressionAndArrowParameterList
//
//    Let formals be CoveredFormalsList of CoverParenthesizedExpressionAndArrowParameterList.
//    Return ExpectedArgumentCount of formals.
//
//PropertySetParameterList : FormalParameter
//
//    If HasInitializer of FormalParameter is true, return 0.
//    Return 1.
//
//AsyncArrowBindingIdentifier : BindingIdentifier
//
//    Return 1.
    }

NH_ECMASCRIPT_END(0)
}

