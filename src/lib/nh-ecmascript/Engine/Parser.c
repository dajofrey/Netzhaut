// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Parser.h"
#include "Lexer.h"

#include "../Common/IndexMap.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

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

// DATA ============================================================================================

const char *NH_ECMASCRIPT_RESERVED_WORDS_PP[] = {
    "await",
    "break",
    "case",
    "catch",
    "class",
    "const",
    "continue",
    "debugger",
    "default",
    "delete", 
    "do",
    "else",
    "enum",
    "export",
    "extends", 
    "false",
    "finally", 
    "for",
    "function",
    "if",
    "import", 
    "in",
    "instanceof",
    "new",
    "null",
    "return",
    "super",
    "switch", 
    "this",
    "throw",
    "true",
    "try",
    "typeof", 
    "var",
    "void", 
    "while", 
    "with", 
    "yield",
};

const char *NH_ECMASCRIPT_PARSE_NODE_NAMES_PP[] = {
    "ArrayLiteral",
    "ElementList",
    "AdditiveExpression",
    "Arguments",
    "ArrayBindingPattern",
    "ArrowFunction",
    "AssignmentExpression",
    "AssignmentOperator",
    "AsyncArrowFunction",
    "AsyncConciseBody",
    "AsyncFunctionBody",
    "AsyncFunctionDeclaration",
    "AsyncFunctionExpression",
    "AsyncGeneratorDeclaration",
    "AsyncGeneratorExpression",
    "BindingElement",
    "BindingElementList",
    "BindingElisionElement",
    "BindingIdentifier",
    "BindingList",
    "BindingPattern",
    "BindingProperty",
    "BindingPropertyList",
    "BindingRestElement",
    "BindingRestProperty",
    "BitwiseANDExpression",
    "BitwiseORExpression",
    "BitwiseXORExpression",
    "Block",
    "BlockStatement",
    "BooleanLiteral",
    "BreakStatement",
    "CallExpression",
    "CaseBlock",
    "CaseClause",
    "CaseClauses",
    "Catch",
    "ClassDeclaration",
    "ClassExpression",
    "ConciseBody",
    "ConditionalExpression",
    "ContinueStatement",
    "CoverCallExpressionAndAsyncArrowHead",
    "CoverParenthesizedExpressionAndArrowParameterList",
    "DebuggerStatement",
    "Declaration",
    "DefaultClause",
    "DoWhileStatement",
    "Elision",
    "EmptyStatement",
    "EqualityExpression",
    "ExponentiationExpression",
    "ExportDeclaration",
    "ExportFromClause",
    "Expression",
    "ExpressionStatement",
    "ForBinding",
    "ForDeclaration",
    "ForInOfStatement",
    "ForStatement",
    "IterationStatement",
    "BreakableStatement",
    "FormalParameter",
    "FormalParameterList",
    "FormalParameters",
    "FunctionBody",
    "FunctionDeclaration",
    "FunctionExpression",
    "FunctionStatementList",
    "GeneratorDeclaration",
    "GeneratorExpression",
    "HoistableDeclaration",
    "Identifier",
    "IdentifierReference",
    "IfStatement",
    "ImportDeclaration",
    "Initializer",
    "LabelIdentifier",
    "LabelledItem",
    "LabelledStatement",
    "LeftHandSideExpression",
    "LetOrConst",
    "LexicalBinding",
    "LexicalDeclaration",
    "Literal",
    "LogicalANDExpression",
    "LogicalORExpression",
    "MemberExpression",
    "Module",
    "ModuleBody",
    "ModuleItem",
    "ModuleItemList",
    "MultiplicativeExpression",
    "NamedExports",
    "NewExpression",
    "NullLiteral",
    "NumericLiteral",
    "ObjectBindingPattern",
    "OptionalChain",
    "OptionalExpression",
    "PrimaryExpression",
    "RelationalExpression",
    "ReturnStatement",
    "Script",
    "ScriptBody",
    "ShiftExpression",
    "ShortCircuitExpression",
    "SpreadElement",
    "SingleNameBinding",
    "Statement",
    "StatementList",
    "StatementListItem",
    "StringLiteral",
    "SwitchStatement",
    "ThrowStatement",
    "TryStatement",
    "UnaryExpression",
    "UpdateExpression",
    "VariableDeclaration",
    "VariableDeclarationList",
    "VariableStatement",
    "WhileStatement",
    "WithStatement",
};

size_t NH_ECMASCRIPT_RESERVED_WORDS_PP_COUNT = sizeof(NH_ECMASCRIPT_RESERVED_WORDS_PP) / sizeof(NH_ECMASCRIPT_RESERVED_WORDS_PP[0]);
size_t NH_ECMASCRIPT_PARSE_NODE_NAMES_PP_COUNT = sizeof(NH_ECMASCRIPT_PARSE_NODE_NAMES_PP) / sizeof(NH_ECMASCRIPT_PARSE_NODE_NAMES_PP[0]);

// DECLARE =========================================================================================

typedef struct nh_ecmascript_Parser {
    nh_ecmascript_InputElement *Tokens_p;
    int unparsed;
} nh_ecmascript_Parser;

static nh_ecmascript_ParseResult nh_ecmascript_parseStatementList(
    nh_ecmascript_Parser *Parser_p
);

static nh_ecmascript_ParseResult nh_ecmascript_parseStatement(
    nh_ecmascript_Parser *Parser_p
);

static nh_ecmascript_ParseResult nh_ecmascript_parseIdentifier(
    nh_ecmascript_Parser *Parser_p
);

static nh_ecmascript_ParseResult nh_ecmascript_parseAssignmentExpression(
    nh_ecmascript_Parser *Parser_p
);

static nh_ecmascript_ParseResult nh_ecmascript_parseForStatement(
    nh_ecmascript_Parser *Parser_p
);

static nh_ecmascript_ParseResult nh_ecmascript_parseAssignmentOperator(
    nh_ecmascript_Parser *Parser_p
);

static nh_ecmascript_ParseResult nh_ecmascript_parseExpression(
    nh_ecmascript_Parser *Parser_p
);

// HELPER ==========================================================================================

static nh_ecmascript_ParseResult nh_ecmascript_initParseResult(
    nh_ecmascript_ParseNode *Node_p)
{
    nh_ecmascript_ParseResult Result;
    Result.Node_p = Node_p;
    Result.SyntaxErrors = nh_core_initList(8);

    return Result;
}

static nh_ecmascript_ParseNode *nh_ecmascript_allocateNonTerminalParseNode(
    const char *name_p, int chunkSize)
{
    unsigned int *index_p = nh_core_getFromHashMap(&NH_ECMASCRIPT_INDEXMAP.ParseNodeNames, (char*)name_p);
    if (index_p == NULL) {return NULL;}

    nh_ecmascript_ParseNode *Node_p = (nh_ecmascript_ParseNode*)nh_core_allocate(sizeof(nh_ecmascript_ParseNode));
    Node_p->Children = nh_core_initList(chunkSize);
    Node_p->Value_p  = NULL;
    Node_p->type     = *index_p;

    return Node_p;
}

static nh_ecmascript_ParseNode *nh_ecmascript_allocateTerminalParseNode(
    nh_ecmascript_InputElement *InputElement_p)
{
    nh_ecmascript_ParseNode *Node_p = (nh_ecmascript_ParseNode*)nh_core_allocate(sizeof(nh_ecmascript_ParseNode));
    Node_p->Children = nh_core_initList(1);
    Node_p->Value_p  = InputElement_p;
    Node_p->type     = -1;

    return Node_p;
}

static void nh_ecmascript_freeParseNode(
    nh_ecmascript_ParseNode *Node_p)
{
    return;
}

static nh_ecmascript_Parser nh_ecmascript_advanceParser(
    nh_ecmascript_Parser Parser, int steps)
{
    Parser.unparsed -= steps;
    Parser.Tokens_p  = Parser.Tokens_p + steps;
 
    return Parser;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseEasyRecursiveProductionStep(
    nh_ecmascript_Parser *Parser_p, nh_ecmascript_ParseNode *Parent_p, char *name_p, bool commaSeparated, 
    nh_ecmascript_ParseResult (*parseItem_f)(nh_ecmascript_Parser *Parser_p)) 
{
    if (commaSeparated && Parent_p != NULL) {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] != ',') {
            return nh_ecmascript_initParseResult(Parent_p);
        }
        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
    }

    nh_ecmascript_ParseResult Result = parseItem_f(Parser_p);
    if (Result.Node_p == NULL || Result.SyntaxErrors.size > 0) {
        return nh_ecmascript_initParseResult(Parent_p);
    }

    if (Parent_p == NULL) {
        Parent_p = nh_ecmascript_allocateNonTerminalParseNode(name_p, 2);
        Parent_p->Children = nh_core_initList(8);
        nh_core_appendToList(&Parent_p->Children, Result.Node_p);
    }
    else {
        nh_core_appendToList(&Parent_p->Children, Result.Node_p);
    }

    return nh_ecmascript_parseEasyRecursiveProductionStep(Parser_p, Parent_p, name_p, commaSeparated, parseItem_f);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseEasyRecursiveProduction(
    nh_ecmascript_Parser *Parser_p, char *name_p, bool commaSeparated, nh_ecmascript_ParseResult (*parseItem_f)(nh_ecmascript_Parser *Parser_p))
{
    nh_ecmascript_Parser LocalParser = *Parser_p;

    nh_ecmascript_ParseResult Result = nh_ecmascript_parseEasyRecursiveProductionStep(
        &LocalParser, NULL, name_p, commaSeparated, parseItem_f 
    );

    if (Result.Node_p != NULL && Result.SyntaxErrors.size <= 0) {
        *Parser_p = LocalParser;
    }

    return Result;
}

// PARSER ==========================================================================================

static nh_ecmascript_ParseResult nh_ecmascript_parseBlock(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.p[0] != '{') {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_Parser LocalParser = nh_ecmascript_advanceParser(*Parser_p, 1);
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseStatementList(&LocalParser);

    if (Result.Node_p != NULL)
    {
        if (LocalParser.unparsed <= 0 || LocalParser.Tokens_p[0].String.p[0] != '}') {
            nh_ecmascript_freeParseNode(Result.Node_p);
            return nh_ecmascript_initParseResult(NULL);
        }

        nh_ecmascript_ParseNode *Block_p = nh_ecmascript_allocateNonTerminalParseNode("Block", 3);
        nh_ecmascript_ParseNode *OpenBracket_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_ecmascript_ParseNode *CloseBracket_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        nh_core_appendToList(&Block_p->Children, OpenBracket_p);
        nh_core_appendToList(&Block_p->Children, Result.Node_p);
        nh_core_appendToList(&Block_p->Children, CloseBracket_p);

        *Parser_p = nh_ecmascript_advanceParser(LocalParser, 1);
        return nh_ecmascript_initParseResult(Block_p);
    }

    // empty block
    else if (LocalParser.unparsed > 0 && *LocalParser.Tokens_p[0].String.p == '}')
    {
        nh_ecmascript_ParseNode *Block_p = nh_ecmascript_allocateNonTerminalParseNode("Block", 2);
        nh_ecmascript_ParseNode *OpenBracket_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_ecmascript_ParseNode *CloseBracket_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        nh_core_appendToList(&Block_p->Children, OpenBracket_p);
        nh_core_appendToList(&Block_p->Children, CloseBracket_p);

        *Parser_p = nh_ecmascript_advanceParser(LocalParser, 1);
        return nh_ecmascript_initParseResult(Block_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseBlockStatement(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseBlock(Parser_p);

    if (Result.Node_p != NULL) {
        nh_ecmascript_ParseNode *BlockStatement_p = nh_ecmascript_allocateNonTerminalParseNode("BlockStatement", 1);
        nh_core_appendToList(&BlockStatement_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(BlockStatement_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseIdentifierReference(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_initParseResult(NULL);

    if (Parser_p->unparsed <= 0) {return Result;}

    if (!strcmp("yield", Parser_p->Tokens_p[0].String.p) || !strcmp("await", Parser_p->Tokens_p[0].String.p)) 
    {
        nh_ecmascript_ParseNode *IdentifierReference_p = nh_ecmascript_allocateNonTerminalParseNode("IdentifierReference", 1);
        nh_ecmascript_ParseNode *YieldOrAwait_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&IdentifierReference_p->Children, YieldOrAwait_p);

        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(IdentifierReference_p);
    }
    else {

        Result = nh_ecmascript_parseIdentifier(Parser_p);

        if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
            nh_ecmascript_ParseNode *IdentifierReference_p = nh_ecmascript_allocateNonTerminalParseNode("IdentifierReference", 1);
            nh_core_appendToList(&IdentifierReference_p->Children, Result.Node_p);
            return nh_ecmascript_initParseResult(IdentifierReference_p);
        }
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseNumericLiteral(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed > 0 && nh_ecmascript_isNumericToken(&Parser_p->Tokens_p[0])) 
    {
        nh_ecmascript_ParseNode *NumericLiteral_p = nh_ecmascript_allocateNonTerminalParseNode("NumericLiteral", 1);
        nh_ecmascript_ParseNode *Number_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&NumericLiteral_p->Children, Number_p);

        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(NumericLiteral_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseBooleanLiteral(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed > 0 && (!strcmp("true", Parser_p->Tokens_p[0].String.p) || !strcmp("false", Parser_p->Tokens_p[0].String.p))) 
    {
        nh_ecmascript_ParseNode *BooleanLiteral_p = nh_ecmascript_allocateNonTerminalParseNode("BooleanLiteral", 1);
        nh_ecmascript_ParseNode *TrueOrFalse_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&BooleanLiteral_p->Children, TrueOrFalse_p);

        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(BooleanLiteral_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseNullLiteral(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed > 0 && !strcmp("null", Parser_p->Tokens_p[0].String.p)) 
    {
        nh_ecmascript_ParseNode *NullLiteral_p = nh_ecmascript_allocateNonTerminalParseNode("NullLiteral", 1);
        nh_ecmascript_ParseNode *Null_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&NullLiteral_p->Children, Null_p);

        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(NullLiteral_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseStringLiteral(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_STRING_LITERAL) 
    {
        nh_ecmascript_ParseNode *StringLiteral_p = nh_ecmascript_allocateNonTerminalParseNode("StringLiteral", 1);
        nh_ecmascript_ParseNode *String_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&StringLiteral_p->Children, String_p);

        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(StringLiteral_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseElision(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.p[0] != ',') {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_Parser LocalParser = *Parser_p;
    int commaCount = 0;
    nh_ecmascript_ParseNode *Elision_p = nh_ecmascript_allocateNonTerminalParseNode("Elision", 2);

    while (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ',') {
        nh_ecmascript_ParseNode *Comma_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);
        nh_core_appendToList(&Elision_p->Children, Comma_p);
        LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);
        commaCount++;
    }

    if (commaCount <= 0) {return nh_ecmascript_initParseResult(NULL);}    
    *Parser_p = LocalParser;
    return nh_ecmascript_initParseResult(Elision_p);
}

static bool nh_ecmascript_consumeDots(
    nh_ecmascript_Parser *Parser_p, nh_ecmascript_ParseNode *Parent_p)
{
    // Accept either a single token "..." or three consecutive '.' tokens
    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "...")) {
        nh_ecmascript_ParseNode *DotsToken_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_core_appendToList(&Parent_p->Children, DotsToken_p);
        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return true;
    }

    if (Parser_p->unparsed >= 3 && Parser_p->Tokens_p[0].String.p[0] == '.' && Parser_p->Tokens_p[1].String.p[0] == '.' && Parser_p->Tokens_p[2].String.p[0] == '.') {
        nh_ecmascript_ParseNode *Dot1_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_ecmascript_ParseNode *Dot2_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[1]);
        nh_ecmascript_ParseNode *Dot3_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[2]);
        nh_core_appendToList(&Parent_p->Children, Dot1_p);
        nh_core_appendToList(&Parent_p->Children, Dot2_p);
        nh_core_appendToList(&Parent_p->Children, Dot3_p);
        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 3);
        return true;
    }

    return false;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseSpreadElement(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseNode *SpreadElement_p = nh_ecmascript_allocateNonTerminalParseNode("SpreadElement", 2);
    nh_ecmascript_Parser LocalParser = *Parser_p;

    if (!nh_ecmascript_consumeDots(&LocalParser, SpreadElement_p)) {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_ParseResult Arg = nh_ecmascript_parseAssignmentExpression(&LocalParser);
    if (Arg.Node_p == NULL || Arg.SyntaxErrors.size > 0) {
        nh_ecmascript_freeParseNode(SpreadElement_p);
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_core_appendToList(&SpreadElement_p->Children, Arg.Node_p);
    *Parser_p = LocalParser;
    return nh_ecmascript_initParseResult(SpreadElement_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseElementList(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseNode *ElementList_p = nh_ecmascript_allocateNonTerminalParseNode("ElementList", 4);

    // Optional leading Elision
    nh_ecmascript_ParseResult LeadElision = nh_ecmascript_parseElision(&LocalParser);
    if (LeadElision.Node_p != NULL && LeadElision.SyntaxErrors.size == 0) {
        nh_core_appendToList(&ElementList_p->Children, LeadElision.Node_p);
    }

    // First element: AssignmentExpression or SpreadElement
    nh_ecmascript_ParseResult First = nh_ecmascript_parseSpreadElement(&LocalParser);
    if (First.Node_p == NULL) { First = nh_ecmascript_parseAssignmentExpression(&LocalParser); }
    if (First.Node_p == NULL || First.SyntaxErrors.size > 0) {
        nh_ecmascript_freeParseNode(ElementList_p);
        return nh_ecmascript_initParseResult(NULL);
    }
    nh_core_appendToList(&ElementList_p->Children, First.Node_p);

    // Zero or more: , Elision_opt (AssignmentExpression | SpreadElement)
    while (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ',') {
        nh_ecmascript_ParseNode *Comma_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);
        LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);
        nh_core_appendToList(&ElementList_p->Children, Comma_p);

        nh_ecmascript_ParseResult MidElision = nh_ecmascript_parseElision(&LocalParser);
        if (MidElision.Node_p != NULL && MidElision.SyntaxErrors.size == 0) {
            nh_core_appendToList(&ElementList_p->Children, MidElision.Node_p);
        }

        nh_ecmascript_ParseResult Next = nh_ecmascript_parseSpreadElement(&LocalParser);
        if (Next.Node_p == NULL) { Next = nh_ecmascript_parseAssignmentExpression(&LocalParser); }
        if (Next.Node_p == NULL || Next.SyntaxErrors.size > 0) {
            nh_ecmascript_freeParseNode(ElementList_p);
            return nh_ecmascript_initParseResult(NULL);
        }
        nh_core_appendToList(&ElementList_p->Children, Next.Node_p);
    }

    *Parser_p = LocalParser;
    return nh_ecmascript_initParseResult(ElementList_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseArrayLiteral(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.p[0] != '[') {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_Parser LocalParser = nh_ecmascript_advanceParser(*Parser_p, 1);
    nh_ecmascript_ParseNode *ArrayLiteral_p = nh_ecmascript_allocateNonTerminalParseNode("ArrayLiteral", 6);
    nh_ecmascript_ParseNode *LeftBracket_p  = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
    nh_core_appendToList(&ArrayLiteral_p->Children, LeftBracket_p);

    // Case: immediate closing ']'
    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ']') {
        nh_ecmascript_ParseNode *RightBracket_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);
        nh_core_appendToList(&ArrayLiteral_p->Children, RightBracket_p);
        *Parser_p = nh_ecmascript_advanceParser(LocalParser, 1);
        return nh_ecmascript_initParseResult(ArrayLiteral_p);
    }

    // Try ElementList
    nh_ecmascript_ParseResult Elements = nh_ecmascript_parseElementList(&LocalParser);
    if (Elements.Node_p != NULL && Elements.SyntaxErrors.size == 0) {
        nh_core_appendToList(&ArrayLiteral_p->Children, Elements.Node_p);

        // Optional trailing comma
        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ',') {
            nh_ecmascript_ParseNode *Comma_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);
            LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);
            nh_core_appendToList(&ArrayLiteral_p->Children, Comma_p);

            // Optional trailing elision after the final comma
            nh_ecmascript_ParseResult TrailElision = nh_ecmascript_parseElision(&LocalParser);
            if (TrailElision.Node_p != NULL && TrailElision.SyntaxErrors.size == 0) {
                nh_core_appendToList(&ArrayLiteral_p->Children, TrailElision.Node_p);
            }
        }

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ']') {
            nh_ecmascript_ParseNode *RightBracket_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);
            nh_core_appendToList(&ArrayLiteral_p->Children, RightBracket_p);
            *Parser_p = nh_ecmascript_advanceParser(LocalParser, 1);
            return nh_ecmascript_initParseResult(ArrayLiteral_p);
        }

        nh_ecmascript_freeParseNode(ArrayLiteral_p);
        return nh_ecmascript_initParseResult(NULL);
    }

    // Try only elision followed by ']'
    nh_ecmascript_Parser OnlyElisionParser = LocalParser;
    nh_ecmascript_ParseResult OnlyElision = nh_ecmascript_parseElision(&OnlyElisionParser);
    if (OnlyElision.Node_p != NULL && OnlyElision.SyntaxErrors.size == 0) {
        if (OnlyElisionParser.unparsed > 0 && OnlyElisionParser.Tokens_p[0].String.p[0] == ']') {
            nh_core_appendToList(&ArrayLiteral_p->Children, OnlyElision.Node_p);
            nh_ecmascript_ParseNode *RightBracket_p = nh_ecmascript_allocateTerminalParseNode(&OnlyElisionParser.Tokens_p[0]);
            nh_core_appendToList(&ArrayLiteral_p->Children, RightBracket_p);
            *Parser_p = nh_ecmascript_advanceParser(OnlyElisionParser, 1);
            return nh_ecmascript_initParseResult(ArrayLiteral_p);
        }
    }

    nh_ecmascript_freeParseNode(ArrayLiteral_p);
    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseLiteral(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseNullLiteral(Parser_p);

    if (Result.Node_p != NULL) {
        nh_ecmascript_ParseNode *Literal_p = nh_ecmascript_allocateNonTerminalParseNode("Literal", 1);
        nh_core_appendToList(&Literal_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(Literal_p);
    }

    Result = nh_ecmascript_parseBooleanLiteral(Parser_p);

    if (Result.Node_p != NULL) {
        nh_ecmascript_ParseNode *Literal_p = nh_ecmascript_allocateNonTerminalParseNode("Literal", 1);
        nh_core_appendToList(&Literal_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(Literal_p);
    }

    Result = nh_ecmascript_parseNumericLiteral(Parser_p);

    if (Result.Node_p != NULL) {
        nh_ecmascript_ParseNode *Literal_p = nh_ecmascript_allocateNonTerminalParseNode("Literal", 1);
        nh_core_appendToList(&Literal_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(Literal_p);
    }

    Result = nh_ecmascript_parseStringLiteral(Parser_p);

    if (Result.Node_p != NULL) {
        nh_ecmascript_ParseNode *Literal_p = nh_ecmascript_allocateNonTerminalParseNode("Literal", 1);
        nh_core_appendToList(&Literal_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(Literal_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parsePrimaryExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_initParseResult(NULL);
    Result = nh_ecmascript_parseArrayLiteral(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *PrimaryExpression_p = nh_ecmascript_allocateNonTerminalParseNode("PrimaryExpression", 1);
        nh_core_appendToList(&PrimaryExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(PrimaryExpression_p);
    }

    if (!strcmp("this", Parser_p->Tokens_p[0].String.p)) 
    {
        nh_ecmascript_ParseNode *PrimaryExpression_p = nh_ecmascript_allocateNonTerminalParseNode("PrimaryExpression", 1);
        nh_ecmascript_ParseNode *This_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&PrimaryExpression_p->Children, This_p);

        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(PrimaryExpression_p);
    }

    Result = nh_ecmascript_parseIdentifierReference(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *PrimaryExpression_p = nh_ecmascript_allocateNonTerminalParseNode("PrimaryExpression", 1);
        nh_core_appendToList(&PrimaryExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(PrimaryExpression_p);
    }

    Result = nh_ecmascript_parseLiteral(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *PrimaryExpression_p = nh_ecmascript_allocateNonTerminalParseNode("PrimaryExpression", 1);
        nh_core_appendToList(&PrimaryExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(PrimaryExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseMemberExpressionRecursively(
    nh_ecmascript_Parser *Parser_p, nh_ecmascript_ParseNode *MemberExpression_p)
{
    if (MemberExpression_p != NULL)
    {
        // Dot property access: obj.prop
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == '.') 
        {
            nh_ecmascript_Parser NextParser = nh_ecmascript_advanceParser(*Parser_p, 1);

            if (NextParser.unparsed > 0 && NextParser.Tokens_p[0].type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME)
            {
                nh_ecmascript_ParseNode *NewMemberExpression_p = nh_ecmascript_allocateNonTerminalParseNode("MemberExpression", 1);
                nh_ecmascript_ParseNode *FullStop_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
                nh_ecmascript_ParseNode *IdentifierName_p = nh_ecmascript_allocateTerminalParseNode(&NextParser.Tokens_p[0]);

                nh_core_appendToList(&NewMemberExpression_p->Children, MemberExpression_p);
                nh_core_appendToList(&NewMemberExpression_p->Children, FullStop_p);
                nh_core_appendToList(&NewMemberExpression_p->Children, IdentifierName_p);

                *Parser_p = nh_ecmascript_advanceParser(NextParser, 1);
                return nh_ecmascript_parseMemberExpressionRecursively(Parser_p, NewMemberExpression_p);
            }
        }

        // Bracket property access: obj[expr]
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == '[')
        {
            nh_ecmascript_Parser NextParser = nh_ecmascript_advanceParser(*Parser_p, 1);
            nh_ecmascript_ParseResult IndexExpr = nh_ecmascript_parseExpression(&NextParser);
            if (IndexExpr.Node_p != NULL && IndexExpr.SyntaxErrors.size == 0)
            {
                if (NextParser.unparsed > 0 && NextParser.Tokens_p[0].String.p[0] == ']')
                {
                    nh_ecmascript_ParseNode *NewMemberExpression_p = nh_ecmascript_allocateNonTerminalParseNode("MemberExpression", 4);
                    nh_ecmascript_ParseNode *LeftBracket_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
                    nh_ecmascript_ParseNode *RightBracket_p = nh_ecmascript_allocateTerminalParseNode(&NextParser.Tokens_p[0]);

                    nh_core_appendToList(&NewMemberExpression_p->Children, MemberExpression_p);
                    nh_core_appendToList(&NewMemberExpression_p->Children, LeftBracket_p);
                    nh_core_appendToList(&NewMemberExpression_p->Children, IndexExpr.Node_p);
                    nh_core_appendToList(&NewMemberExpression_p->Children, RightBracket_p);

                    *Parser_p = nh_ecmascript_advanceParser(NextParser, 1);
                    return nh_ecmascript_parseMemberExpressionRecursively(Parser_p, NewMemberExpression_p);
                }

                nh_ecmascript_freeParseNode(IndexExpr.Node_p);
            }
        }
    }
    else {

        nh_ecmascript_ParseResult Result = nh_ecmascript_parsePrimaryExpression(Parser_p);
    
        if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
            nh_ecmascript_ParseNode *MemberExpression_p = nh_ecmascript_allocateNonTerminalParseNode("MemberExpression", 1);
            nh_core_appendToList(&MemberExpression_p->Children, Result.Node_p);
            return nh_ecmascript_parseMemberExpressionRecursively(Parser_p, MemberExpression_p);
        }
    }

    return nh_ecmascript_initParseResult(MemberExpression_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseMemberExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseMemberExpressionRecursively(&LocalParser, NULL);
    
    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        *Parser_p = LocalParser;
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseOptionalChain(
    nh_ecmascript_Parser *Parser_p)
{
    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseOptionalExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseMemberExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        nh_ecmascript_ParseResult OptionalChainResult = nh_ecmascript_parseOptionalChain(&LocalParser);

        if (OptionalChainResult.Node_p != NULL && OptionalChainResult.SyntaxErrors.size == 0) 
        {
            nh_ecmascript_ParseNode *OptionalExpression_p = nh_ecmascript_allocateNonTerminalParseNode("OptionalExpression", 2);
            nh_core_appendToList(&OptionalExpression_p->Children, Result.Node_p);
            nh_core_appendToList(&OptionalExpression_p->Children, OptionalChainResult.Node_p);
            *Parser_p = LocalParser;
            return nh_ecmascript_initParseResult(OptionalExpression_p);
        }
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseNewExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseMemberExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *NewExpression_p = nh_ecmascript_allocateNonTerminalParseNode("NewExpression", 1);
        nh_core_appendToList(&NewExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(NewExpression_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseArguments(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.p[0] != '(') {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_Parser LocalParser = nh_ecmascript_advanceParser(*Parser_p, 1);

    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ')') 
    {
        nh_ecmascript_ParseNode *Arguments_p    = nh_ecmascript_allocateNonTerminalParseNode("Arguments", 1);
        nh_ecmascript_ParseNode *LeftBracket_p  = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_ecmascript_ParseNode *RightBracket_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        nh_core_appendToList(&Arguments_p->Children, LeftBracket_p);
        nh_core_appendToList(&Arguments_p->Children, RightBracket_p);

        *Parser_p = nh_ecmascript_advanceParser(LocalParser, 1);
        return nh_ecmascript_initParseResult(Arguments_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseCoverCallExpressionAndAsyncArrowHead(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult MemberExpression = nh_ecmascript_parseMemberExpression(Parser_p);

    if (MemberExpression.Node_p != NULL && MemberExpression.SyntaxErrors.size == 0) 
    {
        nh_ecmascript_ParseResult Arguments = nh_ecmascript_parseArguments(Parser_p);

        if (Arguments.Node_p != NULL && Arguments.SyntaxErrors.size == 0) 
        {
            nh_ecmascript_ParseNode *CoverCallExpressionAndAsyncArrowHead_p = nh_ecmascript_allocateNonTerminalParseNode("CoverCallExpressionAndAsyncArrowHead", 2);
            nh_core_appendToList(&CoverCallExpressionAndAsyncArrowHead_p->Children, MemberExpression.Node_p);
            nh_core_appendToList(&CoverCallExpressionAndAsyncArrowHead_p->Children, Arguments.Node_p);
            return nh_ecmascript_initParseResult(CoverCallExpressionAndAsyncArrowHead_p);
        }

        nh_ecmascript_freeParseNode(MemberExpression.Node_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseCallExpressionRecursively(
    nh_ecmascript_Parser *Parser_p, nh_ecmascript_ParseNode *PreviousCallExpression_p)
{
    if (PreviousCallExpression_p != NULL)
    {
        nh_ecmascript_ParseResult Arguments = nh_ecmascript_parseArguments(Parser_p);
    
        if (Arguments.Node_p != NULL && Arguments.SyntaxErrors.size == 0) {
            nh_ecmascript_ParseNode *CallExpression_p = nh_ecmascript_allocateNonTerminalParseNode("CallExpression", 2);
            nh_core_appendToList(&CallExpression_p->Children, PreviousCallExpression_p);
            nh_core_appendToList(&CallExpression_p->Children, Arguments.Node_p);
            return nh_ecmascript_parseCallExpressionRecursively(Parser_p, CallExpression_p);
        }

        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == '.') {
            nh_ecmascript_Parser NextParser = nh_ecmascript_advanceParser(*Parser_p, 1);
            if (NextParser.unparsed > 0 && NextParser.Tokens_p[0].type == NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME)
            {
                nh_ecmascript_ParseNode *CallExpression_p = nh_ecmascript_allocateNonTerminalParseNode("CallExpression", 1);
                nh_ecmascript_ParseNode *FullStop_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
                nh_ecmascript_ParseNode *IdentifierName_p = nh_ecmascript_allocateTerminalParseNode(&NextParser.Tokens_p[0]);

                nh_core_appendToList(&CallExpression_p->Children, PreviousCallExpression_p);
                nh_core_appendToList(&CallExpression_p->Children, FullStop_p);
                nh_core_appendToList(&CallExpression_p->Children, IdentifierName_p);

                *Parser_p = nh_ecmascript_advanceParser(NextParser, 1);
                return nh_ecmascript_parseCallExpressionRecursively(Parser_p, CallExpression_p);
            }  
        }
    }
    else {

        nh_ecmascript_ParseResult CoverCallExpressionAndAsyncArrowHead = nh_ecmascript_parseCoverCallExpressionAndAsyncArrowHead(Parser_p);
    
        if (CoverCallExpressionAndAsyncArrowHead.Node_p != NULL && CoverCallExpressionAndAsyncArrowHead.SyntaxErrors.size == 0) {
            nh_ecmascript_ParseNode *CallExpression_p = nh_ecmascript_allocateNonTerminalParseNode("CallExpression", 1);
            nh_core_appendToList(&CallExpression_p->Children, CoverCallExpressionAndAsyncArrowHead.Node_p);
            return nh_ecmascript_parseCallExpressionRecursively(Parser_p, CallExpression_p);
        }
    }

    return nh_ecmascript_initParseResult(PreviousCallExpression_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseCallExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult CallExpression = nh_ecmascript_parseCallExpressionRecursively(&LocalParser, NULL);
    
    if (CallExpression.Node_p != NULL && CallExpression.SyntaxErrors.size == 0) {
        *Parser_p = LocalParser;
    }

    return CallExpression;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseLeftHandSideExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Call = nh_ecmascript_parseCallExpression(Parser_p);

    if (Call.Node_p != NULL && Call.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *LeftHandSideExpression_p = nh_ecmascript_allocateNonTerminalParseNode("LeftHandSideExpression", 1);
        nh_core_appendToList(&LeftHandSideExpression_p->Children, Call.Node_p);
        return nh_ecmascript_initParseResult(LeftHandSideExpression_p);
    }

    nh_ecmascript_ParseResult New = nh_ecmascript_parseNewExpression(Parser_p);

    if (New.Node_p != NULL && New.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *LeftHandSideExpression_p = nh_ecmascript_allocateNonTerminalParseNode("LeftHandSideExpression", 1);
        nh_core_appendToList(&LeftHandSideExpression_p->Children, New.Node_p);
        return nh_ecmascript_initParseResult(LeftHandSideExpression_p);
    }

    nh_ecmascript_ParseResult Optional = nh_ecmascript_parseOptionalExpression(Parser_p);

    if (Optional.Node_p != NULL && Optional.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *LeftHandSideExpression_p = nh_ecmascript_allocateNonTerminalParseNode("LeftHandSideExpression", 1);
        nh_core_appendToList(&LeftHandSideExpression_p->Children, Optional.Node_p);
        return nh_ecmascript_initParseResult(LeftHandSideExpression_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseUpdateExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult LeftHandSide = nh_ecmascript_parseLeftHandSideExpression(&LocalParser);

    if (LeftHandSide.Node_p != NULL && LeftHandSide.SyntaxErrors.size == 0)
    {
        // Try to match postfix ++ or --
        bool hasOp = false;
        nh_ecmascript_InputElement *OpTok1_p = NULL;
        nh_ecmascript_InputElement *OpTok2_p = NULL;

        if (LocalParser.unparsed > 0)
        {
            const char *tok = LocalParser.Tokens_p[0].String.p;
            if (!strcmp(tok, "++") || !strcmp(tok, "--")) {
                hasOp = true;
                OpTok1_p = &LocalParser.Tokens_p[0];
                LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);
            }
            else if (LocalParser.unparsed > 1) {
                char c0 = LocalParser.Tokens_p[0].String.p[0];
                char c1 = LocalParser.Tokens_p[1].String.p[0];
                if ((c0 == '+' && c1 == '+') || (c0 == '-' && c1 == '-')) {
                    hasOp = true;
                    OpTok1_p = &LocalParser.Tokens_p[0];
                    OpTok2_p = &LocalParser.Tokens_p[1];
                    LocalParser = nh_ecmascript_advanceParser(LocalParser, 2);
                }
            }
        }

        nh_ecmascript_ParseNode *UpdateExpression_p = nh_ecmascript_allocateNonTerminalParseNode("UpdateExpression", hasOp ? 2 : 1);
        nh_core_appendToList(&UpdateExpression_p->Children, LeftHandSide.Node_p);
        if (hasOp) {
            // Represent the operator using one or two terminal tokens
            nh_ecmascript_ParseNode *Op1_p = nh_ecmascript_allocateTerminalParseNode(OpTok1_p);
            nh_core_appendToList(&UpdateExpression_p->Children, Op1_p);
            if (OpTok2_p != NULL) {
                nh_ecmascript_ParseNode *Op2_p = nh_ecmascript_allocateTerminalParseNode(OpTok2_p);
                nh_core_appendToList(&UpdateExpression_p->Children, Op2_p);
            }
        }

        *Parser_p = LocalParser;
        return nh_ecmascript_initParseResult(UpdateExpression_p);
    }

    return LeftHandSide;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseUnaryExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Update = nh_ecmascript_parseUpdateExpression(Parser_p);

    if (Update.Node_p != NULL && Update.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *UnaryExpression_p = nh_ecmascript_allocateNonTerminalParseNode("UnaryExpression", 1);
        nh_core_appendToList(&UnaryExpression_p->Children, Update.Node_p);
        return nh_ecmascript_initParseResult(UnaryExpression_p);
    }

    return Update;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseExponentiationExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Unary = nh_ecmascript_parseUnaryExpression(Parser_p);

    if (Unary.Node_p != NULL && Unary.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *ExponentiationExpression_p = nh_ecmascript_allocateNonTerminalParseNode("ExponentiationExpression", 1);
        nh_core_appendToList(&ExponentiationExpression_p->Children, Unary.Node_p);
        return nh_ecmascript_initParseResult(ExponentiationExpression_p);
    }

    return Unary;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseMultiplicativeExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult Left = nh_ecmascript_parseExponentiationExpression(&LocalParser);
    if (Left.Node_p == NULL || Left.SyntaxErrors.size > 0) { return Left; }

    nh_ecmascript_ParseNode *Accum_p = Left.Node_p;
    for (;;) {
        if (LocalParser.unparsed <= 0) {break;}
        char c = LocalParser.Tokens_p[0].String.p[0];
        if (c != '*' && c != '/' && c != '%') {break;}

        nh_ecmascript_InputElement *OpTok_p = &LocalParser.Tokens_p[0];
        LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);

        nh_ecmascript_ParseResult Right = nh_ecmascript_parseExponentiationExpression(&LocalParser);
        if (Right.Node_p == NULL || Right.SyntaxErrors.size > 0) {
            nh_ecmascript_freeParseNode(Accum_p);
            return nh_ecmascript_initParseResult(NULL);
        }

        nh_ecmascript_ParseNode *New_p = nh_ecmascript_allocateNonTerminalParseNode("MultiplicativeExpression", 3);
        nh_ecmascript_ParseNode *Op_p = nh_ecmascript_allocateTerminalParseNode(OpTok_p);
        nh_core_appendToList(&New_p->Children, Accum_p);
        nh_core_appendToList(&New_p->Children, Op_p);
        nh_core_appendToList(&New_p->Children, Right.Node_p);
        Accum_p = New_p;
    }

    nh_ecmascript_ParseNode *MultiplicativeExpression_p = nh_ecmascript_allocateNonTerminalParseNode("MultiplicativeExpression", 1);
    nh_core_appendToList(&MultiplicativeExpression_p->Children, Accum_p);
    *Parser_p = LocalParser;
    return nh_ecmascript_initParseResult(MultiplicativeExpression_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseAdditiveExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult Left = nh_ecmascript_parseMultiplicativeExpression(&LocalParser);
    if (Left.Node_p == NULL || Left.SyntaxErrors.size > 0) { return Left; }

    nh_ecmascript_ParseNode *Accum_p = Left.Node_p;
    for (;;) {
        if (LocalParser.unparsed <= 0) {break;}
        char op = LocalParser.Tokens_p[0].String.p[0];
        if (op != '+' && op != '-') {break;}

        nh_ecmascript_InputElement *OpTok_p = &LocalParser.Tokens_p[0];
        LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);

        nh_ecmascript_ParseResult Right = nh_ecmascript_parseMultiplicativeExpression(&LocalParser);
        if (Right.Node_p == NULL || Right.SyntaxErrors.size > 0) {
            nh_ecmascript_freeParseNode(Accum_p);
            return nh_ecmascript_initParseResult(NULL);
        }

        nh_ecmascript_ParseNode *New_p = nh_ecmascript_allocateNonTerminalParseNode("AdditiveExpression", 3);
        nh_ecmascript_ParseNode *Op_p = nh_ecmascript_allocateTerminalParseNode(OpTok_p);
        nh_core_appendToList(&New_p->Children, Accum_p);
        nh_core_appendToList(&New_p->Children, Op_p);
        nh_core_appendToList(&New_p->Children, Right.Node_p);
        Accum_p = New_p;
    }

    nh_ecmascript_ParseNode *AdditiveExpression_p = nh_ecmascript_allocateNonTerminalParseNode("AdditiveExpression", 1);
    nh_core_appendToList(&AdditiveExpression_p->Children, Accum_p);
    *Parser_p = LocalParser;
    return nh_ecmascript_initParseResult(AdditiveExpression_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseShiftExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseAdditiveExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *ShiftExpression_p = nh_ecmascript_allocateNonTerminalParseNode("ShiftExpression", 1);
        nh_core_appendToList(&ShiftExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(ShiftExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseRelationalExpression(
    nh_ecmascript_Parser *Parser_p)
{
    // Parse left operand
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult Left = nh_ecmascript_parseShiftExpression(&LocalParser);
    if (Left.Node_p == NULL || Left.SyntaxErrors.size > 0) {
        return Left;
    }

    // Iteratively parse relational operators: <, >, <=, >=
    nh_ecmascript_ParseNode *Accum_p = Left.Node_p;
    for (;;) {
        if (LocalParser.unparsed <= 0) {break;}
        const char c0 = LocalParser.Tokens_p[0].String.p[0];
        const char c1 = LocalParser.unparsed > 1 ? LocalParser.Tokens_p[1].String.p[0] : '\0';

        bool isOp = false;
        // Recognize one or two-char operators
        if (c0 == '<' || c0 == '>') { isOp = true; }
        if ((c0 == '<' || c0 == '>') && c1 == '=') { isOp = true; }

        if (!isOp) { break; }

        // Capture operator token
        nh_ecmascript_InputElement *OpToken_p = &LocalParser.Tokens_p[0];
        // Advance over operator token (treat <=/>= as single token by lexer; if not, we still consume one token here)
        LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);

        // Parse right operand
        nh_ecmascript_ParseResult Right = nh_ecmascript_parseShiftExpression(&LocalParser);
        if (Right.Node_p == NULL || Right.SyntaxErrors.size > 0) {
            nh_ecmascript_freeParseNode(Accum_p);
            return nh_ecmascript_initParseResult(NULL);
        }

        nh_ecmascript_ParseNode *New_p = nh_ecmascript_allocateNonTerminalParseNode("RelationalExpression", 3);
        nh_ecmascript_ParseNode *Op_p = nh_ecmascript_allocateTerminalParseNode(OpToken_p);
        nh_core_appendToList(&New_p->Children, Accum_p);
        nh_core_appendToList(&New_p->Children, Op_p);
        nh_core_appendToList(&New_p->Children, Right.Node_p);
        Accum_p = New_p;
    }

    nh_ecmascript_ParseNode *RelationalExpression_p = nh_ecmascript_allocateNonTerminalParseNode("RelationalExpression", 1);
    nh_core_appendToList(&RelationalExpression_p->Children, Accum_p);
    *Parser_p = LocalParser;
    return nh_ecmascript_initParseResult(RelationalExpression_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseEqualityExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseRelationalExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *EqualityExpression_p = nh_ecmascript_allocateNonTerminalParseNode("EqualityExpression", 1);
        nh_core_appendToList(&EqualityExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(EqualityExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseBitwiseANDExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseEqualityExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *BitwiseANDExpression_p = nh_ecmascript_allocateNonTerminalParseNode("BitwiseANDExpression", 1);
        nh_core_appendToList(&BitwiseANDExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(BitwiseANDExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseBitwiseXORExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseBitwiseANDExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *BitwiseXORExpression_p = nh_ecmascript_allocateNonTerminalParseNode("BitwiseXORExpression", 1);
        nh_core_appendToList(&BitwiseXORExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(BitwiseXORExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseBitwiseORExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseBitwiseXORExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *BitwiseORExpression_p = nh_ecmascript_allocateNonTerminalParseNode("BitwiseORExpression", 1);
        nh_core_appendToList(&BitwiseORExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(BitwiseORExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseLogicalANDExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseBitwiseORExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *LogicalANDExpression_p = nh_ecmascript_allocateNonTerminalParseNode("LogicalANDExpression", 1);
        nh_core_appendToList(&LogicalANDExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(LogicalANDExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseLogicalORExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseLogicalANDExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *LogicalORExpression_p = nh_ecmascript_allocateNonTerminalParseNode("LogicalORExpression", 1);
        nh_core_appendToList(&LogicalORExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(LogicalORExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseShortCircuitExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseLogicalORExpression(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *ShortCircuitExpression_p = nh_ecmascript_allocateNonTerminalParseNode("ShortCircuitExpression", 1);
        nh_core_appendToList(&ShortCircuitExpression_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(ShortCircuitExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseConditionalExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult ShortCircuit = nh_ecmascript_parseShortCircuitExpression(Parser_p);

    if (ShortCircuit.Node_p != NULL && ShortCircuit.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *ConditionalExpression_p = nh_ecmascript_allocateNonTerminalParseNode("ConditionalExpression", 1);
        nh_core_appendToList(&ConditionalExpression_p->Children, ShortCircuit.Node_p);
        return nh_ecmascript_initParseResult(ConditionalExpression_p);
    }

    return ShortCircuit;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseAssignmentOperator(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0) {
        return nh_ecmascript_initParseResult(NULL);
    }

    const char *token = Parser_p->Tokens_p[0].String.p;
    
    // Check for compound assignment operators
    if (!strcmp(token, "+=") || !strcmp(token, "-=") || !strcmp(token, "*=") || 
        !strcmp(token, "/=") || !strcmp(token, "%=") || !strcmp(token, "**=") ||
        !strcmp(token, "<<=") || !strcmp(token, ">>=") || !strcmp(token, ">>>=") ||
        !strcmp(token, "&=") || !strcmp(token, "^=") || !strcmp(token, "|=") ||
        !strcmp(token, "&&=") || !strcmp(token, "||=") || !strcmp(token, "??=")) {

        nh_ecmascript_ParseNode *AssignmentOperator_p = nh_ecmascript_allocateNonTerminalParseNode("AssignmentOperator", 1);
        nh_ecmascript_ParseNode *Operator_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        
        nh_core_appendToList(&AssignmentOperator_p->Children, Operator_p);
        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(AssignmentOperator_p);
    }
    
    // Check for simple assignment operator
    if (!strcmp(token, "=")) {
        nh_ecmascript_ParseNode *AssignmentOperator_p = nh_ecmascript_allocateNonTerminalParseNode("AssignmentOperator", 1);
        nh_ecmascript_ParseNode *Operator_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        
        nh_core_appendToList(&AssignmentOperator_p->Children, Operator_p);
        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(AssignmentOperator_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseAssignmentExpression(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseLeftHandSideExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        // Try to parse assignment operator
        nh_ecmascript_ParseResult OperatorResult = nh_ecmascript_parseAssignmentOperator(&LocalParser);
        if (OperatorResult.Node_p != NULL && OperatorResult.SyntaxErrors.size == 0) 
        {
            nh_ecmascript_ParseResult AssignmentResult = nh_ecmascript_parseAssignmentExpression(&LocalParser);

            if (AssignmentResult.Node_p != NULL && AssignmentResult.SyntaxErrors.size == 0) 
            {
                nh_ecmascript_ParseNode *AssignmentExpression_p = nh_ecmascript_allocateNonTerminalParseNode("AssignmentExpression", 3);

                nh_core_appendToList(&AssignmentExpression_p->Children, Result.Node_p);
                nh_core_appendToList(&AssignmentExpression_p->Children, OperatorResult.Node_p);
                nh_core_appendToList(&AssignmentExpression_p->Children, AssignmentResult.Node_p);

                *Parser_p = LocalParser;
                return nh_ecmascript_initParseResult(AssignmentExpression_p);
            }
            nh_ecmascript_freeParseNode(OperatorResult.Node_p);
        }
        nh_ecmascript_freeParseNode(Result.Node_p);
    }

    LocalParser = *Parser_p;
    Result = nh_ecmascript_parseConditionalExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
        nh_ecmascript_ParseNode *AssignmentExpression_p = nh_ecmascript_allocateNonTerminalParseNode("AssignmentExpression", 1);
        nh_core_appendToList(&AssignmentExpression_p->Children, Result.Node_p);
        *Parser_p = LocalParser;
        return nh_ecmascript_initParseResult(AssignmentExpression_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseInitializer(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || strcmp(Parser_p->Tokens_p[0].String.p, "=")) {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_Parser LocalParser = nh_ecmascript_advanceParser(*Parser_p, 1);
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseAssignmentExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        nh_ecmascript_ParseNode *Initializer_p = nh_ecmascript_allocateNonTerminalParseNode("Initializer", 2);
        nh_ecmascript_ParseNode *Equal_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&Initializer_p->Children, Equal_p);
        nh_core_appendToList(&Initializer_p->Children, Result.Node_p);

        *Parser_p = LocalParser;
        return nh_ecmascript_initParseResult(Initializer_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseIdentifier(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].type != NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME) {
        return nh_ecmascript_initParseResult(NULL);
    }

    unsigned int *index_p = nh_core_getFromHashMap(&NH_ECMASCRIPT_INDEXMAP.ReservedWords, Parser_p->Tokens_p[0].String.p);

    if (index_p == NULL) 
    {
        nh_ecmascript_ParseNode *Identifier_p = nh_ecmascript_allocateNonTerminalParseNode("Identifier", 1);
        nh_ecmascript_ParseNode *IdentifierName_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&Identifier_p->Children, IdentifierName_p);
        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(Identifier_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseBindingIdentifier(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_initParseResult(NULL);

    if (Parser_p->unparsed <= 0) {return Result;}

    if (!strcmp("yield", Parser_p->Tokens_p[0].String.p) || !strcmp("await", Parser_p->Tokens_p[0].String.p)) 
    {
        nh_ecmascript_ParseNode *BindingIdentifier_p = nh_ecmascript_allocateNonTerminalParseNode("BindingIdentifier", 1);
        nh_ecmascript_ParseNode *YieldOrAwait_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

        nh_core_appendToList(&BindingIdentifier_p->Children, YieldOrAwait_p);
        *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);
        return nh_ecmascript_initParseResult(BindingIdentifier_p);
    }
    else {

        Result = nh_ecmascript_parseIdentifier(Parser_p);        

        if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) {
            nh_ecmascript_ParseNode *BindingIdentifier_p = nh_ecmascript_allocateNonTerminalParseNode("BindingIdentifier", 1);
            nh_core_appendToList(&BindingIdentifier_p->Children, Result.Node_p);
            return nh_ecmascript_initParseResult(BindingIdentifier_p);
        }
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseVariableDeclaration(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseBindingIdentifier(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        nh_ecmascript_ParseNode *VariableDeclaration_p = nh_ecmascript_allocateNonTerminalParseNode("VariableDeclaration", 1);
        nh_ecmascript_ParseResult InitializerResult = nh_ecmascript_parseInitializer(Parser_p);

        if (InitializerResult.Node_p != NULL && InitializerResult.SyntaxErrors.size == 0) {
            VariableDeclaration_p->Children = nh_core_initList(2);
            nh_core_appendToList(&VariableDeclaration_p->Children, Result.Node_p);
            nh_core_appendToList(&VariableDeclaration_p->Children, InitializerResult.Node_p);
        }
        else {
            VariableDeclaration_p->Children = nh_core_initList(1);
            nh_core_appendToList(&VariableDeclaration_p->Children, Result.Node_p);
        }

        Result = nh_ecmascript_initParseResult(VariableDeclaration_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseVariableDeclarationList(
    nh_ecmascript_Parser *Parser_p)
{
    return nh_ecmascript_parseEasyRecursiveProduction(Parser_p, "VariableDeclarationList", true, nh_ecmascript_parseVariableDeclaration);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseVariableStatement(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || strcmp(Parser_p->Tokens_p[0].String.p, "var")) {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_Parser LocalParser = nh_ecmascript_advanceParser(*Parser_p, 1);
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseVariableDeclarationList(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        if (LocalParser.unparsed <= 0 || LocalParser.Tokens_p[0].String.p[0] != ';') {
            nh_ecmascript_freeParseNode(Result.Node_p);
            return nh_ecmascript_initParseResult(NULL);
        }

        nh_ecmascript_ParseNode *VariableStatement_p = nh_ecmascript_allocateNonTerminalParseNode("VariableStatement", 3);
        nh_ecmascript_ParseNode *Var_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_ecmascript_ParseNode *Semicolon_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        nh_core_appendToList(&VariableStatement_p->Children, Var_p);
        nh_core_appendToList(&VariableStatement_p->Children, Result.Node_p);
        nh_core_appendToList(&VariableStatement_p->Children, Semicolon_p);

        *Parser_p = nh_ecmascript_advanceParser(LocalParser, 1);
        return nh_ecmascript_initParseResult(VariableStatement_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseEmptyStatement(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.p[0] != ';') {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_ParseNode *EmptyStatement_p = nh_ecmascript_allocateNonTerminalParseNode("EmptyStatement", 1);
    nh_ecmascript_ParseNode *Semicolon_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);

    nh_core_appendToList(&EmptyStatement_p->Children, Semicolon_p);
    *Parser_p = nh_ecmascript_advanceParser(*Parser_p, 1);

    return nh_ecmascript_initParseResult(EmptyStatement_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseExpression(
    nh_ecmascript_Parser *Parser_p)
{
    return nh_ecmascript_parseEasyRecursiveProduction(Parser_p, "Expression", true, nh_ecmascript_parseAssignmentExpression);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseExpressionStatement(
    nh_ecmascript_Parser *Parser_p)
{
// TODO [lookahead ∉ { {, function, async [no LineTerminator here] function, class, let [ }] Expression[+In, ?Yield, ?Await] ; 

    if (Parser_p->unparsed <= 0 || Parser_p->Tokens_p[0].String.p[0] == '{' || !strcmp("function", Parser_p->Tokens_p[0].String.p) || !strcmp("class", Parser_p->Tokens_p[0].String.p)) {
        return nh_ecmascript_initParseResult(NULL);
    }
    else if (Parser_p->unparsed >= 2) {
        if (!strcmp("let", Parser_p->Tokens_p[0].String.p) && Parser_p->Tokens_p[1].String.p[0] == '[') {
            return nh_ecmascript_initParseResult(NULL);
        }
    }

    nh_ecmascript_Parser LocalParser = *Parser_p;
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseExpression(&LocalParser);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size == 0) 
    {
        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] != ';') {
            nh_ecmascript_freeParseNode(Result.Node_p);
            return nh_ecmascript_initParseResult(NULL);
        }

        nh_ecmascript_ParseNode *ExpressionStatement_p = nh_ecmascript_allocateNonTerminalParseNode("ExpressionStatement", 2);
        nh_ecmascript_ParseNode *Semicolon_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);

        nh_core_appendToList(&ExpressionStatement_p->Children, Result.Node_p);
        nh_core_appendToList(&ExpressionStatement_p->Children, Semicolon_p);

        *Parser_p = nh_ecmascript_advanceParser(LocalParser, 1);
        return nh_ecmascript_initParseResult(ExpressionStatement_p);
    }

    return nh_ecmascript_initParseResult(NULL);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseForStatement(
    nh_ecmascript_Parser *Parser_p)
{
    if (Parser_p->unparsed <= 0 || strcmp(Parser_p->Tokens_p[0].String.p, "for")) {
        return nh_ecmascript_initParseResult(NULL);
    }

    nh_ecmascript_Parser LocalParser = nh_ecmascript_advanceParser(*Parser_p, 1);

    // Check for opening parenthesis
    if (LocalParser.unparsed <= 0 || LocalParser.Tokens_p[0].String.p[0] != '(') {
        return nh_ecmascript_initParseResult(NULL);
    }
    LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);
    // Parse init: either 'var' VariableDeclarationList, Expression, or empty
    nh_ecmascript_ParseResult InitResult = nh_ecmascript_initParseResult(NULL);
    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] != ';')
    {
        if (!strcmp(LocalParser.Tokens_p[0].String.p, "var")) {
            nh_ecmascript_Parser AfterVar = nh_ecmascript_advanceParser(LocalParser, 1);
            nh_ecmascript_ParseResult DeclList = nh_ecmascript_parseVariableDeclarationList(&AfterVar);
            if (DeclList.Node_p == NULL || DeclList.SyntaxErrors.size > 0) {
                return nh_ecmascript_initParseResult(NULL);
            }
            // Build ForDeclaration := 'var' VariableDeclarationList
            nh_ecmascript_ParseNode *ForDeclaration_p = nh_ecmascript_allocateNonTerminalParseNode("ForDeclaration", 2);
            nh_ecmascript_ParseNode *VarToken_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[0]);
            nh_core_appendToList(&ForDeclaration_p->Children, VarToken_p);
            nh_core_appendToList(&ForDeclaration_p->Children, DeclList.Node_p);
            InitResult = nh_ecmascript_initParseResult(ForDeclaration_p);
            LocalParser = AfterVar;
        }
        else {
            InitResult = nh_ecmascript_parseExpression(&LocalParser);
            if (InitResult.Node_p == NULL || InitResult.SyntaxErrors.size > 0) {
                return nh_ecmascript_initParseResult(NULL);
            }
        }
    }
    // First ';'
    if (LocalParser.unparsed <= 0 || LocalParser.Tokens_p[0].String.p[0] != ';') {
        if (InitResult.Node_p != NULL) { nh_ecmascript_freeParseNode(InitResult.Node_p); }
        return nh_ecmascript_initParseResult(NULL);
    }
    LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);

    // Parse test expression (optional)
    nh_ecmascript_ParseResult ConditionResult = nh_ecmascript_initParseResult(NULL);
    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] != ';')
    {
        ConditionResult = nh_ecmascript_parseExpression(&LocalParser);
        if (ConditionResult.Node_p == NULL || ConditionResult.SyntaxErrors.size > 0) {
            if (InitResult.Node_p != NULL) { nh_ecmascript_freeParseNode(InitResult.Node_p); }
            return nh_ecmascript_initParseResult(NULL);
        }
    }

    // Second ';'
    if (LocalParser.unparsed <= 0 || LocalParser.Tokens_p[0].String.p[0] != ';') {
        if (InitResult.Node_p != NULL) { nh_ecmascript_freeParseNode(InitResult.Node_p); }
        if (ConditionResult.Node_p != NULL) { nh_ecmascript_freeParseNode(ConditionResult.Node_p); }
        return nh_ecmascript_initParseResult(NULL);
    }
    LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);

    // Parse update expression (optional)
    nh_ecmascript_ParseResult IncrementResult = nh_ecmascript_initParseResult(NULL);
    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] != ')')
    {
        IncrementResult = nh_ecmascript_parseExpression(&LocalParser);
        if (IncrementResult.Node_p == NULL || IncrementResult.SyntaxErrors.size > 0) {
            if (InitResult.Node_p != NULL) { nh_ecmascript_freeParseNode(InitResult.Node_p); }
            if (ConditionResult.Node_p != NULL) { nh_ecmascript_freeParseNode(ConditionResult.Node_p); }
            return nh_ecmascript_initParseResult(NULL);
        }
    }

    // ')'
    if (LocalParser.unparsed <= 0 || LocalParser.Tokens_p[0].String.p[0] != ')') {
        if (InitResult.Node_p != NULL) { nh_ecmascript_freeParseNode(InitResult.Node_p); }
        if (ConditionResult.Node_p != NULL) { nh_ecmascript_freeParseNode(ConditionResult.Node_p); }
        if (IncrementResult.Node_p != NULL) { nh_ecmascript_freeParseNode(IncrementResult.Node_p); }
        return nh_ecmascript_initParseResult(NULL);
    }
    LocalParser = nh_ecmascript_advanceParser(LocalParser, 1);

    // Body
    nh_ecmascript_ParseResult BodyResult = nh_ecmascript_parseStatement(&LocalParser);
    if (BodyResult.Node_p == NULL || BodyResult.SyntaxErrors.size > 0) {
        if (InitResult.Node_p != NULL) { nh_ecmascript_freeParseNode(InitResult.Node_p); }
        if (ConditionResult.Node_p != NULL) { nh_ecmascript_freeParseNode(ConditionResult.Node_p); }
        if (IncrementResult.Node_p != NULL) { nh_ecmascript_freeParseNode(IncrementResult.Node_p); }
        return nh_ecmascript_initParseResult(NULL);
    }

    // Build AST (keep same child structure as before: for, (, init?, test?, update?, ), body)
    nh_ecmascript_ParseNode *ForStatement_p = nh_ecmascript_allocateNonTerminalParseNode("ForStatement", 7);
    nh_ecmascript_ParseNode *ForKeyword_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
    nh_ecmascript_ParseNode *OpenParen_p = nh_ecmascript_allocateTerminalParseNode(&Parser_p->Tokens_p[1]);
    nh_ecmascript_ParseNode *CloseParen_p = nh_ecmascript_allocateTerminalParseNode(&LocalParser.Tokens_p[-1]);

    nh_core_appendToList(&ForStatement_p->Children, ForKeyword_p);
    nh_core_appendToList(&ForStatement_p->Children, OpenParen_p);
    if (InitResult.Node_p != NULL) { nh_core_appendToList(&ForStatement_p->Children, InitResult.Node_p); }
    if (ConditionResult.Node_p != NULL) { nh_core_appendToList(&ForStatement_p->Children, ConditionResult.Node_p); }
    if (IncrementResult.Node_p != NULL) { nh_core_appendToList(&ForStatement_p->Children, IncrementResult.Node_p); }
    nh_core_appendToList(&ForStatement_p->Children, CloseParen_p);
    nh_core_appendToList(&ForStatement_p->Children, BodyResult.Node_p);

    *Parser_p = LocalParser;
    return nh_ecmascript_initParseResult(ForStatement_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseIterationStatement(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseForStatement(Parser_p);

    if (Result.Node_p != NULL) 
    {
        nh_ecmascript_ParseNode *IterationStatement_p = nh_ecmascript_allocateNonTerminalParseNode("IterationStatement", 1);
        nh_core_appendToList(&IterationStatement_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(IterationStatement_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseBreakableStatement(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseIterationStatement(Parser_p);

    if (Result.Node_p != NULL) 
    {
        nh_ecmascript_ParseNode *BreakableStatement_p = nh_ecmascript_allocateNonTerminalParseNode("BreakableStatement", 1);
        nh_core_appendToList(&BreakableStatement_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(BreakableStatement_p);
    }

    // TODO switch statement

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseStatement(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseBlockStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

    Result = nh_ecmascript_parseVariableStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

    Result = nh_ecmascript_parseBreakableStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

    Result = nh_ecmascript_parseEmptyStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

    Result = nh_ecmascript_parseExpressionStatement(Parser_p);
    if (Result.Node_p != NULL) {goto PARSE_STATEMENT_SUCCESS;}

    return Result;

PARSE_STATEMENT_SUCCESS: ;

    nh_ecmascript_ParseNode *Statement_p = nh_ecmascript_allocateNonTerminalParseNode("Statement", 1);
    nh_core_appendToList(&Statement_p->Children, Result.Node_p);

    return nh_ecmascript_initParseResult(Statement_p);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseStatementListItem(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseStatement(Parser_p);

    if (Result.Node_p != NULL) 
    {
        nh_ecmascript_ParseNode *StatementListItem_p = nh_ecmascript_allocateNonTerminalParseNode("StatementListItem", 1);
        nh_core_appendToList(&StatementListItem_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(StatementListItem_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseStatementList(
    nh_ecmascript_Parser *Parser_p)
{
    return nh_ecmascript_parseEasyRecursiveProduction(Parser_p, "StatementList", false, nh_ecmascript_parseStatementListItem);
}

static nh_ecmascript_ParseResult nh_ecmascript_parseScriptBody(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseStatementList(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size <= 0) 
    {
        nh_ecmascript_ParseNode *ScriptBody_p = nh_ecmascript_allocateNonTerminalParseNode("ScriptBody", 1);
        nh_core_appendToList(&ScriptBody_p->Children, Result.Node_p);
        return nh_ecmascript_initParseResult(ScriptBody_p);
    }

    return Result;
}

static nh_ecmascript_ParseResult nh_ecmascript_parseScriptNode(
    nh_ecmascript_Parser *Parser_p)
{
    nh_ecmascript_ParseResult Result = nh_ecmascript_parseScriptBody(Parser_p);

    if (Result.Node_p != NULL && Result.SyntaxErrors.size <= 0) 
    {
        nh_ecmascript_ParseNode *Script_p = nh_ecmascript_allocateNonTerminalParseNode("Script", 1);
        nh_core_appendToList(&Script_p->Children, Result.Node_p);
        Result = nh_ecmascript_initParseResult(Script_p);
    }

    return Result;
}

// https://tc39.es/ecma262/#sec-parsetext
nh_ecmascript_ParseResult nh_ecmascript_parseText(
    nh_core_Array InputElements, NH_ECMASCRIPT_PARSE_NODE goalSymbol)
{
    nh_ecmascript_Parser Parser;
    Parser.Tokens_p = (nh_ecmascript_InputElement*)InputElements.p;
    Parser.unparsed = InputElements.length;

    nh_ecmascript_ParseResult Result = nh_ecmascript_initParseResult(NULL);

    switch (goalSymbol)
    {
        case NH_ECMASCRIPT_PARSE_NODE_SCRIPT :
            Result = nh_ecmascript_parseScriptNode(&Parser);
            break;
    }

    return Result;
}
