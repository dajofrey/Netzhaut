#ifndef NH_ECMASCRIPT_PARSER_AST_H
#define NH_ECMASCRIPT_PARSER_AST_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "CST.h"
#include "../Common/Includes.h"

typedef enum NH_ECMASCRIPT_AST_NODE {
    NH_ECMASCRIPT_AST_NODE_PROGRAM,
    NH_ECMASCRIPT_AST_NODE_EXPRESSION_STATEMENT,
    NH_ECMASCRIPT_AST_NODE_CALL_EXPRESSION,
    NH_ECMASCRIPT_AST_NODE_MEMBER_EXPRESSION,
    NH_ECMASCRIPT_AST_NODE_IDENTIFIER,
    NH_ECMASCRIPT_AST_NODE_LITERAL,
    NH_ECMASCRIPT_AST_NODE_BINARY_EXPRESSION,
    NH_ECMASCRIPT_AST_NODE_ASSIGNMENT_EXPRESSION,
    NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATION,
    NH_ECMASCRIPT_AST_NODE_BLOCK_STATEMENT,
    NH_ECMASCRIPT_AST_NODE_IF_STATEMENT,
    NH_ECMASCRIPT_AST_NODE_FOR_STATEMENT,
    NH_ECMASCRIPT_AST_NODE_FUNCTION_DECLARATION,
    NH_ECMASCRIPT_AST_NODE_UPDATE_EXPRESSION,
    NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATOR,
    NH_ECMASCRIPT_AST_NODE_FOR_DECLARATION,
} NH_ECMASCRIPT_AST_NODE;

typedef enum NH_ECMASCRIPT_AST_LITERAL {
    NH_ECMASCRIPT_AST_LITERAL_NUMBER,
    NH_ECMASCRIPT_AST_LITERAL_STRING,
    NH_ECMASCRIPT_AST_LITERAL_BOOL,
    NH_ECMASCRIPT_AST_LITERAL_NULL
} NH_ECMASCRIPT_AST_LITERAL;

typedef struct nh_ecmascript_ASTNode {
    NH_ECMASCRIPT_AST_NODE type;

    union {
        struct {
            nh_core_List Body;
        } Program;

        struct {
            struct nh_ecmascript_ASTNode *Expression_p;
        } ExpressionStatement;

        struct {
            struct nh_ecmascript_ASTNode *Caller_p;
            nh_core_List Arguments;
        } CallExpression;

        struct {
            struct nh_ecmascript_ASTNode *Object_p;
            struct nh_ecmascript_ASTNode *Property_p;
            bool computed; // true for obj[prop], false for obj.prop
        } MemberExpression;

        struct {
            char *name_p;
        } Identifier;

        struct {
            NH_ECMASCRIPT_AST_LITERAL type;
            union {
                double number;
                char *string_p;
                bool boolean;
            };
        } Literal;

        struct {
            struct nh_ecmascript_ASTNode *Left_p;
            struct nh_ecmascript_ASTNode *Right_p;
            char *operator_p; // +, -, *, /, <, >, etc.
        } BinaryExpression;

        struct {
            struct nh_ecmascript_ASTNode *Left_p;
            struct nh_ecmascript_ASTNode *Right_p;
            char *operator_p; // =, +=, -=, etc.
        } AssignmentExpression;

        struct {
            struct nh_ecmascript_ASTNode *Argument_p;
            char *operator_p; // ++, --
            bool prefix; // true for ++i, false for i++
        } UpdateExpression;

        struct {
            char *kind_p; // var, let, const
            nh_core_List Declarators; // of VariableDeclarator*
        } VariableDeclaration;

        struct {
            struct nh_ecmascript_ASTNode *Id_p;
            struct nh_ecmascript_ASTNode *Init_p; // may be NULL
        } VariableDeclarator;

        struct {
            char *kind_p; // var, let, const
            nh_core_List Declarators; // of VariableDeclarator*
        } ForDeclaration;

        struct {
            nh_core_List Body;
        } BlockStatement;

        struct {
            struct nh_ecmascript_ASTNode *Init_p; // may be NULL
            struct nh_ecmascript_ASTNode *Test_p; // may be NULL
            struct nh_ecmascript_ASTNode *Update_p; // may be NULL
            struct nh_ecmascript_ASTNode *Body_p;
        } ForStatement;
    };

} nh_ecmascript_ASTNode;

nh_ecmascript_ASTNode *nh_ecmascript_parseAST(
    nh_ecmascript_ParseNode *Node_p
);

#endif 
