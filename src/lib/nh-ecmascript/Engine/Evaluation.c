// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Evaluation.h"
#include "ExecutionContext.h"
#include "CompletionOrReference.h"

#include "../StaticSemantics/Misc.h"

#include <string.h>
#include <stdio.h>

// EVALUATE ========================================================================================

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateIdentifierReference(
    nh_ecmascript_ParseNode *Node_p)
{
    return nh_ecmascript_wrapReference(nh_ecmascript_resolveBinding(&((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p->String, NULL));
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateNullLiteral(
    nh_ecmascript_ParseNode *Node_p)
{
    return nh_ecmascript_wrapCompletion(nh_ecmascript_normalCompletion(nh_ecmascript_wrapNull()));
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateBooleanLiteral(
    nh_ecmascript_ParseNode *Node_p)
{
    if (!strcmp("true", ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p->String.p)) {
        return nh_ecmascript_wrapCompletion(nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(true)));
    }
    else if (!strcmp("false", ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p->String.p)) {
        return nh_ecmascript_wrapCompletion(nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(false)));
    }

    return nh_ecmascript_wrapCompletion(nh_ecmascript_throwTypeError());
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateLiteral(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_NULL_LITERAL :
            return nh_ecmascript_evaluateNullLiteral(Node_p->Children.pp[0]);
        case NH_ECMASCRIPT_PARSE_NODE_BOOLEAN_LITERAL :
            return nh_ecmascript_evaluateBooleanLiteral(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluatePrimaryExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER_REFERENCE :
            return nh_ecmascript_evaluateIdentifierReference(Node_p->Children.pp[0]);
        case NH_ECMASCRIPT_PARSE_NODE_LITERAL :
            return nh_ecmascript_evaluateLiteral(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateMemberExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_PRIMARY_EXPRESSION) {
        return nh_ecmascript_evaluatePrimaryExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateNewExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_MEMBER_EXPRESSION) {
        return nh_ecmascript_evaluateMemberExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateOptionalExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    return nh_ecmascript_normalEmptyCompletionOrReference();
}

// https://tc39.es/ecma262/#sec-function-calls
static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateCallExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1)
    {
        if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_COVER_CALL_EXPRESSION_AND_ASYNC_ARROW_HEAD) {
        }
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateLeftHandSideExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_NEW_EXPRESSION :
            return nh_ecmascript_evaluateNewExpression(Node_p->Children.pp[0]);
            break;
        case NH_ECMASCRIPT_PARSE_NODE_CALL_EXPRESSION :
            return nh_ecmascript_evaluateCallExpression(Node_p->Children.pp[0]);
            break;
        case NH_ECMASCRIPT_PARSE_NODE_OPTIONAL_EXPRESSION :
            return nh_ecmascript_evaluateOptionalExpression(Node_p->Children.pp[0]);
            break;
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateUpdateExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateLeftHandSideExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateUnaryExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_UPDATE_EXPRESSION) {
        return nh_ecmascript_evaluateUpdateExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateExponentiationExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateUnaryExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateMultiplicativeExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateExponentiationExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateAdditiveExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateMultiplicativeExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateShiftExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateAdditiveExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateRelationalExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateShiftExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateEqualityExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateRelationalExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateBitwiseANDExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateEqualityExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateBitwiseXORExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateBitwiseANDExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateBitwiseORExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateBitwiseXORExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateLogicalANDExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateBitwiseORExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateLogicalORExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateLogicalANDExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateShortCircuitExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type) 
    {
        case NH_ECMASCRIPT_PARSE_NODE_LOGICAL_OR_EXPRESSION :
            return nh_ecmascript_evaluateLogicalORExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateConditionalExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateShortCircuitExpression(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateAssignmentExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 1)
    {
        switch (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type)
        {
            case NH_ECMASCRIPT_PARSE_NODE_CONDITIONAL_EXPRESSION :
                return nh_ecmascript_evaluateConditionalExpression(Node_p->Children.pp[0]);
        }
    }
    else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[1])->type != NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_OPERATOR) {

        if (!strcmp("=", ((nh_ecmascript_ParseNode*)Node_p->Children.pp[1])->Value_p->String.p)) 
        {
//            // TODO If LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral, then
//            nh_ecmascript_CompletionOrReference Left = nh_ecmascript_evaluateLeftHandSideExpression(Node_p->Children.pp[0]);
//            if (Left.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {return Left;}
//            if (nh_ecmascript_isAnonymousFunctionDefinition(Node_p->Children.pp[2]) && nh_ecmascript_isIdentifierRef(Node_p->Children.pp[0])) {
//
//            }
        }
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateInitializer(
    nh_ecmascript_ParseNode *Node_p)
{
    return nh_ecmascript_evaluateAssignmentExpression(Node_p->Children.pp[1]);
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateVariableDeclaration(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 2 && ((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) 
    {
        nh_encoding_UTF8String *BindingId_p = nh_ecmascript_getStringValue(Node_p->Children.pp[0]); 
        nh_ecmascript_Reference Reference = nh_ecmascript_resolveBinding(BindingId_p, NULL);

        nh_ecmascript_Completion InitializerGetValue = nh_ecmascript_normalEmptyCompletion();

        if (nh_ecmascript_isAnonymousFunctionDefinition(Node_p->Children.pp[1])) {
            // TODO
        }
        else {
            nh_ecmascript_CompletionOrReference InitializerEval = nh_ecmascript_evaluateInitializer(Node_p->Children.pp[1]);
            if (InitializerEval.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {
                return InitializerEval;
            }

            InitializerGetValue = nh_ecmascript_getValue(InitializerEval.Completion, InitializerEval.Reference);

            if (InitializerGetValue.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {
                return nh_ecmascript_wrapCompletion(InitializerGetValue);
            }
        }

        nh_ecmascript_putValue(Reference, InitializerGetValue.Value.Payload);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateVariableDeclarationList(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 3)
    {
        nh_ecmascript_CompletionOrReference Result = nh_ecmascript_evaluateVariableDeclarationList(Node_p->Children.pp[0]);
        if (Result.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {return Result;}
        return nh_ecmascript_evaluateVariableDeclaration(Node_p->Children.pp[2]);
    }
    else {
        return nh_ecmascript_evaluateVariableDeclaration(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateVariableStatement(
    nh_ecmascript_ParseNode *Node_p)
{
    nh_ecmascript_CompletionOrReference Result = nh_ecmascript_evaluateVariableDeclarationList(Node_p->Children.pp[1]);
    if (Result.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {return Result;}

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 3) 
    {
        nh_ecmascript_CompletionOrReference Left = nh_ecmascript_evaluateExpression(Node_p->Children.pp[0]);
        if (Left.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {return Left;}
        nh_ecmascript_CompletionOrReference Right = nh_ecmascript_evaluateAssignmentExpression(Node_p->Children.pp[2]);
        return nh_ecmascript_wrapCompletion(nh_ecmascript_getValue(Right.Completion, Right.Reference));
    }

    return nh_ecmascript_evaluateAssignmentExpression(Node_p->Children.pp[0]);
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateExpressionStatement(
    nh_ecmascript_ParseNode *Node_p)
{
    nh_ecmascript_CompletionOrReference ExpressionResult = nh_ecmascript_evaluateExpression(Node_p->Children.pp[0]);

    return nh_ecmascript_wrapCompletion(nh_ecmascript_getValue(ExpressionResult.Completion, ExpressionResult.Reference));
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateStatement(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT : 
            return nh_ecmascript_evaluateVariableStatement(Node_p->Children.pp[0]);
        case NH_ECMASCRIPT_PARSE_NODE_EXPRESSION_STATEMENT : 
            return nh_ecmascript_evaluateExpressionStatement(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateDeclaration(
    nh_ecmascript_ParseNode *Node_p)
{
    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateStatementListItem(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_STATEMENT : 
            return nh_ecmascript_evaluateStatement(Node_p->Children.pp[0]);
        case NH_ECMASCRIPT_PARSE_NODE_DECLARATION : 
            return nh_ecmascript_evaluateDeclaration(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

static nh_ecmascript_CompletionOrReference nh_ecmascript_evaluateStatementList(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size == 2) 
    {
        nh_ecmascript_CompletionOrReference StatementList = nh_ecmascript_evaluateStatementList(Node_p->Children.pp[0]);
        if (StatementList.Completion.type != NH_ECMASCRIPT_COMPLETION_NORMAL) {return StatementList;}

        nh_ecmascript_CompletionOrReference Statement = nh_ecmascript_evaluateStatementListItem(Node_p->Children.pp[1]);
        if (!Statement.Completion.Value.empty) {return Statement;}
        else {
            Statement.Completion.Value = StatementList.Completion.Value;
            return Statement;
        }
    }
    else if (Node_p->Children.size == 1) {
        return nh_ecmascript_evaluateStatementListItem(Node_p->Children.pp[0]);
    }

    return nh_ecmascript_normalEmptyCompletionOrReference();
}

nh_ecmascript_Completion nh_ecmascript_evaluateScriptBody(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p->Children.size > 0) {
        return nh_ecmascript_evaluateStatementList(Node_p->Children.pp[0]).Completion;
    }

    return nh_ecmascript_normalEmptyCompletion();
}

