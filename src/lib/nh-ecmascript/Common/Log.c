// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"

#include "../Parser/Lexer.h"
#include "../Parser/AST.h"
#include "../../nh-core/Util/Debug.h"

#include <stdio.h>

// DEFINE ==========================================================================================

#define NH_ECMASCRIPT_LOG_PREFIX "nh-ecmascript"
#define NH_ECMASCRIPT_LOG_CATEGORY_LOGGING "Logging"
#define NH_ECMASCRIPT_LOG_CATEGORY_DECODER "Decoder"

const char *NH_ECMASCRIPT_AST_NODE_NAMES_PP[] = {
    "Program",
    "ExpressionStatement",
    "CallExpression",
    "MemberExpression",
    "Identifier",
    "Literal",
    "BinaryExpression",
    "AssignmentExpression",
    "VariableDeclaration",
    "BlockStatement",
    "IfStatement",
    "ForStatement",
    "FunctionDeclaration",
    "UpdateExpression",
    "VariableDeclarator",
    "ForDeclaration",
};

//NH_API_RESULT nh_ecmascript_finishLogging(
//    nh_ecmascript_Script *Script_p, nh_core_Array *UnicodeCodePoints_p)
//{
//    char message_p[127] = {'\0'};
//    sprintf(message_p, "[%s:%d:%s](FINISH)", NH_ECMASCRIPT_LOG_PREFIX, NH_ECMASCRIPT_LOG_CATEGORY_LOGGING);
//    nh_core_log(message_p);
//
//return NH_API_SUCCESS;
//}

NH_API_RESULT nh_ecmascript_logDecoder(
    void *handle_p, nh_encoding_UTF32String *Codepoints_p)
{
//    for (int i = 0; i < Codepoints_p->length; ++i) 
//    {
//        char message_p[512] = {'\0'};
//        sprintf(
//            message_p, "[nh-ecmascript:%p:Decoder]{%s}", handle_p,
//            nh_encoding_getDescription(Codepoints_p->p[i])
//        );
//        nh_core_log(message_p);
//    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_ecmascript_logLexer(
    void *handle_p, nh_core_Array *InputElements_p, bool dirty)
{
    for (int i = 0; i < InputElements_p->length; ++i) 
    {
        nh_ecmascript_InputElement *InputElement_p = &((nh_ecmascript_InputElement*)InputElements_p->p)[i];
        char message_p[512] = {'\0'};

        nh_core_String String = nh_core_initString(64);
        nh_core_appendToString(&String, InputElement_p->String.p, InputElement_p->String.length);

        for (int i = 0; i < String.length; ++i) {
            if (String.p[i] == '\n' || String.p[i] == '\r') {String.p[i] = ' ';}
        }

        sprintf(
            message_p, "%s [%s]", String.p, NH_ECMASCRIPT_INPUT_ELEMENTS_PP[InputElement_p->type]
        );

        char addr_p[255];
        sprintf(addr_p, dirty ? "nh-ecmascript:%p:LexerDirty" : "nh-ecmascript:%p:LexerClean", handle_p);
        nh_core_log(addr_p, NULL, message_p);
        nh_core_freeString(&String);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_ecmascript_logParseTree(
    void *handle_p, nh_ecmascript_ParseNode *ParseNode_p, nh_ecmascript_ParseNode *Parent_p, int depth, bool *branch_p)
{
    char message_p[1200] = {'\0'};
    char indent_p[1024] = {'\0'};

    int offset;
    for (offset = 0; offset < depth; ++offset) {
        indent_p[offset] = branch_p[offset] == true ? '|' : ' ';
    }

    if (ParseNode_p->Value_p == NULL) {
        sprintf(message_p, "%s%s", indent_p, NH_ECMASCRIPT_PARSE_NODE_NAMES_PP[ParseNode_p->type]);
    }
    else {
        sprintf(message_p, "%sTERMINAL %s", indent_p, ParseNode_p->Value_p->String.p);
    }

    char addr_p[255];
    sprintf(addr_p, "nh-ecmascript:%p:ParseTree", handle_p);
    nh_core_log(addr_p, NULL, message_p);

    branch_p[depth] = true;
    if (Parent_p != NULL &&  ParseNode_p == Parent_p->Children.pp[Parent_p->Children.size - 1]) {
        branch_p[depth - 1] = false;
    }

    for (int i = 0; i < ParseNode_p->Children.size; ++i) {
        nh_ecmascript_logParseTree(handle_p, ParseNode_p->Children.pp[i], ParseNode_p, depth + 1, branch_p);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_ecmascript_logAST(
    void *handle_p, nh_ecmascript_ASTNode *ASTNode_p, nh_ecmascript_ASTNode *Parent_p, int depth, bool *branch_p)
{
    if (ASTNode_p == NULL) {
        return NH_API_SUCCESS;
    }

    char message_p[1200] = {'\0'};
    char indent_p[1024] = {'\0'};

    int offset;
    for (offset = 0; offset < depth; ++offset) {
        indent_p[offset] = branch_p[offset] == true ? '|' : ' ';
    }

    // Build node description based on type
    char node_desc_p[512] = {'\0'};
    
    switch (ASTNode_p->type) {
        case NH_ECMASCRIPT_AST_NODE_IDENTIFIER:
            sprintf(node_desc_p, "Identifier(%s)", ASTNode_p->Identifier.name_p);
            break;
            
        case NH_ECMASCRIPT_AST_NODE_LITERAL:
            switch (ASTNode_p->Literal.type) {
                case NH_ECMASCRIPT_AST_LITERAL_NUMBER:
                    sprintf(node_desc_p, "Literal(%g)", ASTNode_p->Literal.number);
                    break;
                case NH_ECMASCRIPT_AST_LITERAL_STRING:
                    sprintf(node_desc_p, "Literal(\"%s\")", ASTNode_p->Literal.string_p);
                    break;
                case NH_ECMASCRIPT_AST_LITERAL_BOOL:
                    sprintf(node_desc_p, "Literal(%s)", ASTNode_p->Literal.boolean ? "true" : "false");
                    break;
                case NH_ECMASCRIPT_AST_LITERAL_NULL:
                    sprintf(node_desc_p, "Literal(null)");
                    break;
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_BINARY_EXPRESSION:
            sprintf(node_desc_p, "BinaryExpression(%s)", ASTNode_p->BinaryExpression.operator_p);
            break;
            
        case NH_ECMASCRIPT_AST_NODE_ASSIGNMENT_EXPRESSION:
            sprintf(node_desc_p, "AssignmentExpression(%s)", ASTNode_p->AssignmentExpression.operator_p);
            break;
            
        case NH_ECMASCRIPT_AST_NODE_UPDATE_EXPRESSION:
            sprintf(node_desc_p, "UpdateExpression(%s%s)", 
                ASTNode_p->UpdateExpression.prefix ? ASTNode_p->UpdateExpression.operator_p : "",
                ASTNode_p->UpdateExpression.Argument_p ? ASTNode_p->UpdateExpression.Argument_p->Identifier.name_p : "",
                ASTNode_p->UpdateExpression.prefix ? "" : ASTNode_p->UpdateExpression.operator_p);
            break;
            
        case NH_ECMASCRIPT_AST_NODE_MEMBER_EXPRESSION:
            sprintf(node_desc_p, "MemberExpression(%s)", ASTNode_p->MemberExpression.computed ? "computed" : "dot");
            break;
            
        case NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATION:
            sprintf(node_desc_p, "VariableDeclaration(%s)", ASTNode_p->VariableDeclaration.kind_p);
            break;
            
        case NH_ECMASCRIPT_AST_NODE_FOR_DECLARATION:
            sprintf(node_desc_p, "ForDeclaration(%s)", ASTNode_p->ForDeclaration.kind_p);
            break;
            
        case NH_ECMASCRIPT_AST_NODE_CALL_EXPRESSION:
            sprintf(node_desc_p, "CallExpression");
            break;
            
        case NH_ECMASCRIPT_AST_NODE_PROGRAM:
            sprintf(node_desc_p, "Program");
            break;
            
        case NH_ECMASCRIPT_AST_NODE_EXPRESSION_STATEMENT:
            sprintf(node_desc_p, "ExpressionStatement");
            break;
            
        case NH_ECMASCRIPT_AST_NODE_BLOCK_STATEMENT:
            sprintf(node_desc_p, "BlockStatement");
            break;
            
        case NH_ECMASCRIPT_AST_NODE_FOR_STATEMENT:
            sprintf(node_desc_p, "ForStatement");
            break;
            
        case NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATOR:
            sprintf(node_desc_p, "VariableDeclarator");
            break;
            
        default:
            sprintf(node_desc_p, "%s", NH_ECMASCRIPT_AST_NODE_NAMES_PP[ASTNode_p->type]);
            break;
    }

    sprintf(message_p, "%s%s", indent_p, node_desc_p);

    char addr_p[255];
    sprintf(addr_p, "nh-ecmascript:%p:AST", handle_p);
    nh_core_log(addr_p, NULL, message_p);

    // Set branch indicators for tree structure
    branch_p[depth] = true;

    // Recursively log children
    switch (ASTNode_p->type) {
        case NH_ECMASCRIPT_AST_NODE_PROGRAM:
            for (size_t i = 0; i < ASTNode_p->Program.Body.size; ++i) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->Program.Body.pp[i], ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_EXPRESSION_STATEMENT:
            if (ASTNode_p->ExpressionStatement.Expression_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->ExpressionStatement.Expression_p, ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_CALL_EXPRESSION:
            if (ASTNode_p->CallExpression.Caller_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->CallExpression.Caller_p, ASTNode_p, depth + 1, branch_p);
            }
            for (size_t i = 0; i < ASTNode_p->CallExpression.Arguments.size; ++i) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->CallExpression.Arguments.pp[i], ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_MEMBER_EXPRESSION:
            if (ASTNode_p->MemberExpression.Object_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->MemberExpression.Object_p, ASTNode_p, depth + 1, branch_p);
            }
            if (ASTNode_p->MemberExpression.Property_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->MemberExpression.Property_p, ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_BINARY_EXPRESSION:
            if (ASTNode_p->BinaryExpression.Left_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->BinaryExpression.Left_p, ASTNode_p, depth + 1, branch_p);
            }
            if (ASTNode_p->BinaryExpression.Right_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->BinaryExpression.Right_p, ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_ASSIGNMENT_EXPRESSION:
            if (ASTNode_p->AssignmentExpression.Left_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->AssignmentExpression.Left_p, ASTNode_p, depth + 1, branch_p);
            }
            if (ASTNode_p->AssignmentExpression.Right_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->AssignmentExpression.Right_p, ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_UPDATE_EXPRESSION:
            if (ASTNode_p->UpdateExpression.Argument_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->UpdateExpression.Argument_p, ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATION:
            for (size_t i = 0; i < ASTNode_p->VariableDeclaration.Declarators.size; ++i) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->VariableDeclaration.Declarators.pp[i], ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_FOR_DECLARATION:
            for (size_t i = 0; i < ASTNode_p->ForDeclaration.Declarators.size; ++i) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->ForDeclaration.Declarators.pp[i], ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_BLOCK_STATEMENT:
            for (size_t i = 0; i < ASTNode_p->BlockStatement.Body.size; ++i) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->BlockStatement.Body.pp[i], ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_FOR_STATEMENT:
            if (ASTNode_p->ForStatement.Init_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->ForStatement.Init_p, ASTNode_p, depth + 1, branch_p);
            }
            if (ASTNode_p->ForStatement.Test_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->ForStatement.Test_p, ASTNode_p, depth + 1, branch_p);
            }
            if (ASTNode_p->ForStatement.Update_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->ForStatement.Update_p, ASTNode_p, depth + 1, branch_p);
            }
            if (ASTNode_p->ForStatement.Body_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->ForStatement.Body_p, ASTNode_p, depth + 1, branch_p);
            }
            break;
            
        case NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATOR:
            if (ASTNode_p->VariableDeclarator.Id_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->VariableDeclarator.Id_p, ASTNode_p, depth + 1, branch_p);
            }
            if (ASTNode_p->VariableDeclarator.Init_p) {
                nh_ecmascript_logAST(handle_p, ASTNode_p->VariableDeclarator.Init_p, ASTNode_p, depth + 1, branch_p);
            }
            break;
    }

    return NH_API_SUCCESS;
}
