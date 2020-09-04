#ifndef NH_JS_PARSER_H
#define NH_JS_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Core/Header/List.h"
#include "../../API/Header/Netzhaut.h"

#include <stdbool.h>

typedef struct Nh_JS_Script Nh_JS_Script;
typedef struct Nh_JS_Parsable Nh_JS_Parsable;

#endif

/** @addtogroup JSEnums Enums
 *  \ingroup JS
 *  @{
 */

    typedef enum NH_JS_KEYWORD {
        NH_JS_KEYWORD_IF,
        NH_JS_KEYWORD_IN,
        NH_JS_KEYWORD_DO,
        NH_JS_KEYWORD_VAR,
        NH_JS_KEYWORD_FOR,
        NH_JS_KEYWORD_NEW,
        NH_JS_KEYWORD_TRY,
        NH_JS_KEYWORD_LET,
        NH_JS_KEYWORD_THIS,
        NH_JS_KEYWORD_ELSE,
        NH_JS_KEYWORD_CASE,
        NH_JS_KEYWORD_VOID,
        NH_JS_KEYWORD_WITH,
        NH_JS_KEYWORD_ENUM,
        NH_JS_KEYWORD_WHILE,
        NH_JS_KEYWORD_BREAK,
        NH_JS_KEYWORD_CATCH,
        NH_JS_KEYWORD_THROW,
        NH_JS_KEYWORD_CONST,
        NH_JS_KEYWORD_YIELD,
        NH_JS_KEYWORD_CLASS,
        NH_JS_KEYWORD_SUPER,
        NH_JS_KEYWORD_RETURN,
        NH_JS_KEYWORD_TYPEOF,
        NH_JS_KEYWORD_DELETE,
        NH_JS_KEYWORD_SWITCH,
        NH_JS_KEYWORD_EXPORT,
        NH_JS_KEYWORD_IMPORT,
        NH_JS_KEYWORD_PUBLIC,
        NH_JS_KEYWORD_STATIC,
        NH_JS_KEYWORD_DEFAULT,
        NH_JS_KEYWORD_FINALLY,
        NH_JS_KEYWORD_EXTENDS,
        NH_JS_KEYWORD_PACKAGE,
        NH_JS_KEYWORD_PRIVATE,
        NH_JS_KEYWORD_FUNCTION,
        NH_JS_KEYWORD_CONTINUE,
        NH_JS_KEYWORD_DEBUGGER,
        NH_JS_KEYWORD_PROTECTED,
        NH_JS_KEYWORD_INTERFACE,
        NH_JS_KEYWORD_INSTANCEOF,
        NH_JS_KEYWORD_IMPLEMENTS,
    } NH_JS_KEYWORD;

    typedef enum NH_JS_OPERATOR {
        NH_JS_OPERATOR_INCREMENT,
        NH_JS_OPERATOR_DECREMENT,
        NH_JS_OPERATOR_EQUAL,
        NH_JS_OPERATOR_UNEQUAL,
        NH_JS_OPERATOR_LESS_EQUAL,
        NH_JS_OPERATOR_GREATER_EQUAL,
        NH_JS_OPERATOR_SUBTRACT_ASSIGN,
        NH_JS_OPERATOR_ADD_ASSIGN,
        NH_JS_OPERATOR_MULTIPLY_ASSIGN,
        NH_JS_OPERATOR_DIVIDE_ASSIGN,
        NH_JS_OPERATOR_SHORT_CIRCUIT_AND,
        NH_JS_OPERATOR_SHORT_CIRCUIT_OR,
        NH_JS_OPERATOR_ASSIGN,
        NH_JS_OPERATOR_LESS,
        NH_JS_OPERATOR_GREATER,
        NH_JS_OPERATOR_NOT,
        NH_JS_OPERATOR_AND,
        NH_JS_OPERATOR_OR,
        NH_JS_OPERATOR_SUBTRACT,
        NH_JS_OPERATOR_ADD,
        NH_JS_OPERATOR_MULTIPLY,
        NH_JS_OPERATOR_DIVIDE,
    } NH_JS_OPERATOR;
    
    typedef enum NH_JS_PARSABLE {
        NH_JS_PARSABLE_UNDEFINED,             
        NH_JS_PARSABLE_FUNCTION_DECLARATION,  
        NH_JS_PARSABLE_MEMBER_EXPRESSION,     
        NH_JS_PARSABLE_ASSIGNMENT_EXPRESSION, 
        NH_JS_PARSABLE_EXPRESSION_STATEMENT,  
        NH_JS_PARSABLE_CALL_EXPRESSION,       
        NH_JS_PARSABLE_IDENTIFIER,            
        NH_JS_PARSABLE_STRING_LITERAL,        
        NH_JS_PARSABLE_NUMERIC_LITERAL,       
        NH_JS_PARSABLE_BLOCK_STATEMENT,       
        NH_JS_PARSABLE_VARIABLE_DECLARATOR,   
        NH_JS_PARSABLE_VARIABLE_DECLARATION,  
        NH_JS_PARSABLE_BINARY_EXPRESSION,     
        NH_JS_PARSABLE_WHILE_STATEMENT, 
        NH_JS_PARSABLE_FOR_STATEMENT,
        NH_JS_PARSABLE_UPDATE_EXPRESSION,    
        NH_JS_PARSABLE_IF_STATEMENT,
        NH_JS_PARSABLE_NULL_LITERAL,
        NH_JS_PARSABLE_LOGICAL_EXPRESSION,
        NH_JS_PARSABLE_BOOLEAN_TRUE,
        NH_JS_PARSABLE_BOOLEAN_FALSE,
        NH_JS_PARSABLE_CONSTRUCTOR_CALL,
    } NH_JS_PARSABLE;

    typedef enum NH_JS_ITERATION_STATEMENT {
        NH_JS_ITERATION_STATEMENT_WHILE,
        NH_JS_ITERATION_STATEMENT_DO_WHILE,
        NH_JS_ITERATION_STATEMENT_FOR,
        NH_JS_ITERATION_STATEMENT_FOR_IN,
        NH_JS_ITERATION_STATEMENT_FOR_OF,
        NH_JS_ITERATION_STATEMENT_FOR_AWAIT_OF,
    } NH_JS_ITERATION_STATEMENT;

    /**
     * https://www.ecma-international.org/ecma-262/#sec-ecmascript-language-types
     */
    typedef enum NH_JS_TYPE {
        NH_JS_TYPE_UNDEFINED,
        NH_JS_TYPE_NULL,
        NH_JS_TYPE_BOOLEAN_TRUE,
        NH_JS_TYPE_BOOLEAN_FALSE,
        NH_JS_TYPE_STRING,
        NH_JS_TYPE_SYMBOL,
        NH_JS_TYPE_NUMBER,
        NH_JS_TYPE_OBJECT
    } NH_JS_TYPE;

/** @} */

/** @addtogroup JSStructs Structs
 *  \ingroup JS
 *  @{
 */

    typedef struct Nh_JS_BlockStatement {
        Nh_List Variables;
        Nh_JS_Parsable *Parsables_p;
        int parsableCount;
        struct Nh_JS_BlockStatement *Parent_p;
    } Nh_JS_BlockStatement;

    struct Nh_JS_Parsable {
        int depth;
        NH_JS_PARSABLE type;
        void *Interpreter_p;
        void *Data_p;
        Nh_JS_BlockStatement *Container_p;
    };

    typedef struct Nh_JS_IfStatement {
        int parsableCount;
        Nh_JS_Parsable *Parsables_p;
    } Nh_JS_IfStatement;

    typedef struct Nh_JS_MemberExpression {
        Nh_JS_Parsable Left, Right;
    } Nh_JS_MemberExpression;
    
    typedef struct Nh_JS_ExpressionStatement {
        Nh_JS_Parsable Parsable;
    } Nh_JS_ExpressionStatement;
    
    typedef struct Nh_JS_CallExpression {
        int argumentCount;
        Nh_JS_Parsable Parsable;
        Nh_JS_Parsable *Arguments_p;
    } Nh_JS_CallExpression;
    
    typedef struct Nh_JS_Identifier {
        char *name_p;
    } Nh_JS_Identifier;
    
    typedef struct Nh_JS_VariableDeclarator {
        Nh_JS_Parsable Identifier;
        Nh_JS_Parsable Parsable;
    } Nh_JS_VariableDeclarator;
    
    typedef struct Nh_JS_VariableDeclaration {
        NH_JS_KEYWORD kind;
        Nh_JS_VariableDeclarator Declarator;
    } Nh_JS_VariableDeclaration;
    
    typedef struct Nh_JS_WhileStatement {
        char *todo_p;
    } Nh_JS_WhileStatement;

    typedef struct Nh_JS_ForStatement {
        NH_JS_ITERATION_STATEMENT type;
        int expressionCount;
        Nh_JS_Parsable *Expressions_p;
        Nh_JS_Parsable Statement;
    } Nh_JS_ForStatement;

    /**
     * https://www.ecma-international.org/ecma-262/#sec-ecmascript-language-types-string-type 
     */
    typedef struct Nh_JS_StringLiteral {
        char *value_p;
    } Nh_JS_StringLiteral;
   
    typedef struct Nh_JS_FunctionDeclaration {
        Nh_JS_Parsable Name;
        int parameterCount;
        Nh_JS_Parsable *Parameters_p;
        Nh_JS_Parsable Block;
    } Nh_JS_FunctionDeclaration;

    typedef struct Nh_JS_BinaryExpression {
        NH_JS_OPERATOR operator_;
        Nh_JS_Parsable Left;
        Nh_JS_Parsable Right;
    } Nh_JS_BinaryExpression;

    typedef struct Nh_JS_LogicalExpression {
        NH_JS_OPERATOR operator_;
        Nh_JS_Parsable Left;
        Nh_JS_Parsable Right;
    } Nh_JS_LogicalExpression;

    typedef struct Nh_JS_AssignmentExpression {
        NH_JS_OPERATOR operator_;
        Nh_JS_Parsable Left;
        Nh_JS_Parsable Right;
    } Nh_JS_AssignmentExpression;

    typedef struct Nh_JS_ConstructorCall {
        Nh_JS_Parsable CallExpression;
    } Nh_JS_ConstructorCall;

/** @} */

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    NH_RESULT Nh_JS_parseScript(
        Nh_JS_Script *Script_p
    );

    void Nh_JS_unparseScript(
        Nh_JS_Script *Script_p
    );

/** @} */

#endif 
