// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "DeclaredNames.h"

#include "../Engine/Value.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include <string.h>

// FUNCTIONS =======================================================================================

static void nh_ecmascript_getBoundNames(
    nh_ecmascript_ASTNode *Node_p, nh_core_List *List_p)
{
    if (!Node_p) return;

    if (Node_p->type == NH_ECMASCRIPT_AST_NODE_IDENTIFIER) {
        nh_core_appendToList(List_p, Node_p->Identifier.name_p);
        return;
    }

    // TODO: Extend here if you add object/array destructuring node types.
}

static void nh_ecmascript_getLexicallyDeclaredNamesRecursion(
    nh_ecmascript_ASTNode *Node_p, nh_core_List *Out_p)
{
    if (!Node_p) return;

    switch (Node_p->type) {
        case NH_ECMASCRIPT_AST_NODE_PROGRAM:
        {
            nh_core_List *Body_p = &Node_p->Program.Body;
            for (unsigned long i = 0; i < Body_p->size; ++i)
                nh_ecmascript_getLexicallyDeclaredNamesRecursion(Body_p->pp[i], Out_p);
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_BLOCK_STATEMENT:
        {
            nh_core_List *Body_p = &Node_p->BlockStatement.Body;
            for (unsigned long i = 0; i < Body_p->size; ++i)
                nh_ecmascript_getLexicallyDeclaredNamesRecursion(Body_p->pp[i], Out_p);
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATION:
        {
            const char *kind = Node_p->VariableDeclaration.kind_p;
            if (!kind || strcmp(kind, "var") == 0) break; // only let/const
            nh_core_List *decls = &Node_p->VariableDeclaration.Declarators;
            for (unsigned long i = 0; i < decls->size; ++i) {
                nh_ecmascript_ASTNode *decl = decls->pp[i];
                if (decl && decl->type == NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATOR)
                    nh_ecmascript_getBoundNames(decl->VariableDeclarator.Id_p, Out_p);
            }
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_FOR_DECLARATION:
        {
            const char *kind = Node_p->ForDeclaration.kind_p;
            if (!kind || strcmp(kind, "var") == 0) break;
            nh_core_List *decls = &Node_p->ForDeclaration.Declarators;
            for (unsigned long i = 0; i < decls->size; ++i) {
                nh_ecmascript_ASTNode *decl = decls->pp[i];
                if (decl && decl->type == NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATOR)
                    nh_ecmascript_getBoundNames(decl->VariableDeclarator.Id_p, Out_p);
            }
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_FUNCTION_DECLARATION: {
            // function declarations are lexical
            // guessing it has Identifier child `Id_p` (please adjust if different)
            // If not, just add logic here once you show your FunctionDeclaration struct
            // Example:
            // if (node->FunctionDeclaration.Id_p)
            //     stringvec_push(out, node->FunctionDeclaration.Id_p->Identifier.name_p);
            break;
        }

        default:
            break;
    }
}

nh_core_List nh_ecmascript_getLexicallyDeclaredNames(
    nh_ecmascript_ASTNode *Node_p)
{
    nh_core_List List = nh_core_initList(8);
    nh_ecmascript_getLexicallyDeclaredNamesRecursion(Node_p, &List);
    return List;
}

static void nh_ecmascript_getVarDeclaredNamesRecursion(
    nh_ecmascript_ASTNode *Node_p, nh_core_List *Names_p
) {
    if (!Node_p) return;

    switch (Node_p->type) {

        case NH_ECMASCRIPT_AST_NODE_PROGRAM: {
            for (unsigned long i = 0; i < Node_p->Program.Body.size; ++i) {
                nh_ecmascript_ASTNode *stmt_p = Node_p->Program.Body.pp[i];
                nh_ecmascript_getVarDeclaredNamesRecursion(stmt_p, Names_p);
            }
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_BLOCK_STATEMENT: {
            for (unsigned long i = 0; i < Node_p->BlockStatement.Body.size; ++i) {
                nh_ecmascript_ASTNode *stmt_p = Node_p->BlockStatement.Body.pp[i];
                nh_ecmascript_getVarDeclaredNamesRecursion(stmt_p, Names_p);
            }
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATION: {
            // Only collect for `var`, skip let/const
            if (strcmp(Node_p->VariableDeclaration.kind_p, "var") == 0) {
                for (unsigned long i = 0; i < Node_p->VariableDeclaration.Declarators.size; ++i) {
                    nh_ecmascript_ASTNode *decl_p = Node_p->VariableDeclaration.Declarators.pp[i];
                    nh_ecmascript_getBoundNames(decl_p->VariableDeclarator.Id_p, Names_p);
                }
            }
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_FOR_STATEMENT: {
            // Handle `for (var i = 0; ...)`
            if (Node_p->ForStatement.Init_p) {
                nh_ecmascript_getVarDeclaredNamesRecursion(Node_p->ForStatement.Init_p, Names_p);
            }
            nh_ecmascript_getVarDeclaredNamesRecursion(Node_p->ForStatement.Body_p, Names_p);
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_FOR_DECLARATION: {
            // This is likely a `for (var ...)` special node
            if (strcmp(Node_p->ForDeclaration.kind_p, "var") == 0) {
                for (unsigned long i = 0; i < Node_p->ForDeclaration.Declarators.size; ++i) {
                    nh_ecmascript_ASTNode *decl_p = Node_p->ForDeclaration.Declarators.pp[i];
                    nh_ecmascript_getBoundNames(decl_p->VariableDeclarator.Id_p, Names_p);
                }
            }
            break;
        }

        case NH_ECMASCRIPT_AST_NODE_FUNCTION_DECLARATION: {
//            // Functions are part of var-declared names
//            if (Node_p->FunctionDeclaration.Id_p) {
//                nh_ecmascript_getBoundNames(Node_p->FunctionDeclaration.Id_p, Names_p);
//            }
            break;
        }

        // Skip other statements; they don't contribute var-declared names.
        default:
            break;
    }
}

nh_core_List nh_ecmascript_getVarDeclaredNames(
    nh_ecmascript_ASTNode *Node_p)
{
    nh_core_List List = nh_core_initList(8);
    nh_ecmascript_getVarDeclaredNamesRecursion(Node_p, &List);
    return List;
}

// TOP LEVEL LEXICALLY DECLARED NAMES ==============================================================

// corresponds to https://tc39.es/ecma262/#sec-static-semantics-toplevellexicallydeclarednames
nh_core_List nh_ecmascript_getTopLevelLexicallyDeclaredNames(
    nh_ecmascript_ParseNode *Node_p)
{
//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
//            if (Node_p->Children.size == 3) {
//                return nh_ecmascript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[1]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
//        {
//            nh_core_List Names = nh_ecmascript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[0]);
//            if (Node_p->Children.size == 2) {
//                nh_core_List StatementNames = nh_ecmascript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[1]);
//                nh_core_appendItemsToList(&Names, &StatementNames);
//                nh_core_freeList(&StatementNames, false);
//            }
//            return Names;
//        }
//
//        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
//        {
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
//            {
//                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
//                if (((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
//                    break;
//                }
//                return nh_ecmascript_getBoundNames(Declaration_p, Out_p);
//            }
//            break;
//        }
//    }

    return nh_core_initList(8);
}

nh_core_List nh_ecmascript_getTopLevelVarDeclaredNames(
    nh_ecmascript_ParseNode *Node_p)
{
//    switch (Node_p->type)
//    {
//        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
//            if (Node_p->Children.size == 3) {
//                return nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[1]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
//            {
//                nh_core_List Names = nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[0]);
//                if (Node_p->Children.size == 2) {
//                    nh_core_List StatementNames = nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[1]);
//                    nh_core_appendItemsToList(&Names, &StatementNames);
//                    nh_core_freeList(&StatementNames, false);
//                }
//                return Names;
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
//            {
//                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
//                if (((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
//                    return nh_ecmascript_getBoundNames(((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->Children.pp[0]);
//                }
//            }
//            else {
//                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
//                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
//                    return nh_ecmascript_getTopLevelVarDeclaredNames(Statement_p->Children.pp[0]);
//                }
//                return nh_ecmascript_getVarDeclaredNames(Statement_p);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
//            return nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[2]);
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
//                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
//                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
//                    return nh_ecmascript_getTopLevelVarDeclaredNames(Statement_p->Children.pp[0]);
//                }
//                return nh_ecmascript_getVarDeclaredNames(Statement_p);
//            }
//            else {
//                return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//            }
//            break;
//    }

    return nh_core_initList(8);
}
