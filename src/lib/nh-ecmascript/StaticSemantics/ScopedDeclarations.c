// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "ScopedDeclarations.h"

#include "../Engine/Value.h"
#include "../../nh-core/Util/List.h"

#include <stdio.h>
#include <string.h>

// DECLARATION PART ================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-declarationpart
static nh_core_List nh_ecmascript_getDeclarationPart(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION :
        {
            nh_core_List List = nh_core_initList(8);
            nh_core_appendToList(&List, Node_p->Children.pp[0]);
            return List;
            break;
        }
        case NH_ECMASCRIPT_PARSE_NODE_DECLARATION :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                return nh_ecmascript_getDeclarationPart(Node_p->Children.pp[0]);
            }
            else { // must be ClassDeclaration or LexicalDeclaration
                nh_core_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p->Children.pp[0]);
                return List;
            }
            break;
    }

    return nh_core_initList(8);
}

// LEXICALLY SCOPED DECLARATIONS ===================================================================

// https://tc39.es/ecma262/#sec-static-semantics-lexicallyscopeddeclarations
nh_core_List nh_ecmascript_getLexicallyScopedDeclarations(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_core_List Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List StatementDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, false);
            }
            return Declarations;
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    return nh_ecmascript_getLexicallyScopedDeclarations(Statement_p->Children.pp[0]);
                }
            }
            else { // must be Declaration
                return nh_ecmascript_getDeclarationPart(Node_p->Children.pp[0]);
            }
            break;

//        case NH_ECMASCRIPT_PARSE_NODE_CASE_BLOCK :
//            if (Node_p->Children.size == 1 && ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                return nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
//            }
//            else if (Node_p->Children.size > 0)
//            {
//                nh_core_List Declarations = nh_core_initList(8);
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                    Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
//                    nh_core_List DefaultClauseDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
//                    for (int i = 0; i < DefaultClauseDeclarations.size; ++i) {
//                        nh_core_appendToList(&Declarations, DefaultClauseDeclarations.pp[i]);
//                    }
//                    nh_core_freeList(&DefaultClauseDeclarations, false);
//                    if (Node_p->Children.size > 2) {
//                        nh_core_List CaseClausesDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            nh_core_appendToList(&Declarations, CaseClausesDeclarations.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesDeclarations, false);
//                    }
//                }
//                else {
//                    Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
//                    if (Node_p->Children.size > 1) {
//                        nh_core_List CaseClausesDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            nh_core_appendToList(&Declarations, CaseClausesDeclarations.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesDeclarations, false);
//                    }
//                }
//                return Declarations;
//            }
//            break;
//
        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES :
        {
            nh_core_List Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List CaseDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &CaseDeclarations);
                nh_core_freeList(&CaseDeclarations, false);
            }
            return Declarations;
        }

        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSE :
            if (Node_p->Children.size == 2) {
                return nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DEFAULT_CLAUSE :
            if (Node_p->Children.size == 1) {
                return nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            return nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION) {
                nh_core_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p->Children.pp[0]);
                return List;
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_STATEMENT_LIST :
            if (Node_p->Children.size > 0) {
                return nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CONCISE_BODY :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_BODY) {
                nh_ecmascript_ParseNode *FunctionBody_p = Node_p->Children.pp[0];
                return nh_ecmascript_getLexicallyScopedDeclarations(FunctionBody_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_CONCISE_BODY :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_BODY) {
                nh_ecmascript_ParseNode *AsyncFunctionBody_p = Node_p->Children.pp[0];
                nh_ecmascript_ParseNode *FunctionBody_p = AsyncFunctionBody_p->Children.pp[0];
                return nh_ecmascript_getLexicallyScopedDeclarations(FunctionBody_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
            return nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_MODULE :
            return nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_BODY :
            return nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM_LIST :
        {
            nh_core_List Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List ModuleDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &ModuleDeclarations);
                nh_core_freeList(&ModuleDeclarations, false);
            }
            return Declarations;
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type != NH_ECMASCRIPT_PARSE_NODE_IMPORT_DECLARATION) {
                return nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPORT_FROM_CLAUSE
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_NAMED_EXPORTS
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT) {
                break;
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) {
                return nh_ecmascript_getDeclarationPart(Node_p->Children.pp[0]);
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                return nh_ecmascript_getDeclarationPart(Node_p->Children.pp[0]);
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CLASS_DECLARATION) {
                nh_core_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p->Children.pp[0]);
                return List;
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_EXPRESSION) {
                nh_core_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p);
                return List;
            }
            break;
    }

    return nh_core_initList(8);
}

// TOP LEVEL LEXICALLY SCOPED DECLARATIONS =========================================================

// https://tc39.es/ecma262/#sec-static-semantics-toplevellexicallyscopeddeclarations
nh_core_List nh_ecmascript_getTopLevelLexicallyScopedDeclarations(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 1) {
                return nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_core_List Declarations = nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List StatementDeclarations = nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, false);
            }
            return Declarations;
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
        {
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    break;
                }
                nh_core_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p->Children.pp[0]);
                return List;
            }
            break;
        }
    }

    return nh_core_initList(8);
}

// VAR SCOPED DECLARATIONS ===================================================================

// https://tc39.es/ecma262/#sec-static-semantics-varscopeddeclarations
nh_core_List nh_ecmascript_getVarScopedDeclarations(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
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
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_core_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List StatementDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, false);
            }
            return Declarations;
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST :
            if (Node_p->Children.size == 1) {
                nh_core_List Declarations = nh_core_initList(8); 
                nh_core_appendToList(&Declarations, Node_p->Children.pp[0]);
                return Declarations;
            }
            else {
                nh_core_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]); 
                nh_core_appendToList(&Declarations, Node_p->Children.pp[1]);
                return Declarations;
            }

        case NH_ECMASCRIPT_PARSE_NODE_IF_STATEMENT :
            if (Node_p->Children.size == 3) {
                nh_core_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_List Tmp = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[2]);
                nh_core_appendItemsToList(&Declarations, &Tmp);
                nh_core_freeList(&Tmp, false);
                return Declarations;
            }
            else {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DO_WHILE_STATEMENT :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_WHILE_STATEMENT :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);

        case NH_ECMASCRIPT_PARSE_NODE_FOR_STATEMENT :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST) {
                nh_core_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
                nh_core_List StatementDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, false);
                return Declarations;
            }
            else {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]);
            }

        case NH_ECMASCRIPT_PARSE_NODE_FOR_IN_OF_STATEMENT :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING) {
                nh_core_List Declarations = nh_core_initList(8);
                nh_core_appendToList(&Declarations, Node_p->Children.pp[0]);
                nh_core_List StatementDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, false);
                return Declarations;
            }
            else {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_WITH_STATEMENT :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);

        case NH_ECMASCRIPT_PARSE_NODE_SWITCH_STATEMENT :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);

//        case NH_ECMASCRIPT_PARSE_NODE_CASE_BLOCK :
//            if (Node_p->Children.size == 1 && ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
//            }
//            else if (Node_p->Children.size > 0)
//            {
//                nh_core_List Declarations = nh_core_initList(8);
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                    Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
//                    nh_core_List DefaultClauseDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
//                    for (int i = 0; i < DefaultClauseDeclarations.size; ++i) {
//                        nh_core_appendToList(&Declarations, DefaultClauseDeclarations.pp[i]);
//                    }
//                    nh_core_freeList(&DefaultClauseDeclarations, false);
//                    if (Node_p->Children.size > 2) {
//                        nh_core_List CaseClausesDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            nh_core_appendToList(&Declarations, CaseClausesDeclarations.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesDeclarations, false);
//                    }
//                }
//                else {
//                    Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
//                    if (Node_p->Children.size > 1) {
//                        nh_core_List CaseClausesDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            nh_core_appendToList(&Declarations, CaseClausesDeclarations.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesDeclarations, false);
//                    }
//                }
//                return Declarations;
//            }
//            break;
//
        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES :
        {
            nh_core_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List CaseDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &CaseDeclarations);
                nh_core_freeList(&CaseDeclarations, false);
            }
            return Declarations;
        }

        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSE :
            if (Node_p->Children.size == 2) {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DEFAULT_CLAUSE :
            if (Node_p->Children.size == 1) {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_TRY_STATEMENT :
            if (Node_p->Children.size == 2) {
                nh_core_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
                nh_core_List CatchDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &CatchDeclarations);
                nh_core_freeList(&CatchDeclarations, false);
                return Declarations;
            }
            else if (Node_p->Children.size == 3) {
                nh_core_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
                nh_core_List CatchDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_List FinallyDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[2]);
                nh_core_appendItemsToList(&Declarations, &CatchDeclarations);
                nh_core_appendItemsToList(&Declarations, &FinallyDeclarations);
                nh_core_freeList(&CatchDeclarations, false);
                nh_core_freeList(&FinallyDeclarations, false);
                return Declarations;
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CATCH :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]);

        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_STATEMENT_LIST :
            if (Node_p->Children.size > 0) {
                return nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CONCISE_BODY :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_BODY) {
                nh_ecmascript_ParseNode *FunctionBody_p = Node_p->Children.pp[0];
                return nh_ecmascript_getVarScopedDeclarations(FunctionBody_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_CONCISE_BODY :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_BODY) {
                nh_ecmascript_ParseNode *AsyncFunctionBody_p = Node_p->Children.pp[0];
                nh_ecmascript_ParseNode *FunctionBody_p = AsyncFunctionBody_p->Children.pp[0];
                return nh_ecmascript_getVarScopedDeclarations(FunctionBody_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
            return nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_MODULE :
            if (Node_p->Children.size > 0) {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]); 
            }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_BODY :
            return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM_LIST :
        {
            nh_core_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List ModuleDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &ModuleDeclarations);
                nh_core_freeList(&ModuleDeclarations, false);
            }
            return Declarations;
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM) {
                return nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION) {
                nh_ecmascript_ParseNode *ExportDeclaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT) {
                    return nh_ecmascript_getVarScopedDeclarations(ExportDeclaration_p->Children.pp[0]);
                }
            }
            break;           
    }

    return nh_core_initList(8);
}

// TOP LEVEL VAR SCOPED DECLARATIONS ===============================================================

// https://tc39.es/ecma262/#sec-static-semantics-toplevelvarscopeddeclarations
nh_core_List nh_ecmascript_getTopLevelVarScopedDeclarations(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 1) {
                return nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[0]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_core_List Declarations = nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List StatementDeclarations = nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, false);
            }
            return Declarations;
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
        {
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    nh_core_List Declaration = nh_ecmascript_getDeclarationPart(Declaration_p->Children.pp[0]);
                    return Declaration;
                }
            }
            else {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    return nh_ecmascript_getTopLevelVarScopedDeclarations(Statement_p->Children.pp[0]);
                }
                return nh_ecmascript_getVarScopedDeclarations(Statement_p);
            }
            break;
        }

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            return nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[1]);

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) 
            {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    return nh_ecmascript_getTopLevelVarScopedDeclarations(Statement_p->Children.pp[0]);
                }
                return nh_ecmascript_getVarScopedDeclarations(Statement_p);
            }
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION) {
                nh_ecmascript_ParseNode *FunctionDeclaration_p = Node_p->Children.pp[0];
                nh_core_List Declarations = nh_core_initList(8);
                nh_core_appendToList(&Declarations, FunctionDeclaration_p);
                return Declarations;
            } 
            break;
    }

    return nh_core_initList(8);
}

