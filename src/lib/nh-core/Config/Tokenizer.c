// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Tokenizer.h"

#include "../System/Memory.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// HELPER ==========================================================================================

static NH_BOOL nh_core_isASCIIUpperAlpha(
    NH_BYTE codepoint)
{
NH_CORE_BEGIN()
NH_CORE_END(codepoint >= 0x41 && codepoint <= 0x5A)
}

static NH_BOOL nh_core_isASCIILowerAlpha(
    NH_BYTE codepoint)
{
NH_CORE_BEGIN()
NH_CORE_END(codepoint >= 0x61 && codepoint <= 0x7A)
}

static NH_BOOL nh_core_isASCIINumber(
    NH_BYTE codepoint)
{
NH_CORE_BEGIN()
NH_CORE_END(codepoint >= 0x30 && codepoint <= 0x39)
}

static NH_BOOL nh_core_isASCIIAlpha(
    NH_BYTE codepoint)
{
NH_CORE_BEGIN()
NH_CORE_END(nh_core_isASCIIUpperAlpha(codepoint) || nh_core_isASCIILowerAlpha(codepoint))
}

static NH_BOOL nh_core_isBracket(
    NH_BYTE codepoint)
{
NH_CORE_BEGIN()
NH_CORE_END(codepoint == '(' || codepoint == ')' || codepoint == '{' || codepoint == '}' || codepoint == '[' || codepoint == ']')
}

static NH_BOOL nh_core_isTokenBegin(
    NH_BYTE codepoint)
{
NH_CORE_BEGIN()

    if (nh_core_isASCIIAlpha(codepoint) || nh_core_isASCIINumber(codepoint) || nh_core_isBracket(codepoint) 
    || codepoint == ',' 
    || codepoint == '"' 
    || codepoint == ':' 
    || codepoint == ';') 

    {NH_CORE_END(NH_TRUE)}

NH_CORE_END(NH_FALSE)
}

// TOKENIZE ========================================================================================

static NH_BYTE *nh_core_tokenizeString(
    nh_ConfigToken *Token_p, NH_BYTE *bytes_p)
{
NH_CORE_BEGIN()

    NH_BYTE *string_p = malloc(1);
    unsigned int stringLength = 0;

    NH_BYTE *stringBegin_p = bytes_p;

    NH_BOOL escape = NH_FALSE;
    while (*bytes_p && (*bytes_p != '"' || escape)) {
        escape = escape ? NH_FALSE : *bytes_p == 0x5C;
        if (!escape && *bytes_p != '\n') {
            string_p = realloc(string_p, stringLength + 1);
            string_p[stringLength++] = *bytes_p;
        }
        bytes_p++;
    }
    if (!*bytes_p) {NH_CORE_END(NULL)}

    *bytes_p = 0;

    string_p = realloc(string_p, stringLength + 1);
    string_p[stringLength] = 0;

    Token_p->string_p = string_p;

    *bytes_p = '"';

NH_CORE_END(&bytes_p[1])
}

static NH_BYTE *nh_core_getConfigToken(
    nh_ConfigToken *Token_p, NH_BYTE *bytes_p) 
{
NH_CORE_BEGIN()

    if (!*bytes_p) {NH_CORE_END(NULL)}

    Token_p->type     = NH_CONFIG_TOKEN_UNDEFINED;
    Token_p->string_p = NULL;

    while (*bytes_p && !nh_core_isTokenBegin(*bytes_p)) {
        if (bytes_p[0] == '/' && bytes_p[1] == '/') {
            while (*bytes_p && *bytes_p != '\n') {bytes_p++;}
        }
        bytes_p++;
    }
    if (!*bytes_p) {NH_CORE_END(NULL)}

    NH_BYTE *tokenBegin_p = bytes_p;

    switch (*tokenBegin_p) 
    {
        case '(' : Token_p->type = NH_CONFIG_TOKEN_ROUND_BRACKET_LEFT; break;
        case ')' : Token_p->type = NH_CONFIG_TOKEN_ROUND_BRACKET_RIGHT; break;
        case '{' : Token_p->type = NH_CONFIG_TOKEN_CURLY_BRACKET_LEFT; break;
        case '}' : Token_p->type = NH_CONFIG_TOKEN_CURLY_BRACKET_RIGHT; break;
        case '[' : Token_p->type = NH_CONFIG_TOKEN_ANGLE_BRACKET_LEFT; break;
        case ']' : Token_p->type = NH_CONFIG_TOKEN_ANGLE_BRACKET_RIGHT; break;
        case ',' : Token_p->type = NH_CONFIG_TOKEN_COMMA; break;
        case ':' : Token_p->type = NH_CONFIG_TOKEN_COLON; break;
        case ';' : Token_p->type = NH_CONFIG_TOKEN_SEMICOLON; break;
        case '"' :
            Token_p->type = NH_CONFIG_TOKEN_STRING;
            NH_CORE_END(nh_core_tokenizeString(Token_p, &bytes_p[1]))
            break;
    }
    
    if (Token_p->type != NH_CONFIG_TOKEN_UNDEFINED) {
        NH_CORE_END(&bytes_p[1])
    }

    while (*bytes_p && (nh_core_isASCIIAlpha(*bytes_p) || nh_core_isASCIINumber(*bytes_p) 
        || *bytes_p == '_' || *bytes_p == '.' || *bytes_p == '-')) {bytes_p++;}
    if (!*bytes_p) {NH_CORE_END(NULL)}

    NH_BYTE tmp = *bytes_p;
    *bytes_p = 0;    

    Token_p->type = NH_CONFIG_TOKEN_IDENTIFIER;
    Token_p->string_p = malloc(strlen(tokenBegin_p) + 1);
    strcpy(Token_p->string_p, tokenBegin_p);

    *bytes_p = tmp;

NH_CORE_END(bytes_p)
}

static void nh_core_getConfigTokens(
    nh_ConfigToken *Tokens_p, NH_BYTE *bytes_p, unsigned int *tokens_p) 
{
NH_CORE_BEGIN()

    unsigned int tokens = 0;

    while (bytes_p)
    {
        nh_ConfigToken Token;
        bytes_p = nh_core_getConfigToken(&Token, bytes_p);
        if (Tokens_p) {
            Tokens_p[tokens] = Token;
        } else if (Token.string_p) {
            free(Token.string_p);
        }
        tokens++;
    }

    if (Tokens_p) {
        Tokens_p[tokens - 1].type = NH_CONFIG_TOKEN_EOF;        
    }

    if (tokens_p) {*tokens_p = tokens;}

NH_CORE_SILENT_END()
}

NH_CORE_RESULT nh_core_tokenizeConfig(
    nh_ConfigTokenizer *Tokenizer_p, NH_BYTE *data_p, int length) 
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL(data_p)
    NH_CORE_CHECK_NULL(Tokenizer_p)
    Tokenizer_p->tokens = 0;
    Tokenizer_p->Tokens_p = NULL;

    NH_BYTE *bytes_p = nh_core_allocate(length + 1);
    NH_CORE_CHECK_NULL(bytes_p)
    memset(bytes_p, 0, length + 1);
    memcpy(bytes_p, data_p, length);

    unsigned int tokens = 0;
    nh_core_getConfigTokens(NULL, bytes_p, &tokens);

    nh_ConfigToken *Tokens_p = malloc(sizeof(nh_ConfigToken) * tokens);
    for (int i = 0; i < tokens; ++i) {
        Tokens_p[i].string_p = NULL;
    }
    nh_core_getConfigTokens(Tokens_p, bytes_p, NULL);

    nh_core_free(bytes_p);

    Tokenizer_p->tokens   = tokens;
    Tokenizer_p->Tokens_p = Tokens_p;

NH_CORE_END(NH_CORE_SUCCESS)
}

void nh_core_freeConfigTokenizer(
    nh_ConfigTokenizer *Tokenizer_p)
{
NH_CORE_BEGIN()

    for (int i = 0; i < Tokenizer_p->tokens; ++i) {
        if (Tokenizer_p->Tokens_p[i].string_p) {free(Tokenizer_p->Tokens_p[i].string_p);}
    }

    free(Tokenizer_p->Tokens_p);
    Tokenizer_p->tokens = 0;

NH_CORE_SILENT_END()
}

