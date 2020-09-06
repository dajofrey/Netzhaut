// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Variable.h"
#include "../Header/Macros.h"
#include "../Header/Script.h"
#include "../Header/Memory.h"

#include "../DOM/Header/HTMLCollection.h"

#include "../../Core/Header/Memory.h"

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG
#include NH_JS_UTILS

#include <string.h>
#include <stdio.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_JS_updateVariablesRecursively(
    Nh_JS_Parsable *Parsable_p, NH_BOOL reset
);

// INIT ============================================================================================

static void Nh_JS_initVariable(
    Nh_JS_Variable *Variable_p, char *name_p, int depth, NH_JS_KEYWORD kind)
{
NH_BEGIN()

    Variable_p->depth    = depth;
    Variable_p->type     = -1; // NOTE type deduction happens at interpreter level 
    Variable_p->data_p   = NULL;   
    Variable_p->name_p   = name_p;
    Variable_p->kind     = kind;
    Variable_p->freeData = false; 

NH_SILENT_END()
}

// ADD =============================================================================================

NH_RESULT Nh_JS_addVariable(
    Nh_List *Variables_p, char *name_p, int depth, NH_JS_KEYWORD kind)
{
NH_BEGIN()

    Nh_JS_Variable *Variable_p = Nh_allocate(sizeof(Nh_JS_Variable));
    NH_CHECK_MEM(Variable_p)

    Nh_JS_initVariable(Variable_p, name_p, depth, kind);
    NH_CHECK(Nh_addListItem(Variables_p, Variable_p))

NH_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

NH_RESULT Nh_JS_updateScriptVariables(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    Nh_JS_Parsable Parsable = {
        .type   = NH_JS_PARSABLE_BLOCK_STATEMENT, 
        .Data_p = &Script_p->Container,
    };
    NH_CHECK(Nh_JS_updateVariablesRecursively(&Parsable, NH_FALSE))

NH_END(NH_SUCCESS)
}

void Nh_JS_resetScriptVariables(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    Nh_JS_Parsable Parsable = {
        .type   = NH_JS_PARSABLE_BLOCK_STATEMENT, 
        .Data_p = &Script_p->Container,
    };
    Nh_JS_updateVariablesRecursively(&Parsable, NH_TRUE);

NH_SILENT_END()
}

void Nh_JS_resetBlockVariables(
    Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    if (Parsable_p->type != NH_JS_PARSABLE_BLOCK_STATEMENT) {NH_SILENT_END()}
    Nh_JS_BlockStatement *Block_p = Parsable_p->Data_p;

    for (int i = 0; i < Block_p->Variables.count; ++i) 
    {
        Nh_JS_Variable *Variable_p = Nh_getListItem(&Block_p->Variables, i);
        if (!Variable_p->freeData) {NH_SILENT_END()}
        Nh_JS_free(Variable_p->type, Variable_p->data_p);
        Nh_JS_initVariable(Variable_p, Variable_p->name_p, Variable_p->depth, Variable_p->kind);
    }
    
NH_SILENT_END()
}

static NH_RESULT Nh_JS_updateBlockVariables(
    Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    if (Parsable_p->type != NH_JS_PARSABLE_BLOCK_STATEMENT) {NH_END(NH_ERROR_BAD_STATE)}
    Nh_JS_BlockStatement *Block_p = Parsable_p->Data_p;

    for (int i = 0; i < Block_p->Variables.count; ++i) 
    {
        Nh_JS_Variable *Variable_p = Nh_getListItem(&Block_p->Variables, i);
        
        switch (Variable_p->type) 
        {
            case NH_JS_TYPE_OBJECT :
            {
                Nh_JS_Object *Object_p = Variable_p->data_p;
                
                switch (Object_p->type)
                {
                    case NH_JS_OBJECT_HTML_COLLECTION : 
                    {
                        NH_CHECK(Nh_JS_updateHTMLCollection(Object_p->data_p))
                        break;
                    }
                }

                break;
            }
        }
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_updateVariablesRecursively(
    Nh_JS_Parsable *Parsable_p, NH_BOOL reset)
{
NH_BEGIN()

    switch (Parsable_p->type) 
    {
        case NH_JS_PARSABLE_ASSIGNMENT_EXPRESSION : 
            {
                Nh_JS_AssignmentExpression *AssignmentExpression_p = Parsable_p->Data_p;               
                Nh_JS_updateVariablesRecursively(&AssignmentExpression_p->Left, reset);
                Nh_JS_updateVariablesRecursively(&AssignmentExpression_p->Right, reset);
            }
            break;

        case NH_JS_PARSABLE_EXPRESSION_STATEMENT : 
            {
                Nh_JS_ExpressionStatement *ExpressionStatement_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(&ExpressionStatement_p->Parsable, reset);
            }
            break;

        case NH_JS_PARSABLE_BLOCK_STATEMENT :
            {
                Nh_JS_BlockStatement *BlockStatement_p = Parsable_p->Data_p;
                for (int i = 0; i < BlockStatement_p->parsableCount; ++i) {
                    Nh_JS_updateVariablesRecursively(&BlockStatement_p->Parsables_p[i], reset);
                }
                if (reset)  {Nh_JS_resetBlockVariables(Parsable_p);}
                else {NH_CHECK(Nh_JS_updateBlockVariables(Parsable_p))}
            } 
            break;

        case NH_JS_PARSABLE_MEMBER_EXPRESSION : 
            {
                Nh_JS_MemberExpression *MemberExpression_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(&MemberExpression_p->Left, reset);
                Nh_JS_updateVariablesRecursively(&MemberExpression_p->Right, reset);
            }
            break;

        case NH_JS_PARSABLE_STRING_LITERAL : 
            break;

        case NH_JS_PARSABLE_CALL_EXPRESSION :
            {
                Nh_JS_CallExpression *CallExpression_p = Parsable_p->Data_p;
                for (int i = 0; i < CallExpression_p->argumentCount; ++i) {
                    Nh_JS_updateVariablesRecursively(&CallExpression_p->Arguments_p[i], reset);
                }
                Nh_JS_updateVariablesRecursively(&CallExpression_p->Parsable, reset);
            }
            break;

        case NH_JS_PARSABLE_IDENTIFIER : 
            break;
        
        case NH_JS_PARSABLE_FUNCTION_DECLARATION : 
            {
                Nh_JS_FunctionDeclaration *Function_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(&Function_p->Name, reset);
                for (int i = 0; i < Function_p->parameterCount; ++i) {
                    Nh_JS_updateVariablesRecursively(&Function_p->Parameters_p[i], reset);
                }
                Nh_JS_updateVariablesRecursively(&Function_p->Block, reset);
            }
            break;

        case NH_JS_PARSABLE_VARIABLE_DECLARATION :  
            {
                Nh_JS_VariableDeclaration *Declaration_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(&Declaration_p->Declarator.Identifier, reset);
                Nh_JS_updateVariablesRecursively(&Declaration_p->Declarator.Parsable, reset);
            }
            break;

        case NH_JS_PARSABLE_FOR_STATEMENT :
            {
                Nh_JS_ForStatement *For_p = Parsable_p->Data_p;
                for (int i = 0; i < For_p->expressionCount; ++i) {
                    Nh_JS_updateVariablesRecursively(&For_p->Expressions_p[i], reset);
                } 
                Nh_JS_updateVariablesRecursively(&For_p->Statement, reset);
            }
            break;

        case NH_JS_PARSABLE_IF_STATEMENT :
            {
                Nh_JS_IfStatement *If_p = Parsable_p->Data_p;
                for (int i = 0; i < If_p->parsableCount; ++i) {
                    Nh_JS_updateVariablesRecursively(&If_p->Parsables_p[i], reset);
                } 
            }
            break;

        case NH_JS_PARSABLE_BINARY_EXPRESSION :
            {
                Nh_JS_BinaryExpression *Binary_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(&Binary_p->Left, reset);
                Nh_JS_updateVariablesRecursively(&Binary_p->Right, reset);
            }
            break;

        default: break;
    }

NH_END(NH_SUCCESS)
}

// GET =============================================================================================

Nh_JS_Variable *Nh_JS_getVariable(
    Nh_List *Variables_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(Variables_p, index))
}

Nh_JS_Variable *Nh_JS_getVariableFromName(
    Nh_JS_Parsable *Parsable_p, char *name_p)
{
NH_BEGIN()

    Nh_JS_BlockStatement *Container_p = Parsable_p->Container_p;
    
    while (Container_p != NULL)
    {
        for (int i = 0; i < Container_p->Variables.count; ++i) {
            Nh_JS_Variable *Variable_p = Nh_getListItem(&Container_p->Variables, i);
            if (!strcmp(Variable_p->name_p, name_p)) {NH_END(Variable_p)}
        }
        Container_p = Container_p->Parent_p;
    }

NH_END(NULL)
}

