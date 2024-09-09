// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Misc.h"

#include "../Engine/Type.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// HAS NAME ========================================================================================

static NH_BOOL nh_ecmascript_hasName(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

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
//                NH_ECMASCRIPT_END(NH_TRUE)
//            }
//            break;
//    }

NH_ECMASCRIPT_END(NH_FALSE)
}

// IS FUNCTION DEFINITION ==========================================================================

static NH_BOOL nh_ecmascript_isFunctionDefinition(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_PRIMARY_EXPRESSION :
//            // TODO
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_EXPRESSION :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ARROW_FUNCTION
//            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_ARROW_FUNCTION) {
//                NH_ECMASCRIPT_END(NH_TRUE)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_EXPRESSION :
//        case NH_ECMASCRIPT_PARSE_NODE_CLASS_EXPRESSION :
//            NH_ECMASCRIPT_END(NH_TRUE)
//    }

NH_ECMASCRIPT_END(NH_FALSE)
}

// IS ANONYMOUS FUNCTION DEFINITION ================================================================

NH_BOOL nh_ecmascript_isAnonymousFunctionDefinition(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    if (!nh_ecmascript_isFunctionDefinition(Node_p)) {NH_ECMASCRIPT_END(NH_FALSE)}
    NH_BOOL hasName = nh_ecmascript_hasName(Node_p);
    if (hasName) {NH_ECMASCRIPT_END(NH_FALSE)}

NH_ECMASCRIPT_END(NH_TRUE)
}

// GET STRING VALUE ================================================================================

nh_encoding_UTF8String *nh_ecmascript_getStringValue(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER_REFERENCE :
//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER :
//        case NH_ECMASCRIPT_PARSE_NODE_LABEL_IDENTIFIER :
//            if (Node_p->Value_p != NULL) {NH_ECMASCRIPT_END(&Node_p->Value_p->String)}
//            NH_ECMASCRIPT_END(nh_ecmascript_getStringValue(Node_p->Children.pp[0]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER :
//            NH_ECMASCRIPT_END(&Node_p->Value_p->String)
//    }

NH_ECMASCRIPT_END(NULL)
}

// IS CONSTANT DECLARATION =========================================================================

NH_BOOL nh_ecmascript_isConstantDeclaration(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_DECLARATION :
//            NH_ECMASCRIPT_END(nh_ecmascript_isConstantDeclaration(Node_p->Children.pp[0]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_LET_OR_CONST :
//            if (!strcmp(Node_p->Value_p->String.p, "let")) {
//                NH_ECMASCRIPT_END(NH_FALSE)
//            }
//            else if (!strcmp(Node_p->Value_p->String.p, "const")) {
//                NH_ECMASCRIPT_END(NH_TRUE)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_GENERATOR_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_GENERATOR_DECLARATION :
//        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_DECLARATION :
//            NH_ECMASCRIPT_END(NH_FALSE)
//
//        case NH_ECMASCRIPT_PARSE_NODE_CLASS_DECLARATION :
//            NH_ECMASCRIPT_END(NH_FALSE)
//
//        case NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION :
//            NH_ECMASCRIPT_END(NH_FALSE)
//    }

NH_ECMASCRIPT_END(NH_FALSE)
}

