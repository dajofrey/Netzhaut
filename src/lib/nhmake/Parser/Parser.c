// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Parser.h"

#include "../Common/Macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// PARSE ===========================================================================================

static nh_make_Token *nh_make_parseToken(
    nh_make_Token *Token_p, nh_make_Definition *Definition_p
); 

static nh_make_Token *nh_make_parseFunction(
    nh_make_Token *Token_p, nh_make_Definition *Definition_p)
{
NH_MAKE_BEGIN()

    Definition_p->type = NH_MAKE_DEFINITION_FUNCTION;
    Definition_p->Function.name_p = Token_p->string_p;
    Definition_p->Function.arguments_pp = NULL; 
    Definition_p->Function.arguments = 0;

    ++Token_p;

    if (Token_p->type != NH_MAKE_TOKEN_ROUND_BRACKET_LEFT) {
        NH_MAKE_END(Token_p)
    }

    Definition_p->Function.arguments_pp = malloc(sizeof(NH_BYTE*));
    NH_MAKE_CHECK_NULL_2(NULL, Definition_p->Function.arguments_pp)

    Definition_p->Function.argumentTypes_p = malloc(sizeof(NH_MAKE_TOKEN));
    NH_MAKE_CHECK_NULL_2(NULL, Definition_p->Function.argumentTypes_p)

    while (Token_p->type != NH_MAKE_TOKEN_ROUND_BRACKET_RIGHT)
    {
        switch (Token_p->type)
        {
            case NH_MAKE_TOKEN_IDENTIFIER :
            case NH_MAKE_TOKEN_STRING :
                Definition_p->Function.argumentTypes_p[Definition_p->Function.arguments] = Token_p->type;
                Definition_p->Function.arguments_pp[Definition_p->Function.arguments++] = Token_p->string_p;
                Definition_p->Function.arguments_pp = realloc(Definition_p->Function.arguments_pp, sizeof(NH_BYTE*) * (Definition_p->Function.arguments + 1));
                Definition_p->Function.argumentTypes_p = realloc(Definition_p->Function.argumentTypes_p, sizeof(NH_MAKE_TOKEN) * (Definition_p->Function.arguments + 1));
                break;
        }
        ++Token_p;
    }

NH_MAKE_END(++Token_p)
}

static nh_make_Token *nh_make_parseBlock(
    nh_make_Token *Token_p, nh_make_Definition *Definition_p)
{
NH_MAKE_BEGIN()

    Definition_p->type = NH_MAKE_DEFINITION_BLOCK;
    Definition_p->Block.Definitions_p = NULL;
    Definition_p->Block.definitions   = 0;

    if (Token_p->type == NH_MAKE_TOKEN_CURLY_BRACKET_RIGHT) {
        NH_MAKE_END(Token_p)
    }

    Definition_p->Block.Definitions_p = malloc(sizeof(nh_make_Definition));
    NH_MAKE_CHECK_NULL_2(NULL, Definition_p->Block.Definitions_p)

    while (Token_p->type != NH_MAKE_TOKEN_CURLY_BRACKET_RIGHT) {        
        Token_p = nh_make_parseToken(Token_p, &Definition_p->Block.Definitions_p[Definition_p->Block.definitions++]);
        Definition_p->Block.Definitions_p = realloc(Definition_p->Block.Definitions_p, sizeof(nh_make_Definition) * (Definition_p->Block.definitions + 1));
    }

NH_MAKE_END(Token_p)
}

static nh_make_Token *nh_make_parseOption(
    nh_make_Token *Token_p, nh_make_Definition *Definition_p)
{
NH_MAKE_BEGIN()

    Definition_p->type = NH_MAKE_DEFINITION_OPTION;

    nh_make_Token *Description_p = Token_p->type == NH_MAKE_TOKEN_STRING ? Token_p : NULL;
    NH_MAKE_BOOL longOption = NH_MAKE_FALSE;

    if (Token_p->type == NH_MAKE_TOKEN_STRING) {
        if (Token_p[2].type == NH_MAKE_TOKEN_HYPHEN_MINUS) {
            longOption = NH_MAKE_TRUE; 
            Token_p = Token_p + 3;
        }
        else {Token_p = Token_p + 2;}
    }
    else {
        if (Token_p[1].type == NH_MAKE_TOKEN_HYPHEN_MINUS) {
            longOption = NH_MAKE_TRUE;
            Token_p = Token_p + 2;
        }
        else {Token_p = Token_p + 1;}
    }

    Definition_p->Option.arguments = 0;
    Definition_p->Option.arguments_pp = NULL;
    Definition_p->Option.longOption = longOption;
    Definition_p->Option.name_p = Token_p->string_p;
    Definition_p->Option.description_p = Description_p ? Description_p->string_p : NULL;
    Definition_p->Option.Block_p = malloc(sizeof(nh_make_Block));
    NH_MAKE_CHECK_NULL_2(NULL, Definition_p->Option.Block_p)

    ++Token_p;

    nh_make_Token *Argument_p = Token_p;
    int arguments = 0;
    while (Argument_p->type == NH_MAKE_TOKEN_IDENTIFIER || Argument_p->type == NH_MAKE_TOKEN_STRING) {
        arguments++;
        Argument_p++;
    }

    if (arguments) {
        Definition_p->Option.arguments = arguments;
        Definition_p->Option.arguments_pp = malloc(sizeof(NH_BYTE*) * arguments);
        NH_MAKE_CHECK_NULL_2(NULL, Definition_p->Option.arguments_pp)
        for (int i = 0; i < arguments; ++i) {
            Definition_p->Option.arguments_pp[i] = Token_p->string_p;
            Token_p++;
        }
    }

    if (Token_p->type != NH_MAKE_TOKEN_CURLY_BRACKET_LEFT) {
        NH_MAKE_END(Token_p)
    }

NH_MAKE_END(nh_make_parseBlock(++Token_p, Definition_p->Option.Block_p))
}

static nh_make_Token *nh_make_parseIf(
    nh_make_Token *Token_p, nh_make_Definition *Definition_p)
{
NH_MAKE_BEGIN()

    Definition_p->type = NH_MAKE_DEFINITION_IF;
    Definition_p->If.string_p = Token_p->string_p;
    Definition_p->If.Block_p = malloc(sizeof(nh_make_Definition)); 
    NH_MAKE_CHECK_NULL_2(NULL, Definition_p->If.Block_p)

    ++Token_p;

    if (Token_p->type != NH_MAKE_TOKEN_CURLY_BRACKET_LEFT) {
        NH_MAKE_END(Token_p)
    }

NH_MAKE_END(nh_make_parseBlock(++Token_p, Definition_p->If.Block_p))
}

static nh_make_Token *nh_make_parseToken(
    nh_make_Token *Token_p, nh_make_Definition *Definition_p) 
{
NH_MAKE_BEGIN()

    Definition_p->type = NH_MAKE_DEFINITION_UNDEFINED;

    switch (Token_p->type)
    {
        case NH_MAKE_TOKEN_CURLY_BRACKET_RIGHT :
        case NH_MAKE_TOKEN_CURLY_BRACKET_LEFT  :
        case NH_MAKE_TOKEN_ROUND_BRACKET_RIGHT :
        case NH_MAKE_TOKEN_ROUND_BRACKET_LEFT  :
        case NH_MAKE_TOKEN_ANGLE_BRACKET_RIGHT :
        case NH_MAKE_TOKEN_ANGLE_BRACKET_LEFT  :
        case NH_MAKE_TOKEN_COMMA               :
            break;

        case NH_MAKE_TOKEN_STRING :
            if (Token_p[1].type == NH_MAKE_TOKEN_HYPHEN_MINUS) {
                NH_MAKE_END(nh_make_parseOption(Token_p, Definition_p))
            }
            break;

        case NH_MAKE_TOKEN_IDENTIFIER :
            switch((Token_p+1)->type) {
                case NH_MAKE_TOKEN_CURLY_BRACKET_LEFT :
                    NH_MAKE_END(nh_make_parseIf(Token_p, Definition_p))
                case NH_MAKE_TOKEN_ROUND_BRACKET_LEFT :
                    NH_MAKE_END(nh_make_parseFunction(Token_p, Definition_p))
            }
            break;

        case NH_MAKE_TOKEN_HYPHEN_MINUS :
            NH_MAKE_END(nh_make_parseOption(Token_p, Definition_p))
            break;
    }

NH_MAKE_END(++Token_p)
}

static NH_MAKE_RESULT nh_make_parseFile(
    nh_make_Parser *Parser_p, nh_make_File *File_p) 
{
NH_MAKE_BEGIN()

    nh_make_Tokenizer Tokenizer = nh_make_initTokenizer();
    NH_MAKE_CHECK(nh_make_tokenizeFile(&Tokenizer, File_p))

    nh_make_Token *Token_p = Tokenizer.Tokens_p;

    Parser_p->definitions = 0;
    Parser_p->Definitions_p = malloc(sizeof(nh_make_Definition));
    NH_MAKE_CHECK_NULL(Parser_p->Definitions_p)

    while (Token_p->type != NH_MAKE_TOKEN_EOF) {
        Token_p = nh_make_parseToken(Token_p, &Parser_p->Definitions_p[Parser_p->definitions]);
        if (Parser_p->Definitions_p[Parser_p->definitions].type != NH_MAKE_DEFINITION_UNDEFINED) {
            Parser_p->definitions++;
            Parser_p->Definitions_p = realloc(Parser_p->Definitions_p, sizeof(nh_make_Definition) * (Parser_p->definitions + 1));
        }
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// SHOW ============================================================================================

static void nh_make_showParserDefinition(
    nh_make_Definition *Definition_p, unsigned int depth)
{
NH_MAKE_BEGIN()

    NH_BYTE depth_p[255];
    memset(depth_p, 0, 255);

    for (int i = 0; i < (depth * 2); ++i) {depth_p[i] = ' ';}

    switch (Definition_p->type)
    {
        case NH_MAKE_DEFINITION_IF :
            nh_make_messagef("%s%s", depth_p, Definition_p->If.string_p);
            nh_make_messagef("%s{", depth_p);
            for (int j = 0; j < Definition_p->If.Block_p->Block.definitions; ++j) {
                nh_make_showParserDefinition(&Definition_p->If.Block_p->Block.Definitions_p[j], depth + 1);
            }
            nh_make_messagef("%s}", depth_p);
            break;

        case NH_MAKE_DEFINITION_FUNCTION :
            nh_make_messagef("%s%s", depth_p, Definition_p->Function.name_p);
            for (int j = 0; j < Definition_p->Function.arguments; ++j) {
                nh_make_messagef("%s  %s", depth_p, Definition_p->Function.arguments_pp[j]);
            }
            break;

        case NH_MAKE_DEFINITION_BLOCK :
            nh_make_messagef("%s{", depth_p);
            for (int j = 0; j < Definition_p->Block.definitions; ++j) {
                nh_make_showParserDefinition(&Definition_p->Block.Definitions_p[j], depth + 1);
            }
            nh_make_messagef("%s}", depth_p);
            break;

        case NH_MAKE_DEFINITION_OPTION :
            if (Definition_p->Option.longOption) {
                nh_make_messagef("%s--%s", depth_p, Definition_p->Option.name_p);
            }
            else {
                nh_make_messagef("%s-%s", depth_p, Definition_p->Option.name_p);
            }
            if (Definition_p->Option.description_p) {
                nh_make_messagef("%s  description: %s", depth_p, Definition_p->Option.description_p);
            }
            for (int i = 0; i < Definition_p->Option.arguments; ++i) {
                nh_make_messagef("%s  argument %d: %s", depth_p, i, Definition_p->Option.arguments_pp[i]);
            }
            nh_make_showParserDefinition(Definition_p->Option.Block_p, depth + 1);
            break;
    }

NH_MAKE_SILENT_END()
}

static void nh_make_showParseTree(
    nh_make_Parser *Parser_p)
{
NH_MAKE_BEGIN()

    nh_make_operationf("Show Parse-Tree");

    for (int i = 0; i < Parser_p->definitions; ++i) {
        nh_make_showParserDefinition(&Parser_p->Definitions_p[i], 0);
    }

NH_MAKE_SILENT_END()
}

// PARSER ARRAY ====================================================================================

void nh_make_initParserArray(
    nh_make_ParserArray *Array_p)
{
NH_MAKE_BEGIN()

    Array_p->length = 0;
    Array_p->Parsers_p = NULL;

NH_MAKE_SILENT_END()
}

static nh_make_Parser nh_make_initParser()
{
NH_MAKE_BEGIN()

    nh_make_Parser Parser;
    Parser.executed = NH_MAKE_FALSE;
    Parser.Definitions_p = NULL;
    Parser.definitions = 0;

NH_MAKE_END(Parser)
}

NH_MAKE_RESULT nh_make_appendParser(
    nh_make_ParserArray *Array_p, nh_make_File *File_p, NH_MAKE_BOOL showParseTree)
{
NH_MAKE_BEGIN()

    if (!Array_p->Parsers_p) {
        Array_p->Parsers_p = malloc(sizeof(nh_make_Parser));
        NH_MAKE_CHECK_NULL(Array_p->Parsers_p)
    }
    else {
        Array_p->Parsers_p = realloc(Array_p->Parsers_p, sizeof(nh_make_Parser) * (Array_p->length + 1));
        NH_MAKE_CHECK_NULL(Array_p->Parsers_p)
    }

    nh_make_Parser *Parser_p = &Array_p->Parsers_p[Array_p->length];

    *Parser_p = nh_make_initParser();
    NH_MAKE_CHECK(nh_make_parseFile(Parser_p, File_p))

    if (showParseTree) {
        nh_make_showParseTree(Parser_p);
    }

    Array_p->length++;

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

