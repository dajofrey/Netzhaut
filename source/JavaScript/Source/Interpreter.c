// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Interpreter.h"
#include "../Header/Macros.h"
#include "../Header/Variable.h"
#include "../Header/Result.h"

#include "../DOM/Header/Document.h"
#include "../DOM/Header/HTMLCollection.h"
#include "../Core/Header/Function.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/List.h"
#include "../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_JS_UTILS

#include <string.h>

// DEFINE ==========================================================================================

#define INTERPRET(Parsable)                                                     \
    Parsable.type == NH_JS_PARSABLE_FUNCTION_DECLARATION || Parsable.type == -1 \
    ? Nh_JS_getNULLResult()                                                     \
    : ((NH_JS_InterpreterPointer)Parsable.Interpreter_p)(Script_p, &Parsable)

#define FREE(Foo)                                    \
    do {                                             \
        Nh_JS_Result Bar = Foo;                      \
        if (Bar.freeData) {Nh_JS_resetResult(&Bar);} \
    } while (0);

// DECLARE =========================================================================================

static Nh_JS_Object *Nh_JS_getFunctionObject(
    Nh_JS_Object *Owner_p, char *functionName_p
);

static Nh_JS_Result Nh_JS_interpretBlockStatement(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretExpressionStatement(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretCallExpression(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretMemberExpression(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretStringLiteral(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretVariableDeclaration(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretIdentifier(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretForStatement(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretIfStatement(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretAssignmentExpression(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretBinaryExpression(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretBooleanPrimitive(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretNumericLiteral(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);
static Nh_JS_Result Nh_JS_interpretConstructorCall(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p
);

// DATA ============================================================================================

NH_JS_InterpreterPointer Interpreter_pp[22] = 
{
    NULL,
    NULL,
    (NH_JS_InterpreterPointer)Nh_JS_interpretMemberExpression,
    (NH_JS_InterpreterPointer)Nh_JS_interpretAssignmentExpression,
    (NH_JS_InterpreterPointer)Nh_JS_interpretExpressionStatement,
    (NH_JS_InterpreterPointer)Nh_JS_interpretCallExpression,
    (NH_JS_InterpreterPointer)Nh_JS_interpretIdentifier,
    (NH_JS_InterpreterPointer)Nh_JS_interpretStringLiteral,
    (NH_JS_InterpreterPointer)Nh_JS_interpretNumericLiteral,
    (NH_JS_InterpreterPointer)Nh_JS_interpretBlockStatement,
    NULL,
    (NH_JS_InterpreterPointer)Nh_JS_interpretVariableDeclaration,
    (NH_JS_InterpreterPointer)Nh_JS_interpretBinaryExpression,
    NULL,
    (NH_JS_InterpreterPointer)Nh_JS_interpretForStatement,
    NULL,
    (NH_JS_InterpreterPointer)Nh_JS_interpretIfStatement,
    NULL,
    NULL,
    (NH_JS_InterpreterPointer)Nh_JS_interpretBooleanPrimitive,
    (NH_JS_InterpreterPointer)Nh_JS_interpretBooleanPrimitive,
    (NH_JS_InterpreterPointer)Nh_JS_interpretConstructorCall,
};

// INTERPRETER API =================================================================================

NH_RESULT Nh_JS_interpretScript(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    if (Script_p->Flags.interpreted || !Script_p->Flags.loaded) {NH_END(NH_ERROR_BAD_STATE)}

    Script_p->Run.callPending         = false;
    Script_p->Run.unresolvedArguments = false;

    for (int i = 0; i < Script_p->Container.parsableCount; ++i) {FREE(INTERPRET(Script_p->Container.Parsables_p[i]))}

    Script_p->Flags.interpreted = true;

NH_END(NH_SUCCESS)
}

#include NH_CUSTOM_CHECK

Nh_JS_Result Nh_JS_interpretFunction(
    Nh_JS_Script *Script_p, Nh_JS_Object *Object_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (!Script_p->Flags.loaded) {NH_END(Nh_JS_getNULLResult())}
    if (Object_p->type != NH_JS_OBJECT_FUNCTION) {NH_END(Nh_JS_getNULLResult())}
    
    Nh_JS_Function *Function_p = Object_p->data_p;

    if (Function_p->external_p == NULL) {
        NH_END(((Nh_JS_FunctionPointer)Function_p->internal_p)(Script_p, Object_p, aCount, Arguments_p))
        for (int i = 0; i < aCount; ++i) {
            Nh_JS_resetResult(&Arguments_p[i]);
        }
    } 
    else {
        Nh_JS_FunctionDeclaration *Decl_p = Function_p->external_p->Data_p;

        for (int i = 0; i < Decl_p->parameterCount; ++i) 
        {
            Nh_JS_Variable *Variable_p = Nh_JS_getVariableFromName(Function_p->external_p, ((Nh_JS_Identifier*)Decl_p->Parameters_p[i].Data_p)->name_p);
            NH_CHECK_NULL(Nh_JS_getNULLResult(), Variable_p)
            Variable_p->data_p = Arguments_p[i].data_p;
            Variable_p->type   = Arguments_p[i].type;
        }

        NH_END(INTERPRET(Decl_p->Block))

        Nh_JS_resetBlockVariables(&Decl_p->Block);
    }

NH_END(Nh_JS_getNULLResult())
}

// INTERPRETER =====================================================================================

#include NH_JS_DEBUG_INTERPRETER

static Nh_JS_Result Nh_JS_interpretBlockStatement(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_BlockStatement *BlockStatement_p = Parsable_p->Data_p;
    for (int i = 0; i < BlockStatement_p->parsableCount; ++i) {
        FREE(INTERPRET(BlockStatement_p->Parsables_p[i]))
    }

NH_END(Nh_JS_getNULLResult())
}

static Nh_JS_Result Nh_JS_interpretExpressionStatement(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_ExpressionStatement *ExpressionStatement_p = Parsable_p->Data_p;

NH_END(INTERPRET(ExpressionStatement_p->Parsable))
}

static Nh_JS_Result Nh_JS_interpretCallExpression(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_CallExpression *CallExpression_p = Parsable_p->Data_p;

    Script_p->Run.callPending = true;
    Script_p->Run.unresolvedArguments = CallExpression_p->argumentCount > 0;

    Nh_JS_Result Result = Nh_JS_getNULLResult();
    Nh_JS_Result Call = INTERPRET(CallExpression_p->Parsable);

    if (Call.type == NH_JS_TYPE_STRING) // check if constructor call
    {
        for (NH_JS_OBJECT object = 0; object < NH_JS_OBJECT_COUNT; ++object) 
        {
            const char **names_pp = Nh_JS_getFunctionNames(object, NULL);
            if (!strcmp(names_pp[0], Call.data_p)) 
            {
                Call.type = NH_JS_TYPE_OBJECT;
                Call.freeData = false;
                switch (object)
                {
                    case NH_JS_OBJECT_EVENT : Call.data_p = &Script_p->DOM.Objects.Event_p->Functions_p[0]; break;
                } 
                break;
            }
        }
    }

    if (Call.type == NH_JS_TYPE_OBJECT && ((Nh_JS_Object*)Call.data_p)->type == NH_JS_OBJECT_FUNCTION)
    {
        Nh_JS_Result Arguments_p[CallExpression_p->argumentCount];
        for (int i = 0; i < CallExpression_p->argumentCount; ++i) {
            Arguments_p[i] = INTERPRET(CallExpression_p->Arguments_p[i]);
        }

        Result = Nh_JS_interpretFunction(Script_p, Call.data_p, CallExpression_p->argumentCount, Arguments_p);

        for (int i = 0; i < CallExpression_p->argumentCount; ++i) {FREE(Arguments_p[i])}
    } 
    else if (Call.type == NH_JS_TYPE_STRING) // TODO change behavior
    {
        Script_p->Run.callPending = false;
        NH_END(Call)
    }

    Script_p->Run.callPending = false;
    FREE(Call)

NH_END(Result)
}

static Nh_JS_Result Nh_JS_interpretMemberExpression(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    bool unresolvedArguments = Script_p->Run.unresolvedArguments;

    Nh_JS_MemberExpression *MemberExpression_p = Parsable_p->Data_p;

    if (MemberExpression_p->Right.type != NH_JS_PARSABLE_IDENTIFIER) {NH_END(Nh_JS_getNULLResult())}

    Script_p->Run.unresolvedArguments = false;
    Nh_JS_Result Left = INTERPRET(MemberExpression_p->Left);

    if (Left.type == NH_JS_TYPE_OBJECT) 
    {
        Nh_JS_Object *Function_p = Nh_JS_getFunctionObject(
            Left.data_p, ((Nh_JS_Identifier*)MemberExpression_p->Right.Data_p)->name_p
        );

        if (Function_p != NULL && (!Script_p->Run.callPending || !unresolvedArguments)) {
            NH_END(Nh_JS_interpretFunction(Script_p, Function_p, 0, NULL))
        } else if (Function_p != NULL) {
            NH_END(Nh_JS_getResult(NH_JS_TYPE_OBJECT, false, Function_p))
        }
    }

NH_END(Nh_JS_getNULLResult())
}

static Nh_JS_Result Nh_JS_interpretStringLiteral(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()
 
    Nh_JS_StringLiteral *StringLiteral_p = Parsable_p->Data_p;

NH_END(Nh_JS_getResult(NH_JS_TYPE_STRING, false, StringLiteral_p->value_p))
}

static Nh_JS_Result Nh_JS_interpretVariableDeclaration(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_VariableDeclaration *Declaration_p = Parsable_p->Data_p;

    Nh_JS_Result Result = INTERPRET(Declaration_p->Declarator.Parsable);

    if (Result.type != NH_JS_TYPE_NULL) 
    {
        Nh_JS_Variable *Variable_p = Nh_JS_getVariableFromName(Parsable_p, ((Nh_JS_Identifier*)Declaration_p->Declarator.Identifier.Data_p)->name_p);

        Variable_p->data_p     = Result.data_p;
        Variable_p->type       = Result.type;
        Variable_p->freeData   = Result.freeData;
        Variable_p->distinct_p = Variable_p->type == NH_JS_TYPE_OBJECT ? Nh_JS_getDistinction(Script_p, Variable_p->data_p) : NULL;

        if (!Variable_p->freeData && Variable_p->type == NH_JS_TYPE_NUMBER) // dont want to override internal values
        {
            double value = *((double*)Variable_p->data_p);
            Variable_p->data_p = Nh_allocate(sizeof(double));
            NH_CHECK_NULL(Nh_JS_getNULLResult(), Variable_p->data_p);
            *((double*)Variable_p->data_p) = value;
            Variable_p->freeData = true;
        }
    } 

NH_END(Nh_JS_getNULLResult())
}

static Nh_JS_Result Nh_JS_interpretIdentifier(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_Identifier *Identifier_p = Parsable_p->Data_p;
    NH_JS_OBJECT type = -1;
    Nh_JS_Result Result = Nh_JS_getNULLResult();

         if (!strcmp(Identifier_p->name_p, "location"))  {type = NH_JS_OBJECT_LOCATION;} 
    else if (!strcmp(Identifier_p->name_p, "document"))  {type = NH_JS_OBJECT_DOCUMENT;}
    else if (!strcmp(Identifier_p->name_p, "console"))   {type = NH_JS_OBJECT_CONSOLE;}
    else if (!strcmp(Identifier_p->name_p, "nhwindow"))  {type = NH_JS_OBJECT_NH_WINDOW;}
    else if (!strcmp(Identifier_p->name_p, "window"))    {type = NH_JS_OBJECT_WINDOW;}
    else if (!strcmp(Identifier_p->name_p, "nhtab"))     {type = NH_JS_OBJECT_NH_TAB;}
    else if (!strcmp(Identifier_p->name_p, "Math"))      {type = NH_JS_OBJECT_MATH;}

    if (type != -1) 
    {
        Nh_JS_Object *Object_p = NULL;

        switch (type)
        {
            case NH_JS_OBJECT_LOCATION  : Object_p = Script_p->DOM.Objects.Location_p; break;
            case NH_JS_OBJECT_DOCUMENT  : Object_p = Script_p->DOM.Objects.Document_p; break;
            case NH_JS_OBJECT_CONSOLE   : Object_p = Script_p->DOM.Objects.Console_p; break;
            case NH_JS_OBJECT_NH_WINDOW : Object_p = Script_p->DOM.Objects.NhWindow_p; break;
            case NH_JS_OBJECT_WINDOW    : Object_p = Script_p->DOM.Objects.Window_p; break;
            case NH_JS_OBJECT_NH_TAB    : Object_p = Script_p->DOM.Objects.NhTab_p; break;
            case NH_JS_OBJECT_MATH      : Object_p = Script_p->Core.Objects.Math_p; break;

            default : Object_p->data_p = NULL;
        }

        Result.freeData = false;
        Result.type     = NH_JS_TYPE_OBJECT;
        Result.data_p   = Object_p;
    }
    else if (Nh_JS_getVariableFromName(Parsable_p, Identifier_p->name_p) != NULL) 
    {
        Nh_JS_Variable *Variable_p = Nh_JS_getVariableFromName(Parsable_p, Identifier_p->name_p);

        Result.freeData = false;
        Result.type     = Variable_p->type;
        Result.data_p   = Variable_p->data_p;
    } 
    else 
    {
        Result.freeData = false;
        Result.type     = NH_JS_TYPE_STRING;
        Result.data_p   = Identifier_p->name_p;
    }

NH_END(Result)
}

static Nh_JS_Result Nh_JS_interpretForStatement(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_ForStatement *ForStatement_p = Parsable_p->Data_p;

    switch (ForStatement_p->type)
    {
        case NH_JS_ITERATION_STATEMENT_FOR          : break; 
        case NH_JS_ITERATION_STATEMENT_FOR_IN       : break;
        case NH_JS_ITERATION_STATEMENT_FOR_AWAIT_OF : break;
        case NH_JS_ITERATION_STATEMENT_FOR_OF       : 

            if (ForStatement_p->expressionCount != 2) {NH_END(Nh_JS_getNULLResult())}
            if (ForStatement_p->Expressions_p[0].type != NH_JS_PARSABLE_VARIABLE_DECLARATION) {NH_END(Nh_JS_getNULLResult())}
            Nh_JS_VariableDeclaration *Declaration_p = ForStatement_p->Expressions_p[0].Data_p;

            Nh_JS_Result Result = INTERPRET(ForStatement_p->Expressions_p[0]);
            if (Result.type != NH_JS_TYPE_NULL) {NH_END(Nh_JS_getNULLResult())}

            Result = INTERPRET(ForStatement_p->Expressions_p[1]);
            Nh_JS_Variable *Variable_p = Nh_JS_getVariableFromName(
                Parsable_p, ((Nh_JS_Identifier*)Declaration_p->Declarator.Identifier.Data_p)->name_p
            );
 
            NH_CHECK_NULL(Nh_JS_getNULLResult(), Variable_p)
            if (Result.type != NH_JS_TYPE_OBJECT) {FREE(Result) NH_END(Nh_JS_getNULLResult())}

            Variable_p->depth += 1;
            Nh_JS_Object *Object_p = Result.data_p;

            switch (Object_p->type)
            {
                case NH_JS_OBJECT_HTML_COLLECTION : 
                {
                    Nh_JS_HTMLCollection *Collection_p = Object_p->data_p;
                    for (int i = 0; i < Collection_p->List.count; ++i) 
                    {
                        Variable_p->type       = NH_JS_TYPE_OBJECT;
                        Variable_p->data_p     = Nh_getListItem(&Collection_p->List, i);
                        Variable_p->distinct_p = Nh_JS_getDistinction(Script_p, Variable_p->data_p);
                        FREE(INTERPRET(ForStatement_p->Statement))
                    }
                    break;
                }

                default : NH_END(Nh_JS_getNULLResult())
            }

            break;
    }

NH_END(Nh_JS_getNULLResult())
}

static Nh_JS_Result Nh_JS_interpretIfStatement(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_IfStatement *IfStatement_p = Parsable_p->Data_p;
    if (IfStatement_p->parsableCount <= 0) {NH_END(Nh_JS_getNULLResult())}

    Nh_JS_Result Expression = INTERPRET(IfStatement_p->Parsables_p[0]);

    if (NH_JS_TRUE(Expression.type)) {
        FREE(INTERPRET(IfStatement_p->Parsables_p[1]))
    } 

    FREE(Expression);

NH_END(Nh_JS_getNULLResult())
}

static Nh_JS_Result Nh_JS_interpretAssignmentExpression(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_AssignmentExpression *Assignment_p = Parsable_p->Data_p;
    Nh_JS_Result Right = INTERPRET(Assignment_p->Right);

    Script_p->Run.callPending = Script_p->Run.unresolvedArguments = true;
    Nh_JS_Result Left = INTERPRET(Assignment_p->Left);
    Script_p->Run.callPending = Script_p->Run.unresolvedArguments = false;

    if (Left.type == NH_JS_TYPE_OBJECT && ((Nh_JS_Object*)Left.data_p)->type == NH_JS_OBJECT_FUNCTION) 
    {
        FREE(Nh_JS_interpretFunction(Script_p, Left.data_p, 1, &Right))
    }
    else if (Left.type == NH_JS_TYPE_NUMBER && Right.type == NH_JS_TYPE_NUMBER) 
    {
        switch (Assignment_p->operator_)
        {
            case NH_JS_OPERATOR_SUBTRACT_ASSIGN : *((double*)Left.data_p) -= *((double*)Right.data_p); break;
            case NH_JS_OPERATOR_ADD_ASSIGN      : *((double*)Left.data_p) += *((double*)Right.data_p); break;
            case NH_JS_OPERATOR_MULTIPLY_ASSIGN : *((double*)Left.data_p) *= *((double*)Right.data_p); break;
            case NH_JS_OPERATOR_DIVIDE_ASSIGN   : *((double*)Left.data_p) /= *((double*)Right.data_p); break;
        }
    }

    FREE(Right)
    FREE(Left)

NH_END(Nh_JS_getNULLResult())
}

static Nh_JS_Result Nh_JS_interpretBinaryExpression(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_BinaryExpression *Binary_p = Parsable_p->Data_p;
    Nh_JS_Result Left   = INTERPRET(Binary_p->Left);
    Nh_JS_Result Right  = INTERPRET(Binary_p->Right);
    Nh_JS_Result Result = Nh_JS_getNULLResult();

    bool twoBools = NH_JS_IS_BOOL(Left.type) && NH_JS_IS_BOOL(Right.type);

    if (!twoBools && Left.type != Right.type) {
        FREE(Right) FREE(Left)
        NH_END(Result)
    }
    NH_JS_TYPE type = Left.type;

    switch (Binary_p->operator_)
    {
        case NH_JS_OPERATOR_EQUAL   :
        case NH_JS_OPERATOR_UNEQUAL :
        {
            if (twoBools) 
            {
                if (Left.type == Right.type) {Result.type = Binary_p->operator_ == NH_JS_OPERATOR_EQUAL   ? NH_JS_TYPE_BOOLEAN_TRUE : NH_JS_TYPE_BOOLEAN_FALSE;}
                else                         {Result.type = Binary_p->operator_ == NH_JS_OPERATOR_UNEQUAL ? NH_JS_TYPE_BOOLEAN_TRUE : NH_JS_TYPE_BOOLEAN_FALSE;}
            }
            else if (type == NH_JS_TYPE_STRING) 
            {
                if (!strcmp(Left.data_p, Right.data_p)) {Result.type = Binary_p->operator_ == NH_JS_OPERATOR_EQUAL   ? NH_JS_TYPE_BOOLEAN_TRUE : NH_JS_TYPE_BOOLEAN_FALSE;}
                else                                    {Result.type = Binary_p->operator_ == NH_JS_OPERATOR_UNEQUAL ? NH_JS_TYPE_BOOLEAN_TRUE : NH_JS_TYPE_BOOLEAN_FALSE;}
            }
            break;
        }
        case NH_JS_OPERATOR_AND               :
        case NH_JS_OPERATOR_OR                :
        case NH_JS_OPERATOR_SHORT_CIRCUIT_AND :
        case NH_JS_OPERATOR_SHORT_CIRCUIT_OR  :
        {
            if (twoBools) 
            {
                switch (Binary_p->operator_)
                {
                    case NH_JS_OPERATOR_SHORT_CIRCUIT_AND :
                        if (NH_JS_TRUE(Left.type) && NH_JS_TRUE(Right.type)) {Result.type = NH_JS_TYPE_BOOLEAN_TRUE;}
                        else                                                 {Result.type = NH_JS_TYPE_BOOLEAN_FALSE;}
                        break;
                    case NH_JS_OPERATOR_SHORT_CIRCUIT_OR  :
                        if (NH_JS_TRUE(Left.type) || NH_JS_TRUE(Right.type)) {Result.type = NH_JS_TYPE_BOOLEAN_TRUE;}
                        else                                                 {Result.type = NH_JS_TYPE_BOOLEAN_FALSE;}
                        break;
                }
            }
            break;
        }
        case NH_JS_OPERATOR_GREATER       :
        case NH_JS_OPERATOR_LESS          :
        case NH_JS_OPERATOR_GREATER_EQUAL :
        case NH_JS_OPERATOR_LESS_EQUAL    :
        {
            if (type == NH_JS_TYPE_NUMBER) 
            {
                switch (Binary_p->operator_)
                {
                    case NH_JS_OPERATOR_GREATER       : 
                        if (*((double*)Left.data_p) > *((double*)Right.data_p)) {Result.type = NH_JS_TYPE_BOOLEAN_TRUE;}
                        else                                                    {Result.type = NH_JS_TYPE_BOOLEAN_FALSE;}
                        break;
                    case NH_JS_OPERATOR_LESS          : 
                        if (*((double*)Left.data_p) < *((double*)Right.data_p))  {Result.type = NH_JS_TYPE_BOOLEAN_TRUE;}
                        else                                                     {Result.type = NH_JS_TYPE_BOOLEAN_FALSE;}
                        break;
                    case NH_JS_OPERATOR_GREATER_EQUAL : 
                        if (*((double*)Left.data_p) >= *((double*)Right.data_p)) {Result.type = NH_JS_TYPE_BOOLEAN_TRUE;}
                        else                                                     {Result.type = NH_JS_TYPE_BOOLEAN_FALSE;}
                        break;
                    case NH_JS_OPERATOR_LESS_EQUAL    : 
                        if (*((double*)Left.data_p) <= *((double*)Right.data_p)) {Result.type = NH_JS_TYPE_BOOLEAN_TRUE;}
                        else                                                     {Result.type = NH_JS_TYPE_BOOLEAN_FALSE;}
                        break;
                }
            }
            break;
        }
        case NH_JS_OPERATOR_ADD      :
        case NH_JS_OPERATOR_SUBTRACT :
        case NH_JS_OPERATOR_MULTIPLY :
        case NH_JS_OPERATOR_DIVIDE   :
        {
            if (type == NH_JS_TYPE_NUMBER) 
            {
                Result.type     = NH_JS_TYPE_NUMBER;
                Result.freeData = true;
                Result.data_p   = Nh_allocate(sizeof(double));
                NH_CHECK_NULL(Nh_JS_getNULLResult(), Result.data_p)

                switch (Binary_p->operator_)
                {
                    case NH_JS_OPERATOR_ADD      : *((double*)Result.data_p) = *((double*)Left.data_p) + *((double*)Right.data_p); break;
                    case NH_JS_OPERATOR_SUBTRACT : *((double*)Result.data_p) = *((double*)Left.data_p) - *((double*)Right.data_p); break;
                    case NH_JS_OPERATOR_MULTIPLY : *((double*)Result.data_p) = *((double*)Left.data_p) * *((double*)Right.data_p); break;
                    case NH_JS_OPERATOR_DIVIDE   : *((double*)Result.data_p) = *((double*)Left.data_p) / *((double*)Right.data_p); break;
                }
            }
            break;
        }

        default : break; 
    }

    FREE(Left)
    FREE(Right)

#undef ALLOCATE_RESULT

NH_END(Result)
}

static Nh_JS_Result Nh_JS_interpretBooleanPrimitive(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_Result Result;

    Result.freeData = false;
    Result.data_p   = NULL;

    switch (Parsable_p->type)
    {
        case NH_JS_PARSABLE_BOOLEAN_TRUE  : Result.type = NH_JS_TYPE_BOOLEAN_TRUE; break;        
        case NH_JS_PARSABLE_BOOLEAN_FALSE : Result.type = NH_JS_TYPE_BOOLEAN_FALSE; break;
    } 

NH_END(Result)
}

static Nh_JS_Result Nh_JS_interpretNumericLiteral(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    Nh_JS_Result Result;

    Result.data_p = Nh_allocate(sizeof(double));
    NH_CHECK_NULL(Nh_JS_getNULLResult(), Result.data_p)

    Result.freeData = true;
    Result.type     = NH_JS_TYPE_NUMBER;
    *((double*)Result.data_p) = *((double*)Parsable_p->Data_p);

NH_END(Result)
}

static Nh_JS_Result Nh_JS_interpretConstructorCall(
    Nh_JS_Script *Script_p, Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

NH_END(INTERPRET(((Nh_JS_ConstructorCall*)Parsable_p->Data_p)->CallExpression))
}

// GET =============================================================================================

#include NH_DEBUG

NH_JS_InterpreterPointer Nh_JS_getInterpreterPointer(
    int type)
{
NH_BEGIN()

    if (type < 0) {NH_END(NULL)}
 
NH_END(Interpreter_pp[type])
}

static Nh_JS_Object *Nh_JS_getFunctionObject(
    Nh_JS_Object *Owner_p, char *functionName_p)
{
NH_BEGIN()

    while (Owner_p != NULL)
    {
        Nh_HashValue *HashValue_p;
        if (hashmap_get(NH_HASHMAPS.JS.Functions_p[Owner_p->type], functionName_p, (void**)(&HashValue_p)) == MAP_OK) {
            NH_END(&Owner_p->Functions_p[HashValue_p->number])
        }
        Owner_p = Owner_p->Inherit_p;
    }

NH_END(NULL)
}

