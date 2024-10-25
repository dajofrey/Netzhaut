// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "ParameterLists.h"

#include "../Engine/Type.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// FUNCTIONS =======================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-hasinitializer
static bool nh_ecmascript_hasInitializer(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER :
            return nh_ecmascript_hasInitializer(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT :
            if (Node_p->Children.size == 1) {
                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN) {
                    return false;
                }
            }
            else {return true;}
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SINGLE_NAME_BINDING :
            if (Node_p->Children.size == 1) {
                return false;
            }
            else {return true;}
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
        {
            bool hasInitializer = nh_ecmascript_hasInitializer(Node_p->Children.pp[0]);
            if (!hasInitializer && Node_p->Children.size == 2) {
                hasInitializer = nh_ecmascript_hasInitializer(Node_p->Children.pp[1]);
            }
            return hasInitializer;
        }
    }

    return false;
}

int nh_ecmascript_getExpectedArgumentCount(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETERS :
            if (Node_p->Children.size == 2) {
                return nh_ecmascript_getExpectedArgumentCount(Node_p->Children.pp[0]); 
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
            if (Node_p->Children.size == 1) {
                if (nh_ecmascript_hasInitializer(Node_p->Children.pp[0])) {return 0;}
                return 1;
            }
            else {
                int count = nh_ecmascript_getExpectedArgumentCount(Node_p->Children.pp[0]);
                if (nh_ecmascript_hasInitializer(Node_p->Children.pp[0]) || nh_ecmascript_hasInitializer(Node_p->Children.pp[1])) {
                    return count;
                }
                return count + 1;
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

    return 0;
}

