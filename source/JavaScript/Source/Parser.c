// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Parser.h"
#include "../Header/Data.h"
#include "../Header/Macros.h"
#include "../Header/Variable.h"

#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/String.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"
#include "../../HTML/Header/Attribute.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

// DEFINE ==========================================================================================

typedef int (*NH_JS_ParserPointer)(void *, void *, void *, void *, void *);

#define DONT_IGNORE(c) (c != ' ' && c != '\n' && c != '\t' && c != '\v' && c != '\r' && c != '\0')
#define IS_LINE_BREAK(c) (c == '\n' || c == '\r')
#define GET_WORD_LENGTH(length) for (length = 0; DONT_IGNORE(current_p[length]) && !IS_LINE_BREAK(current_p[length]); ++length){}

#define CONFIGURE(Parsable_p)                                                  \
    Parsable_p->depth = *depth_p;                                              \
    Parsable_p->Interpreter_p = Nh_JS_getInterpreterPointer(Parsable_p->type); \
    Parsable_p->Container_p = Container_p;

#define GET_TYPE_AND_PARSE(Parsable, code_p, b)                                         \
{                                                                                       \
    Parsable.type = Nh_JS_getParsableType(Maps_p, code_p, b);                           \
    NH_CHECK(Nh_JS_allocateParsable(Parsable.type, &Parsable.Data_p))                   \
    NH_CHECK(Parser_pp[Parsable.type](Maps_p, code_p, &Parsable, depth_p, Container_p)) \
}

#define PARSE(Parsable, code_p)                                                         \
{                                                                                       \
    NH_CHECK(Nh_JS_allocateParsable(Parsable.type, &Parsable.Data_p))                   \
    NH_CHECK(Parser_pp[Parsable.type](Maps_p, code_p, &Parsable, depth_p, Container_p)) \
}

// DECLARE =========================================================================================

static NH_RESULT Nh_JS_parseBinaryExpression(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseIfStatement(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseForStatement(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseFunctionDeclaration(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseBlockStatement(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseExpressionStatement(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseStringLiteral(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseMemberExpression(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseCallExpression(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseVariableDeclaration(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseIdentifier(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseNumericLiteral(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseBooleanLiteral(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseConstructorCall(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);

static NH_RESULT Nh_JS_parseVariableDeclarator(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_VariableDeclarator *Declarator_p, int *depth_p, Nh_JS_BlockStatement *Container_p, NH_JS_KEYWORD kind
);
static NH_RESULT Nh_JS_parseArguments(
    Nh_HashMaps *Maps_p, Nh_JS_CallExpression *CallExpression_p, char *part_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseParameters(
    Nh_HashMaps *Maps_p, Nh_JS_FunctionDeclaration *Function_p, char *current_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseRightMember(
    Nh_HashMaps *Maps_p, Nh_JS_MemberExpression *MemberExpression_p, char *part_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);
static NH_RESULT Nh_JS_parseLeftMember(
    Nh_HashMaps *Maps_p, Nh_JS_MemberExpression *MemberExpression_p, char *part_p, int *depth_p, Nh_JS_BlockStatement *Container_p
);

static NH_RESULT Nh_JS_allocateParsable(
    NH_JS_PARSABLE type, void **data_pp
);

static NH_JS_PARSABLE Nh_JS_getParsableType(
    Nh_HashMaps *Maps_p, char *line_p, bool ignoreCurved
);
static NH_JS_OPERATOR Nh_JS_getOperator(
    char *line_p
);
static int NH_JS_Nh_JS_getOperatorPriority(
    NH_JS_OPERATOR operator
);
static int NH_JS_Nh_JS_getOperatorWidth(
    NH_JS_OPERATOR operator
);
static void Nh_JS_getCurrentWord(
    char *current_p, int *charCount_p, char *word_p, char *stop_p
);
static NH_JS_KEYWORD Nh_JS_getKeyword(
    char *word_p
); 
static NH_JS_KEYWORD Nh_JS_getCurrentKeyword(
    char *current_p
); 
static NH_RESULT Nh_JS_getForStatementType(
    char *current_p, NH_JS_ITERATION_STATEMENT *type_p
);

static bool Nh_JS_isCallExpression(
    char *current_p, int length
);
static bool Nh_JS_isMemberExpression(
    char *current_p, int length
);
static bool Nh_JS_isStringLiteral(
    char *current_p, int length
);
static bool Nh_JS_isBlockStatement(
    char *current_p, int length
);
static bool Nh_JS_isBinaryExpression(
    char *current_p, int length
);
static bool Nh_JS_isAssignmentExpression(
    char *current_p, int length
);
static bool Nh_JS_isNumericLiteral(
    char *current_p, int length
);

static char *Nh_JS_seekNext(
    char *begin_p, int offset
);
static char *Nh_JS_seekEndOfParsable(
    char *current_p, bool ignoreCurved
);

static void Nh_JS_destroyParsable(
    Nh_JS_Parsable *Parsable_p
);

// DATA ============================================================================================

NH_JS_ParserPointer Parser_pp[22] = 
{
    NULL,
    (NH_JS_ParserPointer)Nh_JS_parseFunctionDeclaration,
    (NH_JS_ParserPointer)Nh_JS_parseMemberExpression,
    (NH_JS_ParserPointer)Nh_JS_parseBinaryExpression,
    (NH_JS_ParserPointer)Nh_JS_parseExpressionStatement,
    (NH_JS_ParserPointer)Nh_JS_parseCallExpression,
    (NH_JS_ParserPointer)Nh_JS_parseIdentifier,
    (NH_JS_ParserPointer)Nh_JS_parseStringLiteral,
    (NH_JS_ParserPointer)Nh_JS_parseNumericLiteral,
    (NH_JS_ParserPointer)Nh_JS_parseBlockStatement,
    NULL,
    (NH_JS_ParserPointer)Nh_JS_parseVariableDeclaration,
    (NH_JS_ParserPointer)Nh_JS_parseBinaryExpression,
    NULL,
    (NH_JS_ParserPointer)Nh_JS_parseForStatement,
    NULL,
    (NH_JS_ParserPointer)Nh_JS_parseIfStatement,
    NULL,
    NULL,
    (NH_JS_ParserPointer)Nh_JS_parseBooleanLiteral,
    (NH_JS_ParserPointer)Nh_JS_parseBooleanLiteral,
    (NH_JS_ParserPointer)Nh_JS_parseConstructorCall,
};

// PARSER API ======================================================================================

NH_RESULT Nh_JS_parseScript(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    int parsableCount = 0;
    char *current_p = Nh_JS_seekNext(Nh_getData(Script_p->URI, NULL), 0);

    while (current_p != NULL) 
    {
        parsableCount++;
        current_p = Nh_JS_seekNext(Nh_JS_seekEndOfParsable(current_p, true), 1);
    }

    if (parsableCount > 0) 
    {
        Nh_JS_Parsable *Parsables_p = Nh_allocate(sizeof(Nh_JS_Parsable) * parsableCount);
        NH_CHECK_MEM(Parsables_p)

        current_p = Nh_JS_seekNext(Nh_getData(Script_p->URI, NULL), 0);

        Script_p->Container.Parsables_p   = Parsables_p;
        Script_p->Container.parsableCount = parsableCount;

        int index = 0;
        while (current_p != NULL)
        {
            int depth = 0;
            
            Script_p->Container.Parsables_p[index].type = Nh_JS_getParsableType(Nh_getHashMaps(), current_p, true);
            
            NH_CHECK(Nh_JS_allocateParsable(Script_p->Container.Parsables_p[index].type, &Script_p->Container.Parsables_p[index].Data_p))
            NH_CHECK(
                Parser_pp[Script_p->Container.Parsables_p[index].type]
                    (Nh_getHashMaps(), current_p, &Script_p->Container.Parsables_p[index++], &depth, &Script_p->Container)
            ) 

            current_p = Nh_JS_seekNext(Nh_JS_seekEndOfParsable(current_p, true), 1);
        }
    }

    Script_p->Flags.parsed = true;

NH_END(NH_SUCCESS)
}

// PARSER ==========================================================================================

static NH_RESULT Nh_JS_parseBinaryExpression(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_BinaryExpression *Binary_p = Parsable_p->Data_p;

    if (*current_p == '(') {current_p = &current_p[1];}
    char *tmp_p = Nh_JS_seekNext(current_p, 0);

    int lLength = 0;
    int prio = -1;
    for (int length = 0; length < strlen(tmp_p); ++length) 
    {
        NH_JS_OPERATOR operator_ = Nh_JS_getOperator(&tmp_p[length]);
        if (tmp_p[length] == ';') {break;}
        if (operator_ == -1) {continue;}

        int tmp = NH_JS_Nh_JS_getOperatorPriority(operator_);

        if (tmp >= prio) {
            Binary_p->operator_ = operator_;
            prio = tmp; 
            lLength = length;
        }
        else {break;}

        if (NH_JS_Nh_JS_getOperatorWidth(operator_) > 1) {length++;}
    }

    if (lLength > 0) 
    {
        char c = tmp_p[lLength];
        tmp_p[lLength] = ';'; // mark end of left expression
        GET_TYPE_AND_PARSE(Binary_p->Left, tmp_p, false)
        tmp_p[lLength] = c;
    
    } else {NH_END(NH_ERROR_BAD_STATE)}

    int rLength = lLength;
    for (; rLength < strlen(tmp_p); ++rLength) {
        if (tmp_p[rLength] == ')' || tmp_p[rLength] == ';') {break;}
    }

    if (rLength > 0) 
    {
        tmp_p = &tmp_p[lLength];

        if (Binary_p->operator_ == NH_JS_OPERATOR_ASSIGN
        ||  Binary_p->operator_ == NH_JS_OPERATOR_LESS
        ||  Binary_p->operator_ == NH_JS_OPERATOR_GREATER) {tmp_p = &tmp_p[1];}
        else                                               {tmp_p = &tmp_p[2];}

        GET_TYPE_AND_PARSE(Binary_p->Right, tmp_p, false)
        
    } else {NH_END(NH_ERROR_BAD_STATE)}

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseIfStatement(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_IfStatement *If_p = Parsable_p->Data_p;

    char *tmp_p = Nh_JS_seekNext(current_p, 2);

    if (tmp_p == NULL || *tmp_p != '(') {NH_END(NH_JS_ERROR_UNEXPECTED_TOKEN)}

    Nh_List List;
    NH_INIT_LIST(List)

    char *end_p = Nh_JS_seekEndOfParsable(tmp_p, true);
    char replace = end_p[1];
    end_p[1] = '\0';

    while (tmp_p != NULL)
    {
        char *end2_p = Nh_JS_seekEndOfParsable(tmp_p, false);
        char replace2 = end2_p[1];
        end2_p[1] = '\0';

        Nh_JS_Parsable *Parsable_p = Nh_allocate(sizeof(Nh_JS_Parsable));
        NH_CHECK_MEM(Parsable_p)
        
        GET_TYPE_AND_PARSE((*Parsable_p), tmp_p, false)

        Nh_addListItem(&List, Parsable_p);

        end2_p[1] = replace2;
        tmp_p = Nh_JS_seekNext(Nh_JS_seekEndOfParsable(tmp_p, false), 1);
    }

    if (List.count > 0) 
    {
        If_p->Parsables_p = Nh_allocate(sizeof(Nh_JS_Parsable) * List.count);
        NH_CHECK_MEM(If_p->Parsables_p)
        
        for (int i = 0; i < List.count; ++i) {
            If_p->Parsables_p[i] = *((Nh_JS_Parsable*)Nh_getListItem(&List, i));
        }
    }

    If_p->parsableCount = List.count;

    end_p[1] = replace;
    Nh_destroyList(&List, true);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseForStatement(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_ForStatement *For_p = Parsable_p->Data_p;

    NH_CHECK(Nh_JS_getForStatementType(current_p, &For_p->type))

    char *tmp_p = Nh_JS_seekNext(current_p, 3), *tmp2_p;
    char replace;
    int count = 0;

    switch (For_p->type) 
    {
        case NH_JS_ITERATION_STATEMENT_FOR          : break;
        case NH_JS_ITERATION_STATEMENT_FOR_IN       : break;
        case NH_JS_ITERATION_STATEMENT_FOR_AWAIT_OF : break;
       
        case NH_JS_ITERATION_STATEMENT_FOR_OF       : 

            if (*tmp_p != '(') {NH_END(NH_ERROR_BAD_STATE)}
            tmp_p = Nh_JS_seekNext(tmp_p, 1);

            For_p->expressionCount = 2;
            For_p->Expressions_p = Nh_allocate(sizeof(Nh_JS_Parsable) * 2);
            NH_CHECK_MEM(For_p->Expressions_p)

            for (count = 0; tmp_p[count] != 'o' || tmp_p[count + 1] != 'f'; ++count) {}
            replace = tmp_p[count];
            tmp_p[count] = '\0';

            GET_TYPE_AND_PARSE(For_p->Expressions_p[0], tmp_p, true)

            tmp_p[count] = replace;

            tmp2_p = Nh_JS_seekEndOfParsable(tmp_p, false);
            replace = *tmp2_p;
            *tmp2_p = '\0';
            tmp_p = Nh_JS_seekNext(tmp_p, count + 2);

            GET_TYPE_AND_PARSE(For_p->Expressions_p[1], tmp_p, true)

            *tmp2_p = replace;

            for (count = 0; tmp_p[count] != ')'; ++count) {}
            tmp_p = Nh_JS_seekNext(tmp_p, count + 1);

            GET_TYPE_AND_PARSE(For_p->Statement, tmp_p, true)

            break;
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseFunctionDeclaration(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_FunctionDeclaration *Function_p = Parsable_p->Data_p;

    Function_p->parameterCount = 0;
    Function_p->Parameters_p = NULL;

    current_p = Nh_JS_seekNext(current_p, 8);

    // function name
    int charCount = 0;
    char stop = '(', replace;
    Nh_JS_getCurrentWord(current_p, &charCount, NULL, &stop);

    replace = current_p[charCount];
    current_p[charCount] = '\0';

    GET_TYPE_AND_PARSE(Function_p->Name, current_p, true);
    if (Function_p->Name.type != NH_JS_PARSABLE_IDENTIFIER) {NH_END(NH_ERROR_BAD_STATE)}

    current_p[charCount] = replace;

    // parameters
    current_p = Nh_JS_seekNext(current_p, charCount + 1);
    NH_CHECK(Nh_JS_parseParameters(Maps_p, Function_p, current_p, depth_p, Container_p))

    for (charCount = 0; current_p[charCount] != ')'; ++charCount) {}
    current_p = Nh_JS_seekNext(current_p, charCount + 1);

    Function_p->Block.type = NH_JS_PARSABLE_BLOCK_STATEMENT;
    PARSE(Function_p->Block, current_p)

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseBlockStatement(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    
    Nh_JS_BlockStatement *Block_p = Parsable_p->Data_p;
    NH_INIT_LIST(Block_p->Variables)
    Block_p->Parsables_p   = NULL;
    Block_p->parsableCount = 0;
    Block_p->Parent_p      = Container_p;
    Container_p = Block_p;

    if (current_p[0] != '{') {NH_END(NH_JS_ERROR_UNEXPECTED_TOKEN)}

    char *tmp_p = Nh_JS_seekNext(current_p, 1);
    while (tmp_p != NULL) {
        if (*tmp_p != '}') {Block_p->parsableCount++;}
        tmp_p = Nh_JS_seekNext(Nh_JS_seekEndOfParsable(tmp_p, true), 1);
    }
   
    *depth_p += 1;
 
    if (Block_p->parsableCount > 0) 
    {
        Block_p->Parsables_p = Nh_allocate(sizeof(Nh_JS_Parsable) * Block_p->parsableCount);
        NH_CHECK_MEM(Block_p->Parsables_p)

        tmp_p = Nh_JS_seekNext(current_p, 1);

        int index = 0;
        while (tmp_p != NULL)
        {
            GET_TYPE_AND_PARSE(Block_p->Parsables_p[index], tmp_p, true)
            if (++index < Block_p->parsableCount) {tmp_p = Nh_JS_seekNext(Nh_JS_seekEndOfParsable(tmp_p, true), 1);}
            else {break;}
        }
    }

    *depth_p -= 1;

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseExpressionStatement(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_ExpressionStatement *ExpressionStatement_p = Parsable_p->Data_p;

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseStringLiteral(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()
 
    CONFIGURE(Parsable_p)
    Nh_JS_StringLiteral *StringLiteral_p = Parsable_p->Data_p;

    char terminator = '"';

    for (int i = 0; i < strlen(current_p); ++i) {
        if (current_p[i] == '"' || current_p[i] == 39) {terminator = current_p[i]; current_p = &current_p[i + 1]; break;}
    }

    int charCount = 0;
    for (charCount = 0; current_p[charCount] != terminator; ++charCount) {}

    if (charCount > 0) 
    {
        StringLiteral_p->value_p = Nh_allocate(sizeof(char) * (charCount + 1));
        NH_CHECK_MEM(StringLiteral_p->value_p)

        for (int i = 0; i < charCount; ++i) {StringLiteral_p->value_p[i] = current_p[i];}
        StringLiteral_p->value_p[charCount] = '\0';
    }
    else {NH_END(NH_ERROR_BAD_STATE)}

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseMemberExpression(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_MemberExpression *MemberExpression_p = Parsable_p->Data_p;

    char *parsableEnd_p = Nh_JS_seekEndOfParsable(current_p, true);
    
    int length = 0;
    for (length = 0; parsableEnd_p != &current_p[length]; ++length) {}

    if (length > 0) 
    {
        char *part_p = Nh_allocate(sizeof(char) * (length + 1));
        NH_CHECK_MEM(part_p)

        for (int i = 0; i < length; ++i) {part_p[i] = current_p[i];}
        part_p[length] = '\0';

        NH_CHECK(Nh_JS_parseRightMember(Maps_p, MemberExpression_p, part_p, depth_p, Container_p))
        NH_CHECK(Nh_JS_parseLeftMember(Maps_p, MemberExpression_p, part_p, depth_p, Container_p))

        Nh_free(part_p);
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseIdentifier(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()
 
    CONFIGURE(Parsable_p)
    Nh_JS_Identifier *Identifier_p = Parsable_p->Data_p;

    int charCount = 0;
    Nh_JS_getCurrentWord(current_p, &charCount, NULL, NULL);

    if (charCount > 0) 
    {
        Identifier_p->name_p = Nh_allocate(sizeof(char) * (charCount + 1)); 
        NH_CHECK_MEM(Identifier_p->name_p)

        Nh_JS_getCurrentWord(current_p, NULL, Identifier_p->name_p, NULL);
        Identifier_p->name_p[charCount] = '\0';
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseCallExpression(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_CallExpression *CallExpression_p = Parsable_p->Data_p;

    CallExpression_p->Arguments_p   = NULL;
    CallExpression_p->argumentCount = 0;

    char *parsableEnd_p = Nh_JS_seekEndOfParsable(current_p, true);
    
    int length = 0;
    for (length = 0; parsableEnd_p != &current_p[length]; ++length) {}

    if (length > 0) 
    {
        char *reverse_p = Nh_allocate(sizeof(char) * (length + 1));
        NH_CHECK_MEM(reverse_p)

        for (int i = 0; i < length; ++i) {reverse_p[i] = current_p[i];}
        reverse_p[length] = '\0';
        
        NH_CHECK(Nh_JS_parseArguments(Maps_p, CallExpression_p, reverse_p, depth_p, Container_p))

        for (int i = strlen(reverse_p); i >= 0; --i) {if (reverse_p[i] == '(') {reverse_p[i] = '\0';}}

        GET_TYPE_AND_PARSE(CallExpression_p->Parsable, reverse_p, true)

        Nh_free(reverse_p);
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseVariableDeclaration(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_VariableDeclaration *Declaration_p = Parsable_p->Data_p;
    Declaration_p->kind = Nh_JS_getCurrentKeyword(current_p);

    if (Declaration_p->kind != -1)
    {
        int length = 0;
        GET_WORD_LENGTH(length)
        NH_CHECK(Nh_JS_parseVariableDeclarator(Maps_p, Nh_JS_seekNext(&current_p[length], 0), &Declaration_p->Declarator, depth_p, Container_p, Declaration_p->kind))
    }
    else {
        NH_CHECK(Nh_JS_parseVariableDeclarator(Maps_p, current_p, &Declaration_p->Declarator, depth_p, Container_p, Declaration_p->kind))
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseVariableDeclarator(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_VariableDeclarator *Declarator_p, int *depth_p, Nh_JS_BlockStatement *Container_p, NH_JS_KEYWORD kind)
{
NH_BEGIN()

    int length = 0;
    GET_WORD_LENGTH(length)

    char replace = current_p[length];
    current_p[length] = '\0';

    GET_TYPE_AND_PARSE(Declarator_p->Identifier, current_p, true);
    if (Declarator_p->Identifier.type != NH_JS_PARSABLE_IDENTIFIER) {NH_END(NH_ERROR_BAD_STATE)}

    NH_CHECK(Nh_JS_addVariable(&Container_p->Variables, ((Nh_JS_Identifier*)Declarator_p->Identifier.Data_p)->name_p, *depth_p, kind))

    current_p[length] = replace; 
    current_p = Nh_JS_seekNext(&current_p[length], 0);

    if (current_p == NULL || current_p[0] != '=') {
        Declarator_p->Parsable.type = -1;
        NH_END(NH_SUCCESS)
    }

    current_p = Nh_JS_seekNext(&current_p[1], 0);
    GET_TYPE_AND_PARSE(Declarator_p->Parsable, current_p, true)

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseNumericLiteral(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    *((double*)Parsable_p->Data_p) = strtod(current_p, NULL);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseBooleanLiteral(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseConstructorCall(
    Nh_HashMaps *Maps_p, char *current_p, Nh_JS_Parsable *Parsable_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    CONFIGURE(Parsable_p)
    Nh_JS_ConstructorCall *ConstrCall_p = Parsable_p->Data_p;

    while (*current_p != 'n') {current_p = &current_p[1];}
    current_p = &current_p[3]; // jump over new keyword

    GET_TYPE_AND_PARSE(ConstrCall_p->CallExpression, current_p, true);
    if (ConstrCall_p->CallExpression.type != NH_JS_PARSABLE_CALL_EXPRESSION) {NH_END(NH_ERROR_BAD_STATE)}

NH_END(NH_SUCCESS)
}

// MICRO PARSING ===================================================================================

static NH_RESULT Nh_JS_parseArguments(
    Nh_HashMaps *Maps_p, Nh_JS_CallExpression *CallExpression_p, char *part_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    bool endingFound = false;
    char *begin_p = NULL;
    for (int i = strlen(part_p); i >= 0; --i) {
        if (part_p[i] == ')' && !endingFound) {part_p[i] = '\0'; endingFound = true;}
        if (part_p[i] == '(')                 {begin_p = &part_p[i];}
    }
    
    if (begin_p != NULL) {part_p = begin_p;}
    else {NH_END(NH_ERROR_BAD_STATE)}

    char *args_p = Nh_JS_seekNext(part_p, 1);

    while (args_p != NULL) 
    {
        if (*args_p == ',') {args_p = Nh_JS_seekNext(args_p, 1);}

        int charCount = 0;
        for (charCount = 0; args_p[charCount] != ',' && args_p[charCount] != '\0'; ++charCount) {}

        if (charCount > 0) {CallExpression_p->argumentCount += 1;}

        args_p = Nh_JS_seekNext(args_p, charCount);
    }

    if (CallExpression_p->argumentCount > 0) {
        CallExpression_p->Arguments_p = Nh_allocate(sizeof(Nh_JS_Parsable) * CallExpression_p->argumentCount);
        NH_CHECK_MEM(CallExpression_p->Arguments_p);
    }

    int index = 0;
    args_p = Nh_JS_seekNext(part_p, 1);
    while (args_p != NULL) 
    {
        if (*args_p == ',') {args_p = Nh_JS_seekNext(args_p, 1);}

        int charCount = 0;
        for (charCount = 0; args_p[charCount] != ',' && args_p[charCount] != '\0'; ++charCount) {}

        if (charCount > 0) 
        {
            bool multiParam = args_p[charCount] == ',';
            if (multiParam) {args_p[charCount] = '\0';}

            GET_TYPE_AND_PARSE(CallExpression_p->Arguments_p[index], args_p, true)
            index++;

            if (multiParam) {args_p[charCount] = ',';}
        }

        args_p = Nh_JS_seekNext(args_p, charCount);
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseParameters(
    Nh_HashMaps *Maps_p, Nh_JS_FunctionDeclaration *Function_p, char *current_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    char *tmp_p = current_p;

    while (tmp_p != NULL) 
    {
        int charCount = 0;
        bool end = false;
        for (charCount = 0; tmp_p[charCount] != ',' && tmp_p[charCount] != '\0'; ++charCount) {
            if (tmp_p[charCount] == ')') {end = true; break;}
        }

        if (charCount > 0) {Function_p->parameterCount += 1;}
     
        if (!end) {tmp_p = Nh_JS_seekNext(tmp_p, charCount);} 
        else      {tmp_p = NULL;}
    }

    if (Function_p->parameterCount > 0) {
        Function_p->Parameters_p = Nh_allocate(sizeof(Nh_JS_Parsable) * Function_p->parameterCount);
        NH_CHECK_MEM(Function_p->Parameters_p);
    }

    int index = 0;
    tmp_p = current_p;
    while (tmp_p != NULL) 
    {
        int charCount = 0;
        bool end = false;
        for (charCount = 0; tmp_p[charCount] != ',' && tmp_p[charCount] != '\0'; ++charCount) {
            if (tmp_p[charCount] == ')') {end = true; break;}
        }

        if (charCount > 0) 
        {
            char replace = current_p[charCount];
            tmp_p[charCount] = '\0';

            GET_TYPE_AND_PARSE(Function_p->Parameters_p[index], tmp_p, false);
            if (Function_p->Parameters_p[index].type != NH_JS_PARSABLE_IDENTIFIER) {NH_END(NH_ERROR_BAD_STATE)}

            tmp_p[charCount] = replace;

            NH_CHECK(Nh_JS_addVariable(
                &Container_p->Variables, 
                ((Nh_JS_Identifier*)Function_p->Parameters_p[index].Data_p)->name_p,
                *depth_p,
                -1
            ))
      
            index++;
        }

        if (!end) {tmp_p = Nh_JS_seekNext(tmp_p, charCount);} 
        else      {tmp_p = NULL;}
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseRightMember(
    Nh_HashMaps *Maps_p, Nh_JS_MemberExpression *MemberExpression_p, char *part_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    char *right_p = NULL;
    for (int i = strlen(part_p), charCount = 0; i >= 0; --i, charCount++) {if (part_p[i] == '.') {right_p = &part_p[i + 1]; break;}}
    if (right_p[strlen(right_p) - 1] == ')') {right_p[strlen(right_p) - 1] = '\0';}

    GET_TYPE_AND_PARSE(MemberExpression_p->Right, right_p, true)    

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_parseLeftMember(
    Nh_HashMaps *Maps_p, Nh_JS_MemberExpression *MemberExpression_p, char *part_p, int *depth_p, Nh_JS_BlockStatement *Container_p)
{
NH_BEGIN()

    for (int i = strlen(part_p); i >= 0; --i) {if (part_p[i] == '.') {part_p[i] = '\0'; break;}}
    if (*part_p == '(') {part_p = part_p + 1;}

    GET_TYPE_AND_PARSE(MemberExpression_p->Left, part_p, true)    

NH_END(NH_SUCCESS)
}

// ALLOCATE ========================================================================================

static NH_RESULT Nh_JS_allocateParsable(
    NH_JS_PARSABLE type, void **data_pp)
{
NH_BEGIN()

    switch (type) 
    {
        case NH_JS_PARSABLE_ASSIGNMENT_EXPRESSION : *data_pp = Nh_allocate(sizeof(Nh_JS_AssignmentExpression)); break;
        case NH_JS_PARSABLE_EXPRESSION_STATEMENT  : *data_pp = Nh_allocate(sizeof(Nh_JS_ExpressionStatement)); break;
        case NH_JS_PARSABLE_BLOCK_STATEMENT       : *data_pp = Nh_allocate(sizeof(Nh_JS_BlockStatement)); break;
        case NH_JS_PARSABLE_MEMBER_EXPRESSION     : *data_pp = Nh_allocate(sizeof(Nh_JS_MemberExpression)); break;
        case NH_JS_PARSABLE_STRING_LITERAL        : *data_pp = Nh_allocate(sizeof(Nh_JS_StringLiteral)); break;
        case NH_JS_PARSABLE_CALL_EXPRESSION       : *data_pp = Nh_allocate(sizeof(Nh_JS_CallExpression)); break;
        case NH_JS_PARSABLE_IDENTIFIER            : *data_pp = Nh_allocate(sizeof(Nh_JS_Identifier)); break;
        case NH_JS_PARSABLE_VARIABLE_DECLARATION  : *data_pp = Nh_allocate(sizeof(Nh_JS_VariableDeclaration)); break;
        case NH_JS_PARSABLE_VARIABLE_DECLARATOR   : *data_pp = NULL; break;
        case NH_JS_PARSABLE_WHILE_STATEMENT       : *data_pp = Nh_allocate(sizeof(Nh_JS_WhileStatement)); break;
        case NH_JS_PARSABLE_FOR_STATEMENT         : *data_pp = Nh_allocate(sizeof(Nh_JS_ForStatement)); break;
        case NH_JS_PARSABLE_FUNCTION_DECLARATION  : *data_pp = Nh_allocate(sizeof(Nh_JS_FunctionDeclaration)); break;
        case NH_JS_PARSABLE_IF_STATEMENT          : *data_pp = Nh_allocate(sizeof(Nh_JS_IfStatement)); break;
        case NH_JS_PARSABLE_LOGICAL_EXPRESSION    : *data_pp = Nh_allocate(sizeof(Nh_JS_LogicalExpression)); break;
        case NH_JS_PARSABLE_BINARY_EXPRESSION     : *data_pp = Nh_allocate(sizeof(Nh_JS_BinaryExpression)); break;
        case NH_JS_PARSABLE_BOOLEAN_TRUE          : *data_pp = NULL; break;
        case NH_JS_PARSABLE_BOOLEAN_FALSE         : *data_pp = NULL; break;
        case NH_JS_PARSABLE_NUMERIC_LITERAL       : *data_pp = Nh_allocate(sizeof(double)); break;
        case NH_JS_PARSABLE_NULL_LITERAL          :
        case NH_JS_PARSABLE_CONSTRUCTOR_CALL      : *data_pp = Nh_allocate(sizeof(Nh_JS_ConstructorCall)); break; 
        default                                   : NH_END(NH_ERROR_BAD_STATE)
    }

    if (type != NH_JS_PARSABLE_BOOLEAN_TRUE && type != NH_JS_PARSABLE_BOOLEAN_FALSE) {NH_CHECK_MEM(*data_pp)}

NH_END(NH_SUCCESS)
}

// GET =============================================================================================

static NH_JS_PARSABLE Nh_JS_getParsableType(
    Nh_HashMaps *Maps_p, char *current_p, bool ignoreCurved)
{
NH_BEGIN()

    NH_JS_KEYWORD keyword = Nh_JS_getCurrentKeyword(current_p);

    switch (keyword)
    {
        case NH_JS_KEYWORD_CONST    :
        case NH_JS_KEYWORD_VAR      : NH_END(NH_JS_PARSABLE_VARIABLE_DECLARATION)
        case NH_JS_KEYWORD_FUNCTION : NH_END(NH_JS_PARSABLE_FUNCTION_DECLARATION)
        case NH_JS_KEYWORD_IF       : 
        case NH_JS_KEYWORD_ELSE     : NH_END(NH_JS_PARSABLE_IF_STATEMENT)
        case NH_JS_KEYWORD_FOR      : NH_END(NH_JS_PARSABLE_FOR_STATEMENT)
        case NH_JS_KEYWORD_NEW      : NH_END(NH_JS_PARSABLE_CONSTRUCTOR_CALL)
        default                     : ;
    }

    if (keyword == -1)
    {
        char *parsableEnd_p = Nh_JS_seekEndOfParsable(current_p, ignoreCurved);

        int length = 0;
        for (length = 0; parsableEnd_p != &current_p[length]; ++length) {}

        if (Nh_JS_isBlockStatement(current_p, length))       {NH_END(NH_JS_PARSABLE_BLOCK_STATEMENT)}
        if (Nh_JS_isCallExpression(current_p, length))       {NH_END(NH_JS_PARSABLE_CALL_EXPRESSION)}
        if (Nh_JS_isBinaryExpression(current_p, length))     {NH_END(NH_JS_PARSABLE_BINARY_EXPRESSION)}
        if (Nh_JS_isStringLiteral(current_p, length))        {NH_END(NH_JS_PARSABLE_STRING_LITERAL)}
        if (Nh_JS_isAssignmentExpression(current_p, length)) {NH_END(NH_JS_PARSABLE_ASSIGNMENT_EXPRESSION)}
        if (Nh_JS_isMemberExpression(current_p, length))     {NH_END(NH_JS_PARSABLE_MEMBER_EXPRESSION)}
        if (Nh_JS_isNumericLiteral(current_p, length))       {NH_END(NH_JS_PARSABLE_NUMERIC_LITERAL)}

        char *p = Nh_JS_seekNext(current_p, 0);
        if (strlen(p) >= 4 && p[0] == 't' && p[1] == 'r' && p[2] == 'u' && p[3] == 'e')                {NH_END(NH_JS_PARSABLE_BOOLEAN_TRUE)}
        if (strlen(p) >= 5 && p[0] == 'f' && p[1] == 'a' && p[2] == 'l' && p[3] == 's' && p[4] == 'e') {NH_END(NH_JS_PARSABLE_BOOLEAN_FALSE)}

        NH_END(NH_JS_PARSABLE_IDENTIFIER)
    }

NH_END(NH_JS_PARSABLE_UNDEFINED)
}

static NH_JS_OPERATOR Nh_JS_getOperator(
    char *chars_p)
{
NH_BEGIN()

         if (chars_p[0] == '+' && chars_p[1] == '+') {return NH_JS_OPERATOR_INCREMENT;}
    else if (chars_p[0] == '-' && chars_p[1] == '-') {return NH_JS_OPERATOR_DECREMENT;}

    else if (chars_p[0] == '=' && chars_p[1] == '=') {return NH_JS_OPERATOR_EQUAL;}
    else if (chars_p[0] == '!' && chars_p[1] == '=') {return NH_JS_OPERATOR_UNEQUAL;}
    else if (chars_p[0] == '<' && chars_p[1] == '=') {return NH_JS_OPERATOR_LESS_EQUAL;}
    else if (chars_p[0] == '>' && chars_p[1] == '=') {return NH_JS_OPERATOR_GREATER_EQUAL;}

    else if (chars_p[0] == '-' && chars_p[1] == '=') {return NH_JS_OPERATOR_SUBTRACT_ASSIGN;}
    else if (chars_p[0] == '+' && chars_p[1] == '=') {return NH_JS_OPERATOR_ADD_ASSIGN;}
    else if (chars_p[0] == '*' && chars_p[1] == '=') {return NH_JS_OPERATOR_MULTIPLY_ASSIGN;}
    else if (chars_p[0] == '/' && chars_p[1] == '=') {return NH_JS_OPERATOR_DIVIDE_ASSIGN;}

    else if (chars_p[0] == '&' && chars_p[1] == '&') {return NH_JS_OPERATOR_SHORT_CIRCUIT_AND;}
    else if (chars_p[0] == '|' && chars_p[1] == '|') {return NH_JS_OPERATOR_SHORT_CIRCUIT_OR;}

    else if (chars_p[0] == '=')                      {return NH_JS_OPERATOR_ASSIGN;}
    else if (chars_p[0] == '<')                      {return NH_JS_OPERATOR_LESS;}
    else if (chars_p[0] == '>')                      {return NH_JS_OPERATOR_GREATER;}
    else if (chars_p[0] == '!')                      {return NH_JS_OPERATOR_NOT;}
    else if (chars_p[0] == '&')                      {return NH_JS_OPERATOR_AND;}
    else if (chars_p[0] == '|')                      {return NH_JS_OPERATOR_OR;}
    else if (chars_p[0] == '-')                      {return NH_JS_OPERATOR_SUBTRACT;}
    else if (chars_p[0] == '+')                      {return NH_JS_OPERATOR_ADD;}
    else if (chars_p[0] == '*')                      {return NH_JS_OPERATOR_MULTIPLY;}
    else if (chars_p[0] == '/')                      {return NH_JS_OPERATOR_DIVIDE;}

NH_END(-1)
}

static int NH_JS_Nh_JS_getOperatorWidth(
    NH_JS_OPERATOR operator)
{
NH_BEGIN()

    switch (operator)
    {
        case NH_JS_OPERATOR_INCREMENT         :
        case NH_JS_OPERATOR_DECREMENT         :
        case NH_JS_OPERATOR_SUBTRACT_ASSIGN   : 
        case NH_JS_OPERATOR_ADD_ASSIGN        : 
        case NH_JS_OPERATOR_MULTIPLY_ASSIGN   : 
        case NH_JS_OPERATOR_DIVIDE_ASSIGN     :
        case NH_JS_OPERATOR_EQUAL             :
        case NH_JS_OPERATOR_UNEQUAL           :
        case NH_JS_OPERATOR_LESS_EQUAL        :
        case NH_JS_OPERATOR_GREATER_EQUAL     :
        case NH_JS_OPERATOR_SHORT_CIRCUIT_AND :
        case NH_JS_OPERATOR_SHORT_CIRCUIT_OR  : NH_END(2)
    }

NH_END(1)
}

static int NH_JS_Nh_JS_getOperatorPriority(
    NH_JS_OPERATOR operator)
{
NH_BEGIN()

    switch (operator)
    {
        case NH_JS_OPERATOR_INCREMENT         :
        case NH_JS_OPERATOR_DECREMENT         :
        case NH_JS_OPERATOR_NOT               : NH_END(0)

        case NH_JS_OPERATOR_ASSIGN            : 
        case NH_JS_OPERATOR_SUBTRACT_ASSIGN   : 
        case NH_JS_OPERATOR_ADD_ASSIGN        : 
        case NH_JS_OPERATOR_MULTIPLY_ASSIGN   : 
        case NH_JS_OPERATOR_DIVIDE_ASSIGN     : NH_END(1)

        case NH_JS_OPERATOR_MULTIPLY          :
        case NH_JS_OPERATOR_DIVIDE            : NH_END(2)

        case NH_JS_OPERATOR_SUBTRACT          :
        case NH_JS_OPERATOR_ADD               : NH_END(3)

        case NH_JS_OPERATOR_EQUAL             :
        case NH_JS_OPERATOR_UNEQUAL           :
        case NH_JS_OPERATOR_LESS_EQUAL        :
        case NH_JS_OPERATOR_GREATER_EQUAL     :
        case NH_JS_OPERATOR_LESS              :
        case NH_JS_OPERATOR_GREATER           : NH_END(4)

        case NH_JS_OPERATOR_SHORT_CIRCUIT_AND :
        case NH_JS_OPERATOR_AND               : NH_END(5) 

        case NH_JS_OPERATOR_SHORT_CIRCUIT_OR  :
        case NH_JS_OPERATOR_OR                : NH_END(6)
    }

NH_END(-1)
}

static void Nh_JS_getCurrentWord(
    char *current_p, int *charCount_p, char *word_p, char *stop_p)
{
NH_BEGIN()

    char *tmp_p = Nh_JS_seekNext(current_p, 0);
    int charCount = 0;

    for (int i = 0; i < strlen(tmp_p); ++i) 
    {
        if (stop_p != NULL && *stop_p == tmp_p[i]) {break;}
        if (DONT_IGNORE(tmp_p[i]) && tmp_p[i] != ';') {
            if (word_p != NULL) {word_p[i] = tmp_p[i];}
            charCount += 1;
        }
        else {break;}
    }

    if (charCount_p != NULL) {*charCount_p = charCount;}

NH_SILENT_END()
}

static NH_JS_KEYWORD Nh_JS_getKeyword(
    char *word_p) 
{
NH_BEGIN()

    Nh_HashValue *HashValue_p;
    if (hashmap_get(Nh_getHashMaps()->JS.Keywords, word_p, (void**)(&HashValue_p)) != MAP_OK) {NH_END(-1)}

NH_END(HashValue_p->number)
}

static NH_JS_KEYWORD Nh_JS_getCurrentKeyword(
    char *current_p) 
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    char *word_p = NULL;

    int charCount = 0;
    Nh_JS_getCurrentWord(current_p, &charCount, NULL, NULL);

    if (charCount > 0) {word_p = Nh_allocate(sizeof(char) * (charCount + 1)); NH_CHECK_NULL(false, word_p)} 
    else {NH_END(false)}

    Nh_JS_getCurrentWord(current_p, NULL, word_p, NULL);
    word_p[charCount] = '\0';

    NH_JS_KEYWORD keyword = Nh_JS_getKeyword(word_p);

    Nh_free(word_p);

#include NH_DEFAULT_CHECK

NH_END(keyword)
}

static NH_RESULT Nh_JS_getForStatementType(
    char *current_p, NH_JS_ITERATION_STATEMENT *type_p)
{
NH_BEGIN()

    NH_JS_ITERATION_STATEMENT type;
    char *tmp_p = Nh_JS_seekNext(current_p, 3);

    if (*tmp_p != '(') 
    {
        char *word_p = NULL;
        int charCount = 0;
        Nh_JS_getCurrentWord(tmp_p, &charCount, NULL, NULL);

        if (charCount > 0) {word_p = Nh_allocate(sizeof(char) * (charCount + 1)); NH_CHECK_MEM(word_p)} 
        else {NH_END(NH_ERROR_BAD_STATE)}

        Nh_JS_getCurrentWord(tmp_p, NULL, word_p, NULL);
        word_p[charCount] = '\0';

        if (!strcmp("await", word_p)) {
            Nh_free(word_p);
            *type_p = NH_JS_ITERATION_STATEMENT_FOR_AWAIT_OF; 
            NH_END(NH_SUCCESS)
        } 
        else {
            Nh_free(word_p); 
            NH_END(NH_ERROR_BAD_STATE)
        }
    }

    for (int i = 0; i < strlen(tmp_p) && tmp_p[i] != ')'; ++i) 
    {
        if (tmp_p[i] == 'i' && tmp_p[i + 1] == 'n') {
            *type_p = NH_JS_ITERATION_STATEMENT_FOR_IN; NH_END(NH_SUCCESS)
        }
        if (tmp_p[i] == 'o' && tmp_p[i + 1] == 'f') {
            *type_p = NH_JS_ITERATION_STATEMENT_FOR_OF; NH_END(NH_SUCCESS)
        }
    }

    *type_p = NH_JS_ITERATION_STATEMENT_FOR;

NH_END(NH_SUCCESS)
}

// CHECK ===========================================================================================

static bool Nh_JS_isCallExpression(
    char *current_p, int length)
{
NH_BEGIN()

    bool params = false;

    for (int i = length - 1; i >= 0; --i) 
    {
        if (current_p[i] == ')') {params = true;}

        if (!params && DONT_IGNORE(current_p[i])) {NH_END(false)}
        if (params  && current_p[i] == '(') {NH_END(true)}
    }

NH_END(false)
}

static bool Nh_JS_isBlockStatement(
    char *current_p, int length)
{
NH_BEGIN()

    if (*current_p != '{' && current_p[length - 1] != '}') {NH_END(false)}

NH_END(true)
}

static bool Nh_JS_isMemberExpression(
    char *current_p, int length)
{
NH_BEGIN()

    bool first = false, second = false;

    for (int i = length - 1; i >= 0; --i) 
    {
        if (first && current_p[i] == '.')        {second = true;}
        if (!first && DONT_IGNORE(current_p[i])) {first = true;}

        if (first && !DONT_IGNORE(current_p[i])) {NH_END(false)}
        if (second && DONT_IGNORE(current_p[i])) {NH_END(true)}
    }

NH_END(false)
}

static bool Nh_JS_isStringLiteral(
    char *current_p, int length)
{
NH_BEGIN()

    bool first = false;

    for (int i = 0; i < length; ++i) 
    {
        if (!first && current_p[i] == '"')                              {first = true;}
        if (!first && current_p[i] == 39)                               {first = true;}
        if (!first && DONT_IGNORE(current_p[i]) && current_p[i] != ')') {NH_END(false)}

        if (first && current_p[i] == '"')       {NH_END(true);}
        if (first && current_p[i] == 39)        {NH_END(true);}
        if (first && DONT_IGNORE(current_p[i])) {NH_END(false)}
    }

NH_END(false)
}

static bool Nh_JS_isBinaryExpression(
    char *current_p, int length)
{
NH_BEGIN()

    int prio = -1;
    for (int i = 0; i < length; ++i) 
    {
        if (i + 1 < length) 
        {
            NH_JS_OPERATOR operator_ = Nh_JS_getOperator(&current_p[i]);
            if (operator_ == -1) {continue;}
            if (NH_JS_Nh_JS_getOperatorPriority(operator_) > 1) {NH_END(true)}
        }
    }

NH_END(prio > -1)
}

static bool Nh_JS_isAssignmentExpression(
    char *current_p, int length)
{
NH_BEGIN()

    for (int i = 0; i < length; ++i) 
    {
        if (i + 1 < length) 
        {
            NH_JS_OPERATOR operator_ = Nh_JS_getOperator(&current_p[i]);

            if (operator_ == NH_JS_OPERATOR_ASSIGN
            ||  operator_ == NH_JS_OPERATOR_SUBTRACT
            ||  operator_ == NH_JS_OPERATOR_ADD)
            {
                NH_END(true)
            }
        }
    }

NH_END(false)
}

static bool Nh_JS_isNumericLiteral(
    char *current_p, int length)
{
NH_BEGIN()

    current_p = Nh_JS_seekNext(current_p, 0);

NH_END(isdigit(current_p[0]))
}

// SEEK ============================================================================================

static char *Nh_JS_seekNext(
    char *begin_p, int offset)
{
NH_BEGIN()

    if (begin_p == NULL) {NH_END(NULL)}

    bool singleLineComment = false, multiLineComment = false;

    for (int i = offset; i < strlen(begin_p); ++i) 
    {
        if (!singleLineComment && !multiLineComment && begin_p[i] == '/' && i + 1 < strlen(begin_p) && begin_p[i + 1] == '/') {singleLineComment = true;}
        if (!singleLineComment && !multiLineComment && begin_p[i] == '/' && i + 1 < strlen(begin_p) && begin_p[i + 1] == '*') {multiLineComment  = true;}

        if (!singleLineComment && !multiLineComment && DONT_IGNORE(begin_p[i])) {NH_END(&begin_p[i])}

        if (singleLineComment && IS_LINE_BREAK(begin_p[i]))                  {singleLineComment = false;}
        if (multiLineComment  && begin_p[i - 1] == '*' && begin_p[i] == '/') {multiLineComment  = false;}
    }

NH_END(NULL)
}

static char *Nh_JS_seekEndOfParsable(
    char *current_p, bool ignoreCurved)
{
NH_BEGIN()

    int blockDepth = 0;
    bool block = false;

    for (int i = 0; i <= strlen(current_p); ++i) 
    {
        if (current_p[i] == '{' || (!ignoreCurved && current_p[i] == '(')) {blockDepth++; block = true;}
        if (current_p[i] == '}' || (!ignoreCurved && current_p[i] == ')')) {
            if (blockDepth == 0) {NH_END(&current_p[i])}
            blockDepth--;
        }
        if (!block && blockDepth == 0 && (current_p[i] == ';' || i == strlen(current_p))) {NH_END(&current_p[i])}
        if (block  && blockDepth == 0) {NH_END(&current_p[i])}
    }

NH_END(NULL)
}

// UNPARSE =========================================================================================

void Nh_JS_unparseScript(
    Nh_JS_Script *Script_p)
{   
NH_BEGIN()

    if (Script_p->Flags.loaded) {NH_SILENT_END()}

    Nh_JS_Parsable Parsable = {
        .type   = NH_JS_PARSABLE_BLOCK_STATEMENT, 
        .Data_p = &Script_p->Container,
    };
    Nh_JS_destroyParsable(&Parsable);

    Script_p->Flags.parsed = false;

NH_SILENT_END()
}

static void Nh_JS_destroyParsable(
    Nh_JS_Parsable *Parsable_p)
{
NH_BEGIN()

    switch (Parsable_p->type) 
    {
        case NH_JS_PARSABLE_ASSIGNMENT_EXPRESSION : 
            {
                Nh_JS_AssignmentExpression *AssignmentExpression_p = Parsable_p->Data_p;               

                Nh_JS_destroyParsable(&AssignmentExpression_p->Left);
                Nh_JS_destroyParsable(&AssignmentExpression_p->Right);

                Nh_free(AssignmentExpression_p);
            }
            break;

        case NH_JS_PARSABLE_EXPRESSION_STATEMENT : 
            {
                Nh_JS_ExpressionStatement *ExpressionStatement_p = Parsable_p->Data_p;
                Nh_JS_destroyParsable(&ExpressionStatement_p->Parsable);

                Nh_free(ExpressionStatement_p);
            }
            break;

        case NH_JS_PARSABLE_BLOCK_STATEMENT :
            {
                Nh_JS_BlockStatement *BlockStatement_p = Parsable_p->Data_p;

                for (int i = 0; i < BlockStatement_p->parsableCount; ++i) {
                    Nh_JS_destroyParsable(&BlockStatement_p->Parsables_p[i]);
                }

                Nh_destroyList(&BlockStatement_p->Variables, true);

                Nh_free(BlockStatement_p->Parsables_p);
                if (BlockStatement_p->Parent_p != NULL) {Nh_free(BlockStatement_p);}
            } 
            break;

        case NH_JS_PARSABLE_MEMBER_EXPRESSION : 
            {
                Nh_JS_MemberExpression *MemberExpression_p = Parsable_p->Data_p;

                Nh_JS_destroyParsable(&MemberExpression_p->Left);
                Nh_JS_destroyParsable(&MemberExpression_p->Right);

                Nh_free(MemberExpression_p);
            }
            break;

        case NH_JS_PARSABLE_STRING_LITERAL : 
            {
                Nh_JS_StringLiteral *StringLiteral_p = Parsable_p->Data_p;
                Nh_free(StringLiteral_p->value_p);
                Nh_free(StringLiteral_p);
            }
            break;

        case NH_JS_PARSABLE_CALL_EXPRESSION :
            {
                Nh_JS_CallExpression *CallExpression_p = Parsable_p->Data_p;

                for (int i = 0; i < CallExpression_p->argumentCount; ++i) {
                    Nh_JS_destroyParsable(&CallExpression_p->Arguments_p[i]);
                }
                Nh_JS_destroyParsable(&CallExpression_p->Parsable);

                Nh_free(CallExpression_p->Arguments_p);
                Nh_free(CallExpression_p);
            }
            break;

        case NH_JS_PARSABLE_IDENTIFIER : 
            {
                Nh_JS_Identifier *Identifier_p = Parsable_p->Data_p;
                Nh_free(Identifier_p->name_p);
                Nh_free(Identifier_p);
            }
            break;
        
        case NH_JS_PARSABLE_FUNCTION_DECLARATION : 
            {
                Nh_JS_FunctionDeclaration *Function_p = Parsable_p->Data_p;

                Nh_JS_destroyParsable(&Function_p->Name);
                for (int i = 0; i < Function_p->parameterCount; ++i) {
                    Nh_JS_destroyParsable(&Function_p->Parameters_p[i]);
                }
                Nh_JS_destroyParsable(&Function_p->Block);

                Nh_free(Function_p->Parameters_p);
                Nh_free(Function_p);
            }
            break;

        case NH_JS_PARSABLE_VARIABLE_DECLARATION :  
            {
                Nh_JS_VariableDeclaration *Declaration_p = Parsable_p->Data_p;

                Nh_JS_destroyParsable(&Declaration_p->Declarator.Identifier);
                Nh_JS_destroyParsable(&Declaration_p->Declarator.Parsable);

                Nh_free(Declaration_p);
            }
            break;

        case NH_JS_PARSABLE_FOR_STATEMENT :
            {
                Nh_JS_ForStatement *For_p = Parsable_p->Data_p;
               
                for (int i = 0; i < For_p->expressionCount; ++i) {
                    Nh_JS_destroyParsable(&For_p->Expressions_p[i]);
                } 
                Nh_JS_destroyParsable(&For_p->Statement);

                Nh_free(For_p->Expressions_p);
                Nh_free(For_p);
            }
            break;

        case NH_JS_PARSABLE_IF_STATEMENT :
            {
                Nh_JS_IfStatement *If_p = Parsable_p->Data_p;
               
                for (int i = 0; i < If_p->parsableCount; ++i) {
                    Nh_JS_destroyParsable(&If_p->Parsables_p[i]);
                } 

                Nh_free(If_p->Parsables_p);
                Nh_free(If_p);
            }
            break;

        case NH_JS_PARSABLE_BINARY_EXPRESSION :
            {
                Nh_JS_BinaryExpression *Binary_p = Parsable_p->Data_p;
               
                Nh_JS_destroyParsable(&Binary_p->Left);
                Nh_JS_destroyParsable(&Binary_p->Right);

                Nh_free(Binary_p);
            }
            break;

        case NH_JS_PARSABLE_NUMERIC_LITERAL :
            {
                Nh_free(Parsable_p->Data_p);
            }
            break;

        case NH_JS_PARSABLE_CONSTRUCTOR_CALL :
            {
                Nh_JS_ConstructorCall *ConstrCall_p = Parsable_p->Data_p;
                Nh_JS_destroyParsable(&ConstrCall_p->CallExpression);
                Nh_free(ConstrCall_p);
            }
            break;

        default: break;
    }

NH_SILENT_END()
}

