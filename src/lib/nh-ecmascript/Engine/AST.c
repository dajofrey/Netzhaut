// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "AST.h"
#include "Lexer.h"

#include "../Common/IndexMap.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

// DATA ============================================================================================

nh_ecmascript_ParseResult nh_ecmascript_parseAST(
    nh_ecmascript_CST *CST_p)
{
    nh_ecmascript_Parser Parser;
    Parser.Tokens_p = (nh_ecmascript_InputElement*)InputElements.p;
    Parser.unparsed = InputElements.length;

    nh_ecmascript_ParseResult Result = nh_ecmascript_initParseResult(NULL);

    switch (goalSymbol)
    {
        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT :
            Result = nh_ecmascript_parseScriptNode(&Parser);
            break;
    }

    return Result;
}




static nh_ecmascript_ASTNode *nh_ecmascript_toAST(
    nh_ecmascript_ParseNode *CST) 
{
    if (!CST) return NULL;

    switch (CST->type) {

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT:
        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY:
        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST: {
            nh_ecmascript_ASTNode *prog = ast_alloc(NH_AST_PROGRAM);
            for (size_t i = 0; i < CST->Children.count; i++) {
                nh_ecmascript_ParseNode *child = CST->Children.items[i];
                nh_ecmascript_ASTNode *stmt = cstToAst(child);
                if (stmt) nh_core_appendToList(&prog->children, stmt);
            }
            return prog;
        }

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT: {
            nh_ecmascript_ASTNode *decl = ast_alloc(NH_AST_VARIABLE_DECLARATION);
            for (size_t i = 0; i < CST->Children.count; i++) {
                nh_ecmascript_ParseNode *child = CST->Children.items[i];
                nh_ecmascript_ASTNode *d = cstToAst(child);
                if (d) nh_core_appendToList(&decl->children, d);
            }
            return decl;
        }

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION: {
            nh_ecmascript_ASTNode *d = ast_alloc(NH_AST_VARIABLE_DECLARATOR);
            // children are Identifier + optional Initializer
            for (size_t i = 0; i < CST->Children.count; i++) {
                nh_ecmascript_ParseNode *child = CST->Children.items[i];
                nh_ecmascript_ASTNode *c = cstToAst(child);
                if (c) nh_core_appendToList(&d->children, c);
            }
            return d;
        }

        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER:
        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER_REFERENCE: {
            nh_ecmascript_ASTNode *id = ast_alloc(NH_AST_IDENTIFIER);
            id->name = strdup(CST->Value_p ? CST->Value_p->String.p : "");
            return id;
        }

        case NH_ECMASCRIPT_PARSE_NODE_NUMERIC_LITERAL:
        case NH_ECMASCRIPT_PARSE_NODE_STRING_LITERAL:
        case NH_ECMASCRIPT_PARSE_NODE_BOOLEAN_LITERAL:
        case NH_ECMASCRIPT_PARSE_NODE_NULL_LITERAL: {
            nh_ecmascript_ASTNode *lit = ast_alloc(NH_AST_LITERAL);
            lit->name = strdup(CST->Value_p ? CST->Value_p->String.p : "");
            return lit;
        }

        case NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_EXPRESSION: {
            nh_ecmascript_ASTNode *assign = ast_alloc(NH_AST_ASSIGNMENT_EXPRESSION);
            // assume children[0] = lhs, children[1] = operator, children[2] = rhs
            assign->left = cstToAst(CST->Children.items[0]);
            assign->name = strdup(CST->Children.items[1]->Value_p->String.p); // operator
            assign->right = cstToAst(CST->Children.items[2]);
            return assign;
        }

        case NH_ECMASCRIPT_PARSE_NODE_EXPRESSION_STATEMENT: {
            nh_ecmascript_ASTNode *stmt = ast_alloc(NH_AST_EXPRESSION_STATEMENT);
            if (CST->Children.count > 0) {
                nh_ecmascript_ASTNode *expr = cstToAst(CST->Children.items[0]);
                if (expr) nh_core_appendToList(&stmt->children, expr);
            }
            return stmt;
        }

        case NH_ECMASCRIPT_PARSE_NODE_FOR_STATEMENT: {
            nh_ecmascript_ASTNode *loop = ast_alloc(NH_AST_FOR_STATEMENT);
            // children: init, test, update, body
            for (size_t i = 0; i < CST->Children.count; i++) {
                nh_ecmascript_ASTNode *part = cstToAst(CST->Children.items[i]);
                if (part) nh_core_appendToList(&loop->children, part);
            }
            return loop;
        }

        case NH_ECMASCRIPT_PARSE_NODE_CALL_EXPRESSION: {
            nh_ecmascript_ASTNode *call = ast_alloc(NH_AST_CALL_EXPRESSION);
            // usually [callee, arguments]
            for (size_t i = 0; i < CST->Children.count; i++) {
                nh_ecmascript_ASTNode *c = cstToAst(CST->Children.items[i]);
                if (c) nh_core_appendToList(&call->children, c);
            }
            return call;
        }

        // ... handle other nodes as needed

        default: {
            // generic pass-through: collapse if only one child
            if (CST->Children.count == 1)
                return cstToAst(CST->Children.items[0]);
            if (CST->Children.count == 0 && CST->Value_p) {
                // token wrapper
                nh_ecmascript_ASTNode *leaf = ast_alloc(NH_AST_LITERAL);
                leaf->name = strdup(CST->Value_p->String.p);
                return leaf;
            }
            // otherwise ignore (syntactic sugar node)
            return NULL;
        }
    }
}

