// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "ScopedDeclarations.h"

#include "../Engine/Type.h"
#include "../Common/Macros.h"

#include "../../nhcore/Util/List.h"

#include <stdio.h>
#include <string.h>

// DECLARATION PART ================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-declarationpart
static nh_List nh_ecmascript_getDeclarationPart(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION :
        {
            nh_List List = nh_core_initList(8);
            nh_core_appendToList(&List, Node_p->Children.pp[0]);
            NH_ECMASCRIPT_END(List)
            break;
        }
        case NH_ECMASCRIPT_PARSE_NODE_DECLARATION :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                NH_ECMASCRIPT_END(nh_ecmascript_getDeclarationPart(Node_p->Children.pp[0]))
            }
            else { // must be ClassDeclaration or LexicalDeclaration
                nh_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p->Children.pp[0]);
                NH_ECMASCRIPT_END(List)
            }
            break;
    }

NH_ECMASCRIPT_END(nh_core_initList(8))
}

// LEXICALLY SCOPED DECLARATIONS ===================================================================

// https://tc39.es/ecma262/#sec-static-semantics-lexicallyscopeddeclarations
nh_List nh_ecmascript_getLexicallyScopedDeclarations(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_List Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List StatementDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(Statement_p->Children.pp[0]))
                }
            }
            else { // must be Declaration
                NH_ECMASCRIPT_END(nh_ecmascript_getDeclarationPart(Node_p->Children.pp[0]))
            }
            break;

//        case NH_ECMASCRIPT_PARSE_NODE_CASE_BLOCK :
//            if (Node_p->Children.size == 1 && ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]))
//            }
//            else if (Node_p->Children.size > 0)
//            {
//                nh_List Declarations = nh_core_initList(8);
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                    Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
//                    nh_List DefaultClauseDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
//                    for (int i = 0; i < DefaultClauseDeclarations.size; ++i) {
//                        nh_core_appendToList(&Declarations, DefaultClauseDeclarations.pp[i]);
//                    }
//                    nh_core_freeList(&DefaultClauseDeclarations, NH_FALSE);
//                    if (Node_p->Children.size > 2) {
//                        nh_List CaseClausesDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            nh_core_appendToList(&Declarations, CaseClausesDeclarations.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesDeclarations, NH_FALSE);
//                    }
//                }
//                else {
//                    Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
//                    if (Node_p->Children.size > 1) {
//                        nh_List CaseClausesDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            nh_core_appendToList(&Declarations, CaseClausesDeclarations.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesDeclarations, NH_FALSE);
//                    }
//                }
//                NH_ECMASCRIPT_END(Declarations)
//            }
//            break;
//
        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES :
        {
            nh_List Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List CaseDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &CaseDeclarations);
                nh_core_freeList(&CaseDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSE :
            if (Node_p->Children.size == 2) {
                NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DEFAULT_CLAUSE :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION) {
                nh_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p->Children.pp[0]);
                NH_ECMASCRIPT_END(List)
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_STATEMENT_LIST :
            if (Node_p->Children.size > 0) {
                NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CONCISE_BODY :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_BODY) {
                nh_ecmascript_ParseNode *FunctionBody_p = Node_p->Children.pp[0];
                NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(FunctionBody_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_CONCISE_BODY :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_BODY) {
                nh_ecmascript_ParseNode *AsyncFunctionBody_p = Node_p->Children.pp[0];
                nh_ecmascript_ParseNode *FunctionBody_p = AsyncFunctionBody_p->Children.pp[0];
                NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(FunctionBody_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
            NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE :
            NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_BODY :
            NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM_LIST :
        {
            nh_List Declarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List ModuleDeclarations = nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &ModuleDeclarations);
                nh_core_freeList(&ModuleDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type != NH_ECMASCRIPT_PARSE_NODE_IMPORT_DECLARATION) {
                NH_ECMASCRIPT_END(nh_ecmascript_getLexicallyScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPORT_FROM_CLAUSE
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_NAMED_EXPORTS
            ||  ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT) {
                break;
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) {
                NH_ECMASCRIPT_END(nh_ecmascript_getDeclarationPart(Node_p->Children.pp[0]))
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                NH_ECMASCRIPT_END(nh_ecmascript_getDeclarationPart(Node_p->Children.pp[0]))
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CLASS_DECLARATION) {
                nh_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p->Children.pp[0]);
                NH_ECMASCRIPT_END(List)
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_EXPRESSION) {
                nh_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p);
                NH_ECMASCRIPT_END(List)
            }
            break;
    }

NH_ECMASCRIPT_END(nh_core_initList(8))
}

// TOP LEVEL LEXICALLY SCOPED DECLARATIONS =========================================================

// https://tc39.es/ecma262/#sec-static-semantics-toplevellexicallyscopeddeclarations
nh_List nh_ecmascript_getTopLevelLexicallyScopedDeclarations(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_List Declarations = nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List StatementDeclarations = nh_ecmascript_getTopLevelLexicallyScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
        {
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    break;
                }
                nh_List List = nh_core_initList(8);
                nh_core_appendToList(&List, Node_p->Children.pp[0]);
                NH_ECMASCRIPT_END(List)
            }
            break;
        }
    }

NH_ECMASCRIPT_END(nh_core_initList(8))
}

// VAR SCOPED DECLARATIONS ===================================================================

// https://tc39.es/ecma262/#sec-static-semantics-varscopeddeclarations
nh_List nh_ecmascript_getVarScopedDeclarations(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

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
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List StatementDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST :
            if (Node_p->Children.size == 1) {
                nh_List Declarations = nh_core_initList(8); 
                nh_core_appendToList(&Declarations, Node_p->Children.pp[0]);
                NH_ECMASCRIPT_END(Declarations)
            }
            else {
                nh_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]); 
                nh_core_appendToList(&Declarations, Node_p->Children.pp[1]);
                NH_ECMASCRIPT_END(Declarations)
            }

        case NH_ECMASCRIPT_PARSE_NODE_IF_STATEMENT :
            if (Node_p->Children.size == 3) {
                nh_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_List Tmp = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[2]);
                nh_core_appendItemsToList(&Declarations, &Tmp);
                nh_core_freeList(&Tmp, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            else {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DO_WHILE_STATEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_WHILE_STATEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_FOR_STATEMENT :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST) {
                nh_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
                nh_List StatementDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            else {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]))
            }

        case NH_ECMASCRIPT_PARSE_NODE_FOR_IN_OF_STATEMENT :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING) {
                nh_List Declarations = nh_core_initList(8);
                nh_core_appendToList(&Declarations, Node_p->Children.pp[0]);
                nh_List StatementDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            else {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_WITH_STATEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_SWITCH_STATEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]))

//        case NH_ECMASCRIPT_PARSE_NODE_CASE_BLOCK :
//            if (Node_p->Children.size == 1 && ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))
//            }
//            else if (Node_p->Children.size > 0)
//            {
//                nh_List Declarations = nh_core_initList(8);
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES) {
//                    Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
//                    nh_List DefaultClauseDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
//                    for (int i = 0; i < DefaultClauseDeclarations.size; ++i) {
//                        nh_core_appendToList(&Declarations, DefaultClauseDeclarations.pp[i]);
//                    }
//                    nh_core_freeList(&DefaultClauseDeclarations, NH_FALSE);
//                    if (Node_p->Children.size > 2) {
//                        nh_List CaseClausesDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            nh_core_appendToList(&Declarations, CaseClausesDeclarations.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesDeclarations, NH_FALSE);
//                    }
//                }
//                else {
//                    Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
//                    if (Node_p->Children.size > 1) {
//                        nh_List CaseClausesDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
//                        for (int i = 0; i < CaseClausesDeclarations.size; ++i) {
//                            nh_core_appendToList(&Declarations, CaseClausesDeclarations.pp[i]);
//                        }
//                        nh_core_freeList(&CaseClausesDeclarations, NH_FALSE);
//                    }
//                }
//                NH_ECMASCRIPT_END(Declarations)
//            }
//            break;
//
        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSES :
        {
            nh_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List CaseDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &CaseDeclarations);
                nh_core_freeList(&CaseDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_CASE_CLAUSE :
            if (Node_p->Children.size == 2) {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_DEFAULT_CLAUSE :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_TRY_STATEMENT :
            if (Node_p->Children.size == 2) {
                nh_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
                nh_List CatchDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &CatchDeclarations);
                nh_core_freeList(&CatchDeclarations, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            else if (Node_p->Children.size == 3) {
                nh_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
                nh_List CatchDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_List FinallyDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[2]);
                nh_core_appendItemsToList(&Declarations, &CatchDeclarations);
                nh_core_appendItemsToList(&Declarations, &FinallyDeclarations);
                nh_core_freeList(&CatchDeclarations, NH_FALSE);
                nh_core_freeList(&FinallyDeclarations, NH_FALSE);
                NH_ECMASCRIPT_END(Declarations)
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CATCH :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[Node_p->Children.size - 1]))

        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_STATEMENT_LIST :
            if (Node_p->Children.size > 0) {
                NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_CONCISE_BODY :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_BODY) {
                nh_ecmascript_ParseNode *FunctionBody_p = Node_p->Children.pp[0];
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(FunctionBody_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_ASYNC_CONCISE_BODY :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ASYNC_FUNCTION_BODY) {
                nh_ecmascript_ParseNode *AsyncFunctionBody_p = Node_p->Children.pp[0];
                nh_ecmascript_ParseNode *FunctionBody_p = AsyncFunctionBody_p->Children.pp[0];
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(FunctionBody_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT_BODY :
            NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE :
            if (Node_p->Children.size > 0) {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0])) 
            }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_BODY :
            NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM_LIST :
        {
            nh_List Declarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List ModuleDeclarations = nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &ModuleDeclarations);
                nh_core_freeList(&ModuleDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_MODULE_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM) {
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Node_p->Children.pp[0]))
            }
            else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_EXPORT_DECLARATION) {
                nh_ecmascript_ParseNode *ExportDeclaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT) {
                    NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(ExportDeclaration_p->Children.pp[0]))
                }
            }
            break;           
    }

NH_ECMASCRIPT_END(nh_core_initList(8))
}

// TOP LEVEL VAR SCOPED DECLARATIONS ===============================================================

// https://tc39.es/ecma262/#sec-static-semantics-toplevelvarscopeddeclarations
nh_List nh_ecmascript_getTopLevelVarScopedDeclarations(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK :
            if (Node_p->Children.size == 1) {
                NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[0]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST :
        {
            nh_List Declarations = nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List StatementDeclarations = nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Declarations, &StatementDeclarations);
                nh_core_freeList(&StatementDeclarations, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Declarations)
        }

        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM :
        {
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_DECLARATION) 
            {
                nh_ecmascript_ParseNode *Declaration_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Declaration_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_HOISTABLE_DECLARATION) {
                    nh_List Declaration = nh_ecmascript_getDeclarationPart(Declaration_p->Children.pp[0]);
                    NH_ECMASCRIPT_END(Declaration)
                }
            }
            else {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelVarScopedDeclarations(Statement_p->Children.pp[0]))
                }
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Statement_p))
            }
            break;
        }

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelVarScopedDeclarations(Node_p->Children.pp[1]))

        case NH_ECMASCRIPT_PARSE_NODE_LABELLED_ITEM :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT) 
            {
                nh_ecmascript_ParseNode *Statement_p = Node_p->Children.pp[0];
                if (((nh_ecmascript_ParseNode*)Statement_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_LABELLED_STATEMENT) {
                    NH_ECMASCRIPT_END(nh_ecmascript_getTopLevelVarScopedDeclarations(Statement_p->Children.pp[0]))
                }
                NH_ECMASCRIPT_END(nh_ecmascript_getVarScopedDeclarations(Statement_p))
            }
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION) {
                nh_ecmascript_ParseNode *FunctionDeclaration_p = Node_p->Children.pp[0];
                nh_List Declarations = nh_core_initList(8);
                nh_core_appendToList(&Declarations, FunctionDeclaration_p);
                NH_ECMASCRIPT_END(Declarations)
            } 
            break;
    }

NH_ECMASCRIPT_END(nh_core_initList(8))
}

