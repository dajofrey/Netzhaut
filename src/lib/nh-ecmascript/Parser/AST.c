// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "AST.h"
#include "CST.h"
#include "Lexer.h"
#include "Script.h"

#include "../Common/Log.h"
#include "../Common/IndexMap.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// HELPER FUNCTIONS ================================================================================

static nh_ecmascript_ParseNode *nh_ecmascript_collapseCST(
    const nh_ecmascript_ParseNode *Node_p)
{
    if (!Node_p) return NULL;

    nh_core_List FlatChildren = nh_core_initList(16);

    for (size_t i = 0; i < Node_p->Children.size; ++i) {
        nh_ecmascript_ParseNode *Child_p = Node_p->Children.pp[i];
        nh_ecmascript_ParseNode *FlatChild_p = nh_ecmascript_collapseCST(Child_p);
        if (FlatChild_p)
            nh_core_appendToList(&FlatChildren, FlatChild_p);
    }

    if (FlatChildren.size == 1 && Node_p->Value_p == NULL) {
        nh_ecmascript_ParseNode *Single_p = FlatChildren.pp[0];
        // Only free the list container, not its elements
        nh_core_freeList(&FlatChildren, false);
        return Single_p;
    }

    nh_ecmascript_ParseNode *FlatNode_p = nh_core_allocate(sizeof(nh_ecmascript_ParseNode));
    FlatNode_p->type = Node_p->type;

    // Copy value if it exists (deep copy if necessary)
    if (Node_p->Value_p) {
        FlatNode_p->Value_p = malloc(sizeof(*Node_p->Value_p));
        memcpy(FlatNode_p->Value_p, Node_p->Value_p, sizeof(*Node_p->Value_p));
    } else {
        FlatNode_p->Value_p = NULL;
    }

    FlatNode_p->Children = FlatChildren;

    return FlatNode_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createASTNode(
    NH_ECMASCRIPT_AST_NODE type)
{
    nh_ecmascript_ASTNode *Node_p = (nh_ecmascript_ASTNode*)nh_core_allocate(sizeof(nh_ecmascript_ASTNode));
    Node_p->type = type;
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createIdentifier(
    const char *name_p)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_IDENTIFIER);
    size_t len = strlen(name_p);
    Node_p->Identifier.name_p = (char*)nh_core_allocate(len + 1);
    strcpy(Node_p->Identifier.name_p, name_p);
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createLiteral(
    NH_ECMASCRIPT_AST_LITERAL literal_type, const char *value_p)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_LITERAL);
    Node_p->Literal.type = literal_type;
    
    switch (literal_type) {
        case NH_ECMASCRIPT_AST_LITERAL_NUMBER:
            Node_p->Literal.number = strtod(value_p, NULL);
            break;
        case NH_ECMASCRIPT_AST_LITERAL_STRING:
            {
                size_t len = strlen(value_p);
                Node_p->Literal.string_p = (char*)nh_core_allocate(len + 1);
                strcpy(Node_p->Literal.string_p, value_p);
            }
            break;
        case NH_ECMASCRIPT_AST_LITERAL_BOOL:
            Node_p->Literal.boolean = !strcmp(value_p, "true");
            break;
        case NH_ECMASCRIPT_AST_LITERAL_NULL:
            break;
    }
    
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createMemberExpression(
    nh_ecmascript_ASTNode *object_p, nh_ecmascript_ASTNode *property_p, bool computed)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_MEMBER_EXPRESSION);
    Node_p->MemberExpression.Object_p = object_p;
    Node_p->MemberExpression.Property_p = property_p;
    Node_p->MemberExpression.computed = computed;
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createAssignmentExpression(
    nh_ecmascript_ASTNode *left_p, nh_ecmascript_ASTNode *right_p, const char *operator_p)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_ASSIGNMENT_EXPRESSION);
    Node_p->AssignmentExpression.Left_p = left_p;
    Node_p->AssignmentExpression.Right_p = right_p;
    size_t len = strlen(operator_p);
    Node_p->AssignmentExpression.operator_p = (char*)nh_core_allocate(len + 1);
    strcpy(Node_p->AssignmentExpression.operator_p, operator_p);
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createBinaryExpression(
    nh_ecmascript_ASTNode *left_p, nh_ecmascript_ASTNode *right_p, const char *operator_p)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_BINARY_EXPRESSION);
    Node_p->BinaryExpression.Left_p = left_p;
    Node_p->BinaryExpression.Right_p = right_p;
    size_t len = strlen(operator_p);
    Node_p->BinaryExpression.operator_p = (char*)nh_core_allocate(len + 1);
    strcpy(Node_p->BinaryExpression.operator_p, operator_p);
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createUpdateExpression(
    nh_ecmascript_ASTNode *argument_p, const char *operator_p, bool prefix)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_UPDATE_EXPRESSION);
    Node_p->UpdateExpression.Argument_p = argument_p;
    size_t len = strlen(operator_p);
    Node_p->UpdateExpression.operator_p = (char*)nh_core_allocate(len + 1);
    strcpy(Node_p->UpdateExpression.operator_p, operator_p);
    Node_p->UpdateExpression.prefix = prefix;
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createVariableDeclarator(
    nh_ecmascript_ASTNode *id_p, nh_ecmascript_ASTNode *init_p)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATOR);
    Node_p->VariableDeclarator.Id_p = id_p;
    Node_p->VariableDeclarator.Init_p = init_p;
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createVariableDeclaration(
    const char *kind_p, nh_core_List declarators)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATION);
    size_t len = strlen(kind_p);
    Node_p->VariableDeclaration.kind_p = (char*)nh_core_allocate(len + 1);
    strcpy(Node_p->VariableDeclaration.kind_p, kind_p);
    Node_p->VariableDeclaration.Declarators = declarators;
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createForDeclaration(
    const char *kind_p, nh_core_List declarators)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_FOR_DECLARATION);
    size_t len = strlen(kind_p);
    Node_p->ForDeclaration.kind_p = (char*)nh_core_allocate(len + 1);
    strcpy(Node_p->ForDeclaration.kind_p, kind_p);
    Node_p->ForDeclaration.Declarators = declarators;
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createBlockStatement(
    nh_core_List body)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_BLOCK_STATEMENT);
    Node_p->BlockStatement.Body = body;
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createForStatement(
    nh_ecmascript_ASTNode *init_p, nh_ecmascript_ASTNode *test_p, 
    nh_ecmascript_ASTNode *update_p, nh_ecmascript_ASTNode *body_p)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_FOR_STATEMENT);
    Node_p->ForStatement.Init_p = init_p;
    Node_p->ForStatement.Test_p = test_p;
    Node_p->ForStatement.Update_p = update_p;
    Node_p->ForStatement.Body_p = body_p;
    return Node_p;
}

static nh_ecmascript_ASTNode *nh_ecmascript_createExpressionStatement(
    nh_ecmascript_ASTNode *expression_p)
{
    nh_ecmascript_ASTNode *Node_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_EXPRESSION_STATEMENT);
    Node_p->ExpressionStatement.Expression_p = expression_p;
    return Node_p;
}

// MAIN PARSING FUNCTIONS ==============================================================================

static nh_ecmascript_ASTNode *nh_ecmascript_parseExpression(
    nh_ecmascript_ParseNode *Node_p);

static nh_ecmascript_ASTNode *nh_ecmascript_parseStatement(
    nh_ecmascript_ParseNode *Node_p);

static nh_ecmascript_ASTNode *nh_ecmascript_parseExpression(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p == NULL) {
        return NULL;
    }

    // Handle terminal nodes (tokens)
    if (Node_p->Value_p != NULL) {
        const char *token = Node_p->Value_p->String.p;
        
        // Check if it's a numeric literal
        if (Node_p->Value_p->type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL ||
            Node_p->Value_p->type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL ||
            Node_p->Value_p->type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL ||
            Node_p->Value_p->type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL) {
            return nh_ecmascript_createLiteral(NH_ECMASCRIPT_AST_LITERAL_NUMBER, token);
        }
        
        // Check if it's a string literal
        if (Node_p->Value_p->type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_STRING_LITERAL) {
            return nh_ecmascript_createLiteral(NH_ECMASCRIPT_AST_LITERAL_STRING, token);
        }
        
        // Check if it's a boolean literal
        if (!strcmp(token, "true") || !strcmp(token, "false")) {
            return nh_ecmascript_createLiteral(NH_ECMASCRIPT_AST_LITERAL_BOOL, token);
        }
        
        // Check if it's null
        if (!strcmp(token, "null")) {
            return nh_ecmascript_createLiteral(NH_ECMASCRIPT_AST_LITERAL_NULL, token);
        }
        
        // Check if it's an identifier
        if (Node_p->Value_p->type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME) {
            return nh_ecmascript_createIdentifier(token);
        }
        
        // For other tokens, return NULL (they'll be handled by parent nodes)
        return NULL;
    }

    // Handle non-terminal nodes
    switch (Node_p->type) {
        case NH_ECMASCRIPT_PARSE_NODE_MEMBER_EXPRESSION:
            {
                if (Node_p->Children.size >= 3) {
                    // obj.prop or obj[expr] format
                    nh_ecmascript_ASTNode *object_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                    nh_ecmascript_ASTNode *property_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[2]);
                    
                    // Check if it's computed (bracket notation)
                    bool computed = false;
                    if (Node_p->Children.size > 1) {
                        nh_ecmascript_ParseNode *bracket_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                        if (bracket_p->Value_p != NULL && bracket_p->Value_p->String.p[0] == '[') {
                            computed = true;
                        }
                    }
                    
                    return nh_ecmascript_createMemberExpression(object_p, property_p, computed);
                }
                else if (Node_p->Children.size == 1) {
                    // Simple case - just parse the child
                    return nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                }
                break;
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_ASSIGNMENT_EXPRESSION:
            {
                if (Node_p->Children.size >= 3) {
                    // left operator right format
                    nh_ecmascript_ASTNode *left_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                    nh_ecmascript_ParseNode *operator_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                    nh_ecmascript_ASTNode *right_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[2]);
                    
                    const char *op_str = operator_p->Value_p ? operator_p->Value_p->String.p : "=";
                    return nh_ecmascript_createAssignmentExpression(left_p, right_p, op_str);
                }
                else if (Node_p->Children.size == 1) {
                    // Simple expression case
                    return nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                }
                break;
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_RELATIONAL_EXPRESSION:
            {
                if (Node_p->Children.size >= 3) {
                    // left operator right format
                    nh_ecmascript_ASTNode *left_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                    nh_ecmascript_ParseNode *operator_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                    nh_ecmascript_ASTNode *right_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[2]);
                    
                    const char *op_str = operator_p->Value_p ? operator_p->Value_p->String.p : "<";
                    return nh_ecmascript_createBinaryExpression(left_p, right_p, op_str);
                }
                else if (Node_p->Children.size == 1) {
                    // Simple expression case
                    return nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                }
                break;
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_UPDATE_EXPRESSION:
            {
                if (Node_p->Children.size >= 2) {
                    // Check if it's prefix or postfix
                    nh_ecmascript_ParseNode *first_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[0];
                    nh_ecmascript_ParseNode *second_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                    
                    bool prefix = false;
                    nh_ecmascript_ASTNode *argument_p = NULL;
                    const char *op_str = NULL;
                    
                    if (first_p->Value_p && (!strcmp(first_p->Value_p->String.p, "++") || !strcmp(first_p->Value_p->String.p, "--"))) {
                        // Prefix: ++i or --i
                        prefix = true;
                        op_str = first_p->Value_p->String.p;
                        argument_p = nh_ecmascript_parseExpression(second_p);
                    }
                    else if (second_p->Value_p && (!strcmp(second_p->Value_p->String.p, "++") || !strcmp(second_p->Value_p->String.p, "--"))) {
                        // Postfix: i++ or i--
                        prefix = false;
                        op_str = second_p->Value_p->String.p;
                        argument_p = nh_ecmascript_parseExpression(first_p);
                    }
                    
                    if (argument_p && op_str) {
                        return nh_ecmascript_createUpdateExpression(argument_p, op_str, prefix);
                    }
                }
                else if (Node_p->Children.size == 1) {
                    // Simple expression case
                    return nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                }
                break;
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION:
            {
                nh_core_List declarators = nh_core_initList(1);
                
                // Parse the identifier
                if (Node_p->Children.size > 0) {
                    nh_ecmascript_ParseNode *id_node_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[0];
                    nh_ecmascript_ASTNode *id_p = nh_ecmascript_parseExpression(id_node_p);
                    
                    // Parse initializer if present
                    nh_ecmascript_ASTNode *init_p = NULL;
                    if (Node_p->Children.size > 1) {
                        nh_ecmascript_ParseNode *init_node_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                        if (init_node_p->type == NH_ECMASCRIPT_PARSE_NODE_INITIALIZER && init_node_p->Children.size > 1) {
                            init_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)init_node_p->Children.pp[1]);
                        }
                    }
                    
                    nh_ecmascript_ASTNode *declarator_p = nh_ecmascript_createVariableDeclarator(id_p, init_p);
                    nh_core_appendToList(&declarators, declarator_p);
                }
                
                return nh_ecmascript_createVariableDeclaration("var", declarators);
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_FOR_DECLARATION:
            {
                nh_core_List declarators = nh_core_initList(1);
                
                // Parse variable declaration list
                if (Node_p->Children.size > 1) {
                    nh_ecmascript_ParseNode *var_decl_list_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                    if (var_decl_list_p->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST) {
                        for (size_t i = 0; i < var_decl_list_p->Children.size; i++) {
                            nh_ecmascript_ParseNode *var_decl_p = (nh_ecmascript_ParseNode*)var_decl_list_p->Children.pp[i];
                            if (var_decl_p->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION) {
                                nh_ecmascript_ASTNode *declarator_p = nh_ecmascript_parseExpression(var_decl_p);
                                if (declarator_p) {
                                    nh_core_appendToList(&declarators, declarator_p);
                                }
                            }
                        }
                    }
                }
                
                return nh_ecmascript_createForDeclaration("var", declarators);
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_PRIMARY_EXPRESSION:
        case NH_ECMASCRIPT_PARSE_NODE_LEFT_HAND_SIDE_EXPRESSION:
        case NH_ECMASCRIPT_PARSE_NODE_EXPRESSION:
            // These are wrapper nodes, parse their children
            if (Node_p->Children.size > 0) {
                return nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
            }
            break;
            
        case NH_ECMASCRIPT_PARSE_NODE_ARRAY_LITERAL:
            {
                // Handle array literals like [32, 7, 133, 22, 57]
                if (Node_p->Children.size >= 3) {
                    // Look for ElementList (second child)
                    nh_ecmascript_ParseNode *ElementList_p = NULL;
                    for (size_t i = 1; i < Node_p->Children.size - 1; i++) {
                        nh_ecmascript_ParseNode *child_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[i];
                        if (child_p->type == NH_ECMASCRIPT_PARSE_NODE_ELEMENT_LIST) {
                            ElementList_p = child_p;
                            break;
                        }
                    }
                    
                    if (ElementList_p) {
                        nh_core_List elements = nh_core_initList(ElementList_p->Children.size);
                        
                        // Parse elements (skip commas)
                        for (size_t i = 0; i < ElementList_p->Children.size; i++) {
                            nh_ecmascript_ParseNode *element_p = (nh_ecmascript_ParseNode*)ElementList_p->Children.pp[i];
                            if (element_p->Value_p == NULL || element_p->Value_p->String.p[0] != ',') {
                                nh_ecmascript_ASTNode *element_ast_p = nh_ecmascript_parseExpression(element_p);
                                if (element_ast_p) {
                                    nh_core_appendToList(&elements, element_ast_p);
                                }
                            }
                        }
                        
                        // For now, return a simple identifier representing the array
                        // In a full implementation, you'd want an ArrayExpression AST node
                        return nh_ecmascript_createIdentifier("ArrayLiteral");
                    }
                }
                break;
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_MULTIPLICATIVE_EXPRESSION:
            {
                if (Node_p->Children.size >= 3) {
                    // left operator right format
                    nh_ecmascript_ASTNode *left_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                    nh_ecmascript_ParseNode *operator_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                    nh_ecmascript_ASTNode *right_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[2]);
                    
                    const char *op_str = operator_p->Value_p ? operator_p->Value_p->String.p : "/";
                    return nh_ecmascript_createBinaryExpression(left_p, right_p, op_str);
                }
                else if (Node_p->Children.size == 1) {
                    // Simple expression case
                    return nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                }
                break;
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_COVER_CALL_EXPRESSION_AND_ASYNC_ARROW_HEAD:
            {
                // Handle function calls like console.log("Numbers:", numbers)
                if (Node_p->Children.size >= 2) {
                    nh_ecmascript_ASTNode *caller_p = nh_ecmascript_parseExpression((nh_ecmascript_ParseNode*)Node_p->Children.pp[0]);
                    nh_ecmascript_ParseNode *args_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                    
                    if (args_p->type == NH_ECMASCRIPT_PARSE_NODE_ARGUMENTS) {
                        nh_core_List arguments = nh_core_initList(4);
                        
                        // Look for ArgumentList
                        for (size_t i = 1; i < args_p->Children.size - 1; i++) {
                            nh_ecmascript_ParseNode *arg_list_p = (nh_ecmascript_ParseNode*)args_p->Children.pp[i];
                            if (arg_list_p->type == NH_ECMASCRIPT_PARSE_NODE_ARGUMENT_LIST) {
                                // Parse arguments (skip commas)
                                for (size_t j = 0; j < arg_list_p->Children.size; j++) {
                                    nh_ecmascript_ParseNode *arg_p = (nh_ecmascript_ParseNode*)arg_list_p->Children.pp[j];
                                    if (arg_p->Value_p == NULL || arg_p->Value_p->String.p[0] != ',') {
                                        nh_ecmascript_ASTNode *arg_ast_p = nh_ecmascript_parseExpression(arg_p);
                                        if (arg_ast_p) {
                                            nh_core_appendToList(&arguments, arg_ast_p);
                                        }
                                    }
                                }
                                break;
                            }
                        }
                        
                        nh_ecmascript_ASTNode *call_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_CALL_EXPRESSION);
                        call_p->CallExpression.Caller_p = caller_p;
                        call_p->CallExpression.Arguments = arguments;
                        return call_p;
                    }
                }
                break;
            }
            
        default:
            break;
    }
    
    return NULL;
}

static nh_ecmascript_ASTNode *nh_ecmascript_parseStatement(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p == NULL) {
        return NULL;
    }
    
    
    if (Node_p->Children.size == 0) {
        return NULL;
    }

    // Handle different statement structures
    
    // Check if this is a direct statement node (not wrapped)
    switch (Node_p->type) {
        case NH_ECMASCRIPT_PARSE_NODE_EXPRESSION_STATEMENT:
            {
                if (Node_p->Children.size > 0) {
                    nh_ecmascript_ParseNode *ExprNode_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[0];
                    nh_ecmascript_ASTNode *ExprAST_p = nh_ecmascript_parseExpression(ExprNode_p);
                    return nh_ecmascript_createExpressionStatement(ExprAST_p);
                }
                break;
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_FOR_STATEMENT:
            {
                nh_ecmascript_ASTNode *init_p = NULL;
                nh_ecmascript_ASTNode *test_p = NULL;
                nh_ecmascript_ASTNode *update_p = NULL;
                nh_ecmascript_ASTNode *body_p = NULL;
                
                // Parse init (third child, if present)
                if (Node_p->Children.size > 2) {
                    nh_ecmascript_ParseNode *InitNode_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[2];
                    if (InitNode_p->type == NH_ECMASCRIPT_PARSE_NODE_FOR_DECLARATION) {
                        init_p = nh_ecmascript_parseExpression(InitNode_p);
                    } else {
                        init_p = nh_ecmascript_parseExpression(InitNode_p);
                    }
                }
                
                // Parse test (fourth child, if present)
                if (Node_p->Children.size > 3) {
                    nh_ecmascript_ParseNode *TestNode_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[3];
                    test_p = nh_ecmascript_parseExpression(TestNode_p);
                }
                
                // Parse update (fifth child, if present)
                if (Node_p->Children.size > 4) {
                    nh_ecmascript_ParseNode *UpdateNode_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[4];
                    update_p = nh_ecmascript_parseExpression(UpdateNode_p);
                }
                
                // Parse body (last child)
                if (Node_p->Children.size > 5) {
                    nh_ecmascript_ParseNode *BodyNode_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[Node_p->Children.size - 1];
                    body_p = nh_ecmascript_parseStatement(BodyNode_p);
                }
                
                return nh_ecmascript_createForStatement(init_p, test_p, update_p, body_p);
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_BLOCK_STATEMENT:
            {
                nh_ecmascript_ParseNode *BlockNode_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[0];
                if (BlockNode_p->type == NH_ECMASCRIPT_PARSE_NODE_BLOCK && BlockNode_p->Children.size > 1) {
                    // Parse statement list (second child)
                    nh_ecmascript_ParseNode *StmtList_p = (nh_ecmascript_ParseNode*)BlockNode_p->Children.pp[1];
                    if (StmtList_p->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST) {
                        nh_core_List body = nh_core_initList(StmtList_p->Children.size);
                        
                        for (size_t i = 0; i < StmtList_p->Children.size; i++) {
                            nh_ecmascript_ParseNode *StmtItem_p = (nh_ecmascript_ParseNode*)StmtList_p->Children.pp[i];
                            if (StmtItem_p->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM && StmtItem_p->Children.size > 0) {
                                nh_ecmascript_ParseNode *Stmt_p = (nh_ecmascript_ParseNode*)StmtItem_p->Children.pp[0];
                                nh_ecmascript_ASTNode *StmtAST_p = nh_ecmascript_parseStatement(Stmt_p);
                                if (StmtAST_p != NULL) {
                                    nh_core_appendToList(&body, StmtAST_p);
                                }
                            }
                        }
                        
                        return nh_ecmascript_createBlockStatement(body);
                    }
                }
                break;
            }
            
        case NH_ECMASCRIPT_PARSE_NODE_EMPTY_STATEMENT:
            // Empty statements don't need AST representation
            return NULL;
            
        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT:
            {
                // Handle var statements like "var numbers = [32, 7, 133, 22, 57];"
                if (Node_p->Children.size >= 3) {
                    // Skip the "var" token and semicolon, parse the VariableDeclaration
                    nh_ecmascript_ParseNode *VarDecl_p = (nh_ecmascript_ParseNode*)Node_p->Children.pp[1];
                    if (VarDecl_p->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION) {
                        nh_ecmascript_ASTNode *result = nh_ecmascript_parseExpression(VarDecl_p);
                        return result;
                    }
                }
                break;
            }
            
        default:
            break;
    }
    
    return NULL;
}

// MAIN FUNCTION =======================================================================================

nh_ecmascript_ASTNode *nh_ecmascript_parseAST(
    nh_ecmascript_ParseNode *Node_p)
{
    if (Node_p == NULL) {
        return NULL;
    }

    nh_ecmascript_ParseNode *CollapsedNode_p = nh_ecmascript_collapseCST(Node_p);

    bool branch_p[1024] = {0};
    nh_ecmascript_logParseTree(NULL, CollapsedNode_p, NULL, 0, branch_p);

    // Handle root statement list node
    if (CollapsedNode_p->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST) {
        nh_ecmascript_ASTNode *Program_p = nh_ecmascript_createASTNode(NH_ECMASCRIPT_AST_NODE_PROGRAM);
        Program_p->Program.Body = nh_core_initList(CollapsedNode_p->Children.size);
        
        for (size_t i = 0; i < CollapsedNode_p->Children.size; i++) {
            nh_ecmascript_ParseNode *StmtItem_p = (nh_ecmascript_ParseNode*)CollapsedNode_p->Children.pp[i];
            
            nh_ecmascript_ASTNode *StmtAST_p = NULL;
            
            // Check if it's a StatementListItem wrapper
            if (StmtItem_p->type == NH_ECMASCRIPT_PARSE_NODE_STATEMENT_LIST_ITEM && StmtItem_p->Children.size > 0) {
                nh_ecmascript_ParseNode *Stmt_p = (nh_ecmascript_ParseNode*)StmtItem_p->Children.pp[0];
                StmtAST_p = nh_ecmascript_parseStatement(Stmt_p);
            }
            // Check if it's a direct statement (VariableStatement, ForStatement, etc.)
            else if (StmtItem_p->type == NH_ECMASCRIPT_PARSE_NODE_VARIABLE_STATEMENT ||
                     StmtItem_p->type == NH_ECMASCRIPT_PARSE_NODE_FOR_STATEMENT ||
                     StmtItem_p->type == NH_ECMASCRIPT_PARSE_NODE_EXPRESSION_STATEMENT) {
                StmtAST_p = nh_ecmascript_parseStatement(StmtItem_p);
            } else {
            }
            
            if (StmtAST_p != NULL) {
                nh_core_appendToList(&Program_p->Program.Body, StmtAST_p);
            }
        }
        
        bool branch2_p[1024] = {0};
        nh_ecmascript_logAST(NULL, Program_p, NULL, 0, branch2_p);

        return Program_p;
    }
    
    
    return NULL; 
}
