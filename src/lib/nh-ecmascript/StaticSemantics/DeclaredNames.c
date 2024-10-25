// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "DeclaredNames.h"
#include "BoundNames.h"

#include "../Engine/Type.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include <string.h>

// LEXICALLY DECLARED NAMES ========================================================================

nh_core_List nh_ecmascript_getLexicallyDeclaredNames(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 3) {
                return nh_ecmascript_getLexicallyDeclaredNames(Node_p->Children.pp[1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_core_List Names = nh_ecmascript_getLexicallyDeclaredNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List StatementNames = nh_ecmascript_getLexicallyDeclaredNames(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Names, &StatementNames);
                nh_core_freeList(&StatementNames, false);
            }
            return Names;
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    return nh_ecmascript_getLexicallyDeclaredNames(Statement_p->Children.pp[0]);
                }
            }
            else { // must be Declaration
                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                return nh_ecmascript_getBoundNames(Declaration_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
        {
            nh_ecmascript_ParseNode *StatementList_p = Node_p->Children.pp[0];
            return nh_ecmascript_getTopLevelLexicallyDeclaredNames(StatementList_p);
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM_LIST :
        {
            nh_core_List Names = nh_ecmascript_getLexicallyDeclaredNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List ModuleNames = nh_ecmascript_getLexicallyDeclaredNames(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Names, &ModuleNames);
                nh_core_freeList(&ModuleNames, false);
            }
            return Names;
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_IMPORT_DECLARATION) {
                return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION) {
                nh_ecmascript_ParseNode *ExportDeclaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)ExportDeclaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT) {
                    break;
                } 
                return nh_ecmascript_getBoundNames(ExportDeclaration_p);
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM) {
                return nh_ecmascript_getLexicallyDeclaredNames(Node_p->Children.pp[0]);
            }          
            break;

// CaseBlock : { }
//
//    Return a new empty List.
//
//CaseBlock : { CaseClausesopt DefaultClause CaseClausesopt }
//
//    If the first CaseClauses is present, let names be the LexicallyDeclaredNames of the first CaseClauses.
//    Else, let names be a new empty List.
//    Append to names the elements of the LexicallyDeclaredNames of DefaultClause.
//    If the second CaseClauses is not present, return names.
//    Return the result of appending to names the elements of the LexicallyDeclaredNames of the second CaseClauses.
//
//CaseClauses : CaseClauses CaseClause
//
//    Let names be LexicallyDeclaredNames of CaseClauses.
//    Append to names the elements of the LexicallyDeclaredNames of CaseClause.
//    Return names.
//
//CaseClause : case Expression : StatementListopt
//
//    If the StatementList is present, return the LexicallyDeclaredNames of StatementList.
//    Return a new empty List.
//
//DefaultClause : default : StatementListopt
//
//    If the StatementList is present, return the LexicallyDeclaredNames of StatementList.
//    Return a new empty List.
//
//LabelledStatement : LabelIdentifier : LabelledItem
//
//    Return the LexicallyDeclaredNames of LabelledItem.
//
//LabelledItem : Statement
//
//    Return a new empty List.
//
//LabelledItem : FunctionDeclaration
//
//    Return BoundNames of FunctionDeclaration.
//
//FunctionStatementList : [empty]
//
//    Return a new empty List.
//
//FunctionStatementList : StatementList
//
//    Return TopLevelLexicallyDeclaredNames of StatementList.
//
//ConciseBody : ExpressionBody
//
//    Return a new empty List.
//
//AsyncConciseBody : ExpressionBody
//
//    Return a new empty List.
    }

    return nh_core_initList(8);
}

// TOP LEVEL LEXICALLY DECLARED NAMES ==============================================================

// corresponds to https://tc39.es/ecma262/#sec-static-semantics-toplevellexicallydeclarednames
nh_core_List nh_ecmascript_getTopLevelLexicallyDeclaredNames(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 3) {
                return nh_ecmascript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_core_List Names = nh_ecmascript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List StatementNames = nh_ecmascript_getTopLevelLexicallyDeclaredNames(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Names, &StatementNames);
                nh_core_freeList(&StatementNames, false);
            }
            return Names;
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
        {
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    break;
                }
                return nh_ecmascript_getBoundNames(Declaration_p);
            }
            break;
        }
    }

    return nh_core_initList(8);
}

// VAR DECLARED NAMES ==============================================================================

nh_core_List nh_ecmascript_getVarDeclaredNames(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
        {
            nh_ecmascript_ParseNode *StatementList_p = Node_p->Children.pp[0];
            return nh_ecmascript_getTopLevelVarDeclaredNames(StatementList_p);
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EMPTY_STATEMENT
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPRESSION_STATEMENT
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CONTINUE_STATEMENT
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BREAK_STATEMENT
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_RETURN_STATEMENT
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_THROW_STATEMENT
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DEBUGGER_STATEMENT) {
                break;
            }
            return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 3) {
                return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
            {
                nh_core_List Names = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);
                if (Node_p->Children.size == 2) {
                    nh_core_List StatementNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
                    nh_core_appendItemsToList(&Names, &StatementNames);
                    nh_core_freeList(&StatementNames, false);
                }
                return Names;
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT :
            return nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);

//        case NH_ECMASCRIPT_PARSE_NODE_IF_STATEMENT :
//            if (Node_p->Children.size == 3)
//            {
//                nh_core_List FirstStatementNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                nh_core_List SecondStatementNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[2]);
//                for (int i = 0; i < SecondStatementNames.size; ++i) {
//                    nh_core_appendToList(&FirstStatementNames, SecondStatementNames.pp[i]);
//                }
//                nh_core_freeList(&SecondStatementNames, false);
//                return FirstStatementNames;
//            }
//            else if (Node_p->Children.size == 2) {
//                return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_DO_WHILE_STATEMENT :
//            return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_WHILE_STATEMENT :
//            return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_STATEMENT :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST) {
//                nh_core_List VariableDeclarationListNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                nh_core_List StatementNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]);
//                for (int i = 0; i < StatementNames.size; ++i) {
//                    nh_core_appendToList(&VariableDeclarationListNames, StatementNames.pp[i]);
//                }
//                nh_core_freeList(&StatementNames, false);
//                return VariableDeclarationListNames;
//            }
//            else {
//                return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]);
//            }
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_IN_OF_STATEMENT :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING) {
//                nh_core_List ForBindingNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                nh_core_List StatementNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]);
//                for (int i = 0; i < StatementNames.size; ++i) {
//                    nh_core_appendToList(&ForBindingNames, StatementNames.pp[i]);
//                }
//                nh_core_freeList(&StatementNames, false);
//                return ForBindingNames;
//            }
//            else {
//                return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_WITH_STATEMENT :
//            return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_SWITCH_STATEMENT :
//            return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_CASE_BLOCK :
//            if (Node_p->Children.size == 1 && ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);
//            }
//            else if (Node_p->Children.size > 0)
//            {
//                nh_core_List Names = nh_core_initList(8);
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                    Names = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);
//                    nh_core_List DefaultClauseNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                    for (int i = 0; i < DefaultClauseNames.size; ++i) {
//                        nh_core_appendToList(&Names, DefaultClauseNames.pp[i]);
//                    }
//                    nh_core_freeList(&DefaultClauseNames, false);
//                    if (Node_p->Children.size > 2) {
//                        nh_core_List CaseClausesNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesNames.size; ++i) {
//                            nh_core_appendToList(&Names, CaseClausesNames.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesNames, false);
//                    }
//                }
//                else {
//                    Names = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);
//                    if (Node_p->Children.size > 1) {
//                        nh_core_List CaseClausesNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesNames.size; ++i) {
//                            nh_core_appendToList(&Names, CaseClausesNames.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesNames, false);
//                    }
//                }
//                return Names;
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES :
//            {
//                nh_core_List Names = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);
//                if (Node_p->Children.size == 2) {
//                    nh_core_List ClauseNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                    for (int i = 0; i < ClauseNames.size; ++i) {
//                        nh_core_appendToList(&Names, ClauseNames.pp[i]);
//                    }
//                    nh_core_freeList(&ClauseNames, false);
//                }
//                return Names;
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSE :
//            if (Node_p->Children.size == 2) {
//                return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_DEFAULT_CLAUSE :
//            if (Node_p->Children.size == 1) {
//                return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
//            return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
//                return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[0]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_TRY_STATEMENT :
//            if (Node_p->Children.size == 2) {
//                nh_core_List BlockNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                nh_core_List CatchOrFinallyNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                for (int i = 0; i < CatchOrFinallyNames.size; ++i) {
//                    nh_core_appendToList(&BlockNames, CatchOrFinallyNames.pp[i]);
//                }
//                nh_core_freeList(&CatchOrFinallyNames, false);
//                return BlockNames;
//            }
//            else if (Node_p->Children.size == 3) {
//                nh_core_List BlockNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                nh_core_List CatchNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[1]);
//                nh_core_List FinallyNames = nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[2]);
//                for (int i = 0; i < CatchNames.size; ++i) {
//                    nh_core_appendToList(&BlockNames, CatchNames.pp[i]);
//                }
//                for (int i = 0; i < FinallyNames.size; ++i) {
//                    nh_core_appendToList(&BlockNames, FinallyNames.pp[i]);
//                }
//                nh_core_freeList(&CatchNames, false);
//                nh_core_freeList(&FinallyNames, false);
//                return BlockNames;
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CATCH :
//            return nh_ecmascript_getVarDeclaredNames(Node_p->Children.pp[Node_p->Children.size - 1]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_STATEMENT_LIST :
//            if (Node_p->Children.size > 0) {
//                return nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[0]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_CONCISE_BODY :
//            break;
    }

    return nh_core_initList(8);
}

// TOP LEVEL VAR DECLARED NAMES ==============================================================

nh_core_List nh_ecmascript_getTopLevelVarDeclaredNames(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 3) {
                return nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
            {
                nh_core_List Names = nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[0]);
                if (Node_p->Children.size == 2) {
                    nh_core_List StatementNames = nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[1]);
                    nh_core_appendItemsToList(&Names, &StatementNames);
                    nh_core_freeList(&StatementNames, false);
                }
                return Names;
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    return nh_ecmascript_getBoundNames(((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->Children.pp[0]);
                }
            }
            else {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    return nh_ecmascript_getTopLevelVarDeclaredNames(Statement_p->Children.pp[0]);
                }
                return nh_ecmascript_getVarDeclaredNames(Statement_p);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            return nh_ecmascript_getTopLevelVarDeclaredNames(Node_p->Children.pp[2]);
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    return nh_ecmascript_getTopLevelVarDeclaredNames(Statement_p->Children.pp[0]);
                }
                return nh_ecmascript_getVarDeclaredNames(Statement_p);
            }
            else {
                return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            }
            break;
    }

    return nh_core_initList(8);
}

