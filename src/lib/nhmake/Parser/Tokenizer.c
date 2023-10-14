// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Tokenizer.h"

#include "../Core/Utils.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// HELPER ==========================================================================================

static NH_MAKE_BOOL nh_make_isASCIIUpperAlpha(
    NH_BYTE codepoint)
{
NH_MAKE_BEGIN()
NH_MAKE_END(codepoint >= 0x41 && codepoint <= 0x5A)
}

static NH_MAKE_BOOL nh_make_isASCIILowerAlpha(
    NH_BYTE codepoint)
{
NH_MAKE_BEGIN()
NH_MAKE_END(codepoint >= 0x61 && codepoint <= 0x7A)
}

static NH_MAKE_BOOL nh_make_isASCIIAlpha(
    NH_BYTE codepoint)
{
NH_MAKE_BEGIN()
NH_MAKE_END(nh_make_isASCIIUpperAlpha(codepoint) || nh_make_isASCIILowerAlpha(codepoint))
}

static NH_MAKE_BOOL nh_make_isBracket(
    NH_BYTE codepoint)
{
NH_MAKE_BEGIN()
NH_MAKE_END(codepoint == '(' || codepoint == ')' || codepoint == '{' || codepoint == '}' || codepoint == '[' || codepoint == ']')
}

static NH_MAKE_BOOL nh_make_isTokenBegin(
    NH_BYTE codepoint)
{
NH_MAKE_BEGIN()

    if (nh_make_isASCIIAlpha(codepoint) || nh_make_isBracket(codepoint) 
    || codepoint == ',' 
    || codepoint == '"' 
    || codepoint == ':' 
    || codepoint == '-') 

    {NH_MAKE_END(NH_MAKE_TRUE)}

NH_MAKE_END(NH_MAKE_FALSE)
}

// TOKENIZE ========================================================================================

static NH_BYTE *nh_make_tokenizeString(
    nh_make_Token *Token_p, NH_BYTE *bytes_p)
{
NH_MAKE_BEGIN()

    NH_BYTE *string_p = malloc(1);
    unsigned int stringLength = 0;

    NH_BYTE *stringBegin_p = bytes_p;

    NH_MAKE_BOOL escape = NH_MAKE_FALSE;
    while (*bytes_p && (*bytes_p != '"' || escape)) {
        escape = escape ? NH_MAKE_FALSE : *bytes_p == 0x5C;
        if (!escape && *bytes_p != '\n') {
            string_p = realloc(string_p, stringLength + 1);
            string_p[stringLength++] = *bytes_p;
        }
        bytes_p++;
    }
    if (!*bytes_p) {NH_MAKE_END(NULL)}

    *bytes_p = 0;

    string_p = realloc(string_p, stringLength + 1);
    string_p[stringLength] = 0;

    Token_p->string_p = string_p;

    *bytes_p = '"';

NH_MAKE_END(&bytes_p[1])
}

static NH_BYTE *nh_make_getToken(
    nh_make_Token *Token_p, NH_BYTE *bytes_p) 
{
NH_MAKE_BEGIN()

    if (!*bytes_p) {NH_MAKE_END(NULL)}

    Token_p->type     = NH_MAKE_TOKEN_UNDEFINED;
    Token_p->string_p = NULL;

    while (*bytes_p && !nh_make_isTokenBegin(*bytes_p)) {
        if (bytes_p[0] == '/' && bytes_p[1] == '/') {
            while (*bytes_p && *bytes_p != '\n') {bytes_p++;}
        }
        bytes_p++;
    }
    if (!*bytes_p) {NH_MAKE_END(NULL)}

    NH_BYTE *tokenBegin_p = bytes_p;

    switch (*tokenBegin_p) 
    {
        case '(' : Token_p->type = NH_MAKE_TOKEN_ROUND_BRACKET_LEFT; break;
        case ')' : Token_p->type = NH_MAKE_TOKEN_ROUND_BRACKET_RIGHT; break;
        case '{' : Token_p->type = NH_MAKE_TOKEN_CURLY_BRACKET_LEFT; break;
        case '}' : Token_p->type = NH_MAKE_TOKEN_CURLY_BRACKET_RIGHT; break;
        case '[' : Token_p->type = NH_MAKE_TOKEN_ANGLE_BRACKET_LEFT; break;
        case ']' : Token_p->type = NH_MAKE_TOKEN_ANGLE_BRACKET_RIGHT; break;
        case '-' : Token_p->type = NH_MAKE_TOKEN_HYPHEN_MINUS; break;
        case ',' : Token_p->type = NH_MAKE_TOKEN_COMMA; break;
        case ':' : Token_p->type = NH_MAKE_TOKEN_COLON; break;
        case '"' :
            Token_p->type = NH_MAKE_TOKEN_STRING;
            NH_MAKE_END(nh_make_tokenizeString(Token_p, &bytes_p[1]))
            break;
    }
    
    if (Token_p->type != NH_MAKE_TOKEN_UNDEFINED) {
        NH_MAKE_END(&bytes_p[1])
    }

    while (*bytes_p && (nh_make_isASCIIAlpha(*bytes_p) || *bytes_p == '_')) {bytes_p++;}
    if (!*bytes_p) {NH_MAKE_END(NULL)}

    NH_BYTE tmp = *bytes_p;
    *bytes_p = 0;    

    Token_p->type = NH_MAKE_TOKEN_IDENTIFIER;
    Token_p->string_p = malloc(strlen(tokenBegin_p) + 1);
    strcpy(Token_p->string_p, tokenBegin_p);

    *bytes_p = tmp;

NH_MAKE_END(bytes_p)
}

static void nh_make_getTokens(
    nh_make_Token *Tokens_p, NH_BYTE *bytes_p, unsigned int *tokens_p) 
{
NH_MAKE_BEGIN()

    unsigned int tokens = 0;

    while (bytes_p)
    {
        nh_make_Token Token;
        bytes_p = nh_make_getToken(&Token, bytes_p);
        if (Tokens_p) {Tokens_p[tokens] = Token;}
        tokens++;
    }

    if (Tokens_p) {
        Tokens_p[tokens - 1].type = NH_MAKE_TOKEN_EOF;        
    }

    if (tokens_p) {*tokens_p = tokens;}

NH_MAKE_SILENT_END()
}

NH_MAKE_RESULT nh_make_tokenizeFile(
    nh_make_Tokenizer *Tokenizer_p, nh_make_File *File_p) 
{
NH_MAKE_BEGIN()

    NH_BYTE *bytes_p = malloc(strlen(File_p->data_p) + 1);
    NH_MAKE_CHECK_NULL(bytes_p)
    strcpy(bytes_p, File_p->data_p);

    unsigned int tokens = 0;
    nh_make_getTokens(NULL, bytes_p, &tokens);

    free(bytes_p);
    bytes_p = NULL;

    bytes_p = malloc(strlen(File_p->data_p) + 1);
    NH_MAKE_CHECK_NULL(bytes_p)
    strcpy(bytes_p, File_p->data_p);

    nh_make_Token *Tokens_p = malloc(sizeof(nh_make_Token) * tokens);
    for (int i = 0; i < tokens; ++i) {
        Tokens_p[i].string_p = NULL;
    }
    nh_make_getTokens(Tokens_p, bytes_p, NULL);

    free(bytes_p);

    Tokenizer_p->tokens   = tokens;
    Tokenizer_p->Tokens_p = Tokens_p;

NH_MAKE_END(NH_MAKE_SUCCESS)
}

// INIT ============================================================================================

nh_make_Tokenizer nh_make_initTokenizer()
{
NH_MAKE_BEGIN()

    nh_make_Tokenizer Tokenizer;
    Tokenizer.tokens = 0;
    Tokenizer.Tokens_p = NULL;

NH_MAKE_END(Tokenizer)
}

