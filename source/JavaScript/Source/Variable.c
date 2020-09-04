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

#include "../../Core/Header/Memory.h"

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG
#include NH_JS_UTILS

#include <string.h>

// DECLARE =========================================================================================

static void Nh_JS_initVariable(
    Nh_JS_Variable *Variable_p, char *name_p, int depth, NH_JS_KEYWORD kind
);
static void Nh_JS_resetVariableRecursively(
    Nh_JS_Parsable *Parsable_p
);
static void Nh_JS_resetVariable(
    Nh_JS_Variable *Variable_p
);

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

// RESET ===========================================================================================

void Nh_JS_resetScriptVariables(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    Nh_JS_Parsable Parsable = {
        .type   = NH_JS_PARSABLE_BLOCK_STATEMENT, 
        .Data_p = &Script_p->Container,
    };
    Nh_JS_resetVariableRecursively(&Parsable);

NH_SILENT_END()
}

void Nh_JS_resetBlockVariables(
    Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    if (Parsable_p->type != NH_JS_PARSABLE_BLOCK_STATEMENT) {NH_SILENT_END()}

    Nh_JS_BlockStatement *Block_p = Parsable_p->Data_p;

    for (int i = 0; i < Block_p->Variables.count; ++i) {
        Nh_JS_Variable *Variable_p = Nh_getListItem(&Block_p->Variables, i);
        Nh_JS_resetVariable(Variable_p);
    }
    
NH_SILENT_END()
}

static void Nh_JS_resetVariableRecursively(
    Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    switch (Parsable_p->type) 
    {
        case NH_JS_PARSABLE_ASSIGNMENT_EXPRESSION : 
            {
                Nh_JS_AssignmentExpression *AssignmentExpression_p = Parsable_p->Data_p;               
                Nh_JS_resetVariableRecursively(&AssignmentExpression_p->Left);
                Nh_JS_resetVariableRecursively(&AssignmentExpression_p->Right);
            }
            break;

        case NH_JS_PARSABLE_EXPRESSION_STATEMENT : 
            {
                Nh_JS_ExpressionStatement *ExpressionStatement_p = Parsable_p->Data_p;
                Nh_JS_resetVariableRecursively(&ExpressionStatement_p->Parsable);
            }
            break;

        case NH_JS_PARSABLE_BLOCK_STATEMENT :
            {
                Nh_JS_BlockStatement *BlockStatement_p = Parsable_p->Data_p;
                for (int i = 0; i < BlockStatement_p->parsableCount; ++i) {
                    Nh_JS_resetVariableRecursively(&BlockStatement_p->Parsables_p[i]);
                }
                Nh_JS_resetBlockVariables(Parsable_p);
            } 
            break;

        case NH_JS_PARSABLE_MEMBER_EXPRESSION : 
            {
                Nh_JS_MemberExpression *MemberExpression_p = Parsable_p->Data_p;
                Nh_JS_resetVariableRecursively(&MemberExpression_p->Left);
                Nh_JS_resetVariableRecursively(&MemberExpression_p->Right);
            }
            break;

        case NH_JS_PARSABLE_STRING_LITERAL : 
            break;

        case NH_JS_PARSABLE_CALL_EXPRESSION :
            {
                Nh_JS_CallExpression *CallExpression_p = Parsable_p->Data_p;
                for (int i = 0; i < CallExpression_p->argumentCount; ++i) {
                    Nh_JS_resetVariableRecursively(&CallExpression_p->Arguments_p[i]);
                }
                Nh_JS_resetVariableRecursively(&CallExpression_p->Parsable);
            }
            break;

        case NH_JS_PARSABLE_IDENTIFIER : 
            break;
        
        case NH_JS_PARSABLE_FUNCTION_DECLARATION : 
            {
                Nh_JS_FunctionDeclaration *Function_p = Parsable_p->Data_p;
                Nh_JS_resetVariableRecursively(&Function_p->Name);
                for (int i = 0; i < Function_p->parameterCount; ++i) {
                    Nh_JS_resetVariableRecursively(&Function_p->Parameters_p[i]);
                }
                Nh_JS_resetVariableRecursively(&Function_p->Block);
            }
            break;

        case NH_JS_PARSABLE_VARIABLE_DECLARATION :  
            {
                Nh_JS_VariableDeclaration *Declaration_p = Parsable_p->Data_p;
                Nh_JS_resetVariableRecursively(&Declaration_p->Declarator.Identifier);
                Nh_JS_resetVariableRecursively(&Declaration_p->Declarator.Parsable);
            }
            break;

        case NH_JS_PARSABLE_FOR_STATEMENT :
            {
                Nh_JS_ForStatement *For_p = Parsable_p->Data_p;
                for (int i = 0; i < For_p->expressionCount; ++i) {
                    Nh_JS_resetVariableRecursively(&For_p->Expressions_p[i]);
                } 
                Nh_JS_resetVariableRecursively(&For_p->Statement);
            }
            break;

        case NH_JS_PARSABLE_IF_STATEMENT :
            {
                Nh_JS_IfStatement *If_p = Parsable_p->Data_p;
                for (int i = 0; i < If_p->parsableCount; ++i) {
                    Nh_JS_resetVariableRecursively(&If_p->Parsables_p[i]);
                } 
            }
            break;

        case NH_JS_PARSABLE_BINARY_EXPRESSION :
            {
                Nh_JS_BinaryExpression *Binary_p = Parsable_p->Data_p;
                Nh_JS_resetVariableRecursively(&Binary_p->Left);
                Nh_JS_resetVariableRecursively(&Binary_p->Right);
            }
            break;

        default: break;
    }

NH_SILENT_END()
}

static void Nh_JS_resetVariable(
    Nh_JS_Variable *Variable_p)
{
NH_BEGIN()

    if (!Variable_p->freeData) {NH_SILENT_END()}

    Nh_JS_free(Variable_p->type, Variable_p->data_p);
    Nh_JS_initVariable(Variable_p, Variable_p->name_p, Variable_p->depth, Variable_p->kind);

NH_SILENT_END()
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

// INIT ============================================================================================

static void Nh_JS_initVariable(
    Nh_JS_Variable *Variable_p, char *name_p, int depth, NH_JS_KEYWORD kind)
{
NH_BEGIN()

    Variable_p->depth    = depth;
    Variable_p->type     = -1; // note: type deduction happens at interpreter level 
    Variable_p->data_p   = NULL;   
    Variable_p->name_p   = name_p;
    Variable_p->kind     = kind;
    Variable_p->freeData = false; 

NH_SILENT_END()
}

