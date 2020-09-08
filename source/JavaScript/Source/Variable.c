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
#include "../DOM/Header/HTMLElement.h"
#include "../DOM/Header/Document.h"
#include "../DOM/Header/EventListener.h"

#include "../../Core/Header/Memory.h"

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG
#include NH_JS_UTILS

#include <string.h>
#include <stdio.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_JS_updateVariablesRecursively(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p, NH_BOOL reset
);
static NH_RESULT Nh_JS_updateVariable(
    Nh_JS_Script *Script_p, Nh_JS_Variable *Variable_p
);

// INIT ============================================================================================

void Nh_JS_initVariable(
    Nh_JS_Variable *Variable_p, char *name_p, int depth, NH_JS_KEYWORD kind)
{
NH_BEGIN()

    Variable_p->depth      = depth;
    Variable_p->type       = -1; // NOTE type deduction happens at interpreter level 
    Variable_p->data_p     = NULL;   
    Variable_p->name_p     = name_p;
    Variable_p->kind       = kind;
    Variable_p->freeData   = false; 
    Variable_p->distinct_p = NULL;

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
    NH_CHECK(Nh_JS_updateVariablesRecursively(Script_p, &Parsable, NH_FALSE))

    for (int i = 0; i < Script_p->DOM.EventListeners.count; ++i) {
        Nh_JS_Object *Object_p = Nh_getListItem(&Script_p->DOM.EventListeners, i);
        Nh_JS_EventListener *Listener_p = Object_p->data_p;
        NH_CHECK(Nh_JS_updateVariable(Script_p, &Listener_p->Target))
    }

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
    Nh_JS_updateVariablesRecursively(Script_p, &Parsable, NH_TRUE);

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
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    if (Parsable_p->type != NH_JS_PARSABLE_BLOCK_STATEMENT) {NH_END(NH_ERROR_BAD_STATE)}
    Nh_JS_BlockStatement *Block_p = Parsable_p->Data_p;

    for (int i = 0; i < Block_p->Variables.count; ++i) {
        NH_CHECK(Nh_JS_updateVariable(Script_p, Nh_getListItem(&Block_p->Variables, i)))
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_updateVariable(
    Nh_JS_Script *Script_p, Nh_JS_Variable *Variable_p)
{
NH_BEGIN()

    switch (Variable_p->type) 
    {
        case NH_JS_TYPE_OBJECT :
        {
            if (Variable_p->distinct_p != NULL) 
            {
                Nh_JS_Document *Document_p = Script_p->DOM.Objects.Document_p->data_p;
                for (int i = 0; i < Document_p->Tree.Flat.count; ++i) 
                {
                    Nh_JS_Object *Object_p = Nh_getListItem(&Document_p->Tree.Flat, i);
                    Nh_JS_HTMLElement *Element_p = Object_p->data_p;

                    if (Element_p->Node_p == Variable_p->distinct_p) {
                        Variable_p->data_p = Object_p; 
                        NH_END(NH_SUCCESS)
                    }
                }
            }

            Nh_JS_Object *Object_p = Nh_JS_getObject(Variable_p->data_p, NH_JS_OBJECT_HTML_COLLECTION);
            if (Object_p != NULL) {
                NH_END(Nh_JS_updateHTMLCollection(Object_p->data_p))
            }

            break;
        }
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_updateVariablesRecursively(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p, NH_BOOL reset)
{
NH_BEGIN()

    switch (Parsable_p->type) 
    {
        case NH_JS_PARSABLE_ASSIGNMENT_EXPRESSION : 
            {
                Nh_JS_AssignmentExpression *AssignmentExpression_p = Parsable_p->Data_p;               
                Nh_JS_updateVariablesRecursively(Script_p, &AssignmentExpression_p->Left, reset);
                Nh_JS_updateVariablesRecursively(Script_p, &AssignmentExpression_p->Right, reset);
            }
            break;

        case NH_JS_PARSABLE_EXPRESSION_STATEMENT : 
            {
                Nh_JS_ExpressionStatement *ExpressionStatement_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(Script_p, &ExpressionStatement_p->Parsable, reset);
            }
            break;

        case NH_JS_PARSABLE_BLOCK_STATEMENT :
            {
                Nh_JS_BlockStatement *BlockStatement_p = Parsable_p->Data_p;
                for (int i = 0; i < BlockStatement_p->parsableCount; ++i) {
                    Nh_JS_updateVariablesRecursively(Script_p, &BlockStatement_p->Parsables_p[i], reset);
                }
                if (reset) {Nh_JS_resetBlockVariables(Parsable_p);}
                else {NH_CHECK(Nh_JS_updateBlockVariables(Script_p, Parsable_p))}
            } 
            break;

        case NH_JS_PARSABLE_MEMBER_EXPRESSION : 
            {
                Nh_JS_MemberExpression *MemberExpression_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(Script_p, &MemberExpression_p->Left, reset);
                Nh_JS_updateVariablesRecursively(Script_p, &MemberExpression_p->Right, reset);
            }
            break;

        case NH_JS_PARSABLE_STRING_LITERAL : 
            break;

        case NH_JS_PARSABLE_CALL_EXPRESSION :
            {
                Nh_JS_CallExpression *CallExpression_p = Parsable_p->Data_p;
                for (int i = 0; i < CallExpression_p->argumentCount; ++i) {
                    Nh_JS_updateVariablesRecursively(Script_p, &CallExpression_p->Arguments_p[i], reset);
                }
                Nh_JS_updateVariablesRecursively(Script_p, &CallExpression_p->Parsable, reset);
            }
            break;

        case NH_JS_PARSABLE_IDENTIFIER : 
            break;
        
        case NH_JS_PARSABLE_FUNCTION_DECLARATION : 
            {
                Nh_JS_FunctionDeclaration *Function_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(Script_p, &Function_p->Name, reset);
                for (int i = 0; i < Function_p->parameterCount; ++i) {
                    Nh_JS_updateVariablesRecursively(Script_p, &Function_p->Parameters_p[i], reset);
                }
                Nh_JS_updateVariablesRecursively(Script_p, &Function_p->Block, reset);
            }
            break;

        case NH_JS_PARSABLE_VARIABLE_DECLARATION :  
            {
                Nh_JS_VariableDeclaration *Declaration_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(Script_p, &Declaration_p->Declarator.Identifier, reset);
                Nh_JS_updateVariablesRecursively(Script_p, &Declaration_p->Declarator.Parsable, reset);
            }
            break;

        case NH_JS_PARSABLE_FOR_STATEMENT :
            {
                Nh_JS_ForStatement *For_p = Parsable_p->Data_p;
                for (int i = 0; i < For_p->expressionCount; ++i) {
                    Nh_JS_updateVariablesRecursively(Script_p, &For_p->Expressions_p[i], reset);
                } 
                Nh_JS_updateVariablesRecursively(Script_p, &For_p->Statement, reset);
            }
            break;

        case NH_JS_PARSABLE_IF_STATEMENT :
            {
                Nh_JS_IfStatement *If_p = Parsable_p->Data_p;
                for (int i = 0; i < If_p->parsableCount; ++i) {
                    Nh_JS_updateVariablesRecursively(Script_p, &If_p->Parsables_p[i], reset);
                } 
            }
            break;

        case NH_JS_PARSABLE_BINARY_EXPRESSION :
            {
                Nh_JS_BinaryExpression *Binary_p = Parsable_p->Data_p;
                Nh_JS_updateVariablesRecursively(Script_p, &Binary_p->Left, reset);
                Nh_JS_updateVariablesRecursively(Script_p, &Binary_p->Right, reset);
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

void *Nh_JS_getDistinction(
    Nh_JS_Script *Script_p, Nh_JS_Object *Object_p)
{
NH_BEGIN()

    Nh_JS_Object *Object2_p = Nh_JS_getObject(Object_p, NH_JS_OBJECT_HTML_ELEMENT);
    if (Object2_p != NULL) {NH_END(((Nh_JS_HTMLElement*)Object2_p->data_p)->Node_p)}

    // ...

NH_END(NULL)
}

