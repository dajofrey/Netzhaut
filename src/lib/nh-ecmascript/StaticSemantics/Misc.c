// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Misc.h"

#include "../Engine/Value.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// HAS NAME ========================================================================================

static bool nh_ecmascript_hasName(
    nh_ecmascript_ParseNode *Node_p)
{
//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_PRIMARY_EXPRESSION :
//            // TODO
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_CLASS_EXPRESSION :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {
//                return true;
//            }
//            break;
//    }

    return false;
}

// IS FUNCTION DEFINITION ==========================================================================

static bool nh_ecmascript_isFunctionDefinition(
    nh_ecmascript_ParseNode *Node_p)
{
//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_PRIMARY_EXPRESSION :
//            // TODO
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_EXPRESSION :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ARROW_FUNCTION
//            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_ARROW_FUNCTION) {
//                return true;
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_CLASS_EXPRESSION :
//            return true;
//    }

    return false;
}

// IS ANONYMOUS FUNCTION DEFINITION ================================================================

bool nh_ecmascript_isAnonymousFunctionDefinition(
    nh_ecmascript_ParseNode *Node_p)
{
    if (!nh_ecmascript_isFunctionDefinition(Node_p)) {return false;}
    bool hasName = nh_ecmascript_hasName(Node_p);
    if (hasName) {return false;}

    return true;
}

// GET STRING VALUE ================================================================================

nh_encoding_UTF8String *nh_ecmascript_getStringValue(
    nh_ecmascript_ParseNode *Node_p)
{
//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER_REFERENCE :
//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER :
//        case NH_ECMASCRIPT_PARSE_NODE_LABEL_IDENTIFIER :
//            if (Node_p->Value_p != NULL) {return &Node_p->Value_p->String;}
//            return nh_ecmascript_getStringValue(Node_p->Children.pp[0]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER :
//            return &Node_p->Value_p->String;
//    }

    return NULL;
}

// IS CONSTANT DECLARATION =========================================================================

bool nh_ecmascript_isConstantDeclaration(
    nh_ecmascript_ParseNode *Node_p)
{
//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_DECLARATION :
//            return nh_ecmascript_isConstantDeclaration(Node_p->Children.pp[0]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_LET_OR_CONST :
//            if (!strcmp(Node_p->Value_p->String.p, "let")) {
//                return false;
//            }
//            else if (!strcmp(Node_p->Value_p->String.p, "const")) {
//                return true;
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_DECLARATION :
//            return false;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CLASS_DECLARATION :
//            return false;
//
//        case NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION :
//            return false;
//    }

    return false;
}

