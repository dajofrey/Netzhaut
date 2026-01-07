// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Parser.h"
#include "Tokenizer.h"
#include "Templates.h"

#include "../Common/Log.h"

#include "../../nh-encoding/Base/UnicodeDataHelper.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

// FUNCTIONS =======================================================================================

typedef struct nh_ecmascript_TemplateParser{
    nh_ecmascript_Token *tokens;
    int count;
    int pos;
} nh_ecmascript_TemplateParser;

// Helper: Get current token and advance
static nh_ecmascript_Token* nh_ecmascript_consume(
    nh_ecmascript_TemplateParser *Parser_p)
{
    if (Parser_p->pos >= Parser_p->count) return NULL;
    // Skip trivia: whitespace, line terminators, comments
    while (Parser_p->pos < Parser_p->count && (Parser_p->tokens[Parser_p->pos].type <= NH_ECMASCRIPT_TOKEN_COMMENT)) {
        Parser_p->pos++;
    }
    return &Parser_p->tokens[Parser_p->pos++];
}

// Helper: Peek at the token AFTER the next meaningful token without advancing
static nh_ecmascript_Token* nh_ecmascript_peekNext(
    nh_ecmascript_TemplateParser *Parser_p)
{
    int temp_pos = Parser_p->pos;

    // 1. Skip trivia to find the FIRST meaningful token
    while (temp_pos < Parser_p->count && (Parser_p->tokens[temp_pos].type <= NH_ECMASCRIPT_TOKEN_COMMENT)) {
        temp_pos++;
    }

    // 2. Advance past that first meaningful token
    if (temp_pos < Parser_p->count) {
        temp_pos++;
    }

    // 3. Skip trivia again to find the SECOND meaningful token
    while (temp_pos < Parser_p->count && (Parser_p->tokens[temp_pos].type <= NH_ECMASCRIPT_TOKEN_COMMENT)) {
        temp_pos++;
    }

    // 4. Return the second token if it exists
    return (temp_pos < Parser_p->count) ? &Parser_p->tokens[temp_pos] : NULL;
}

// Helper: Peek without advancing
static nh_ecmascript_Token* nh_ecmascript_peek(
    nh_ecmascript_TemplateParser *Parser_p)
{
    int temp_pos = Parser_p->pos;
    while (temp_pos < Parser_p->count && (Parser_p->tokens[temp_pos].type <= NH_ECMASCRIPT_TOKEN_COMMENT)) {
        temp_pos++;
    }
    return (temp_pos < Parser_p->count) ? &Parser_p->tokens[temp_pos] : NULL;
}

// Helper: Recursive Node Parser (The heart of "No Interpretation")
static nh_ecmascript_TemplateNode* nh_ecmascript_parseNode(
    nh_ecmascript_TemplateParser *P)
{
    nh_ecmascript_Token *t = nh_ecmascript_peek(P);
    nh_ecmascript_TemplateNode *node = calloc(1, sizeof(nh_ecmascript_TemplateNode));

    // 1. INTRINSIC REFERENCE: %Name%
    if (t->type == NH_ECMASCRIPT_TOKEN_PUNCTUATOR && strcmp(t->String.p, "%") == 0) {
        nh_ecmascript_consume(P); // skip %
        node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_INTRINSIC;
        node->data.string_p = strdup(nh_ecmascript_consume(P)->String.p);
        nh_ecmascript_consume(P); // skip closing %
    }
    // 2. PAIRS or NODES: { ... }
    else if (t->type == NH_ECMASCRIPT_TOKEN_PUNCTUATOR && strcmp(t->String.p, "{") == 0) {
        nh_ecmascript_consume(P); // skip {
        
        // Peek to see if it's a mapping { key: value } or a list { val1, val2 }
        nh_ecmascript_Token *next = nh_ecmascript_peekNext(P);
        if (next && next->type == NH_ECMASCRIPT_TOKEN_PUNCTUATOR && strcmp(next->String.p, ":") == 0) {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_PAIRS;
            node->data.Pairs = nh_core_initList(8);
            while (strcmp(nh_ecmascript_peek(P)->String.p, "}") != 0) {
                nh_ecmascript_KeyValuePair *pair = calloc(1, sizeof(nh_ecmascript_KeyValuePair));
                nh_ecmascript_Token *keyTok = nh_ecmascript_consume(P);
                
                pair->key = strdup(keyTok->String.p);
                pair->key_is_symbol = (pair->key[0] == '@' || pair->key[0] == '[');
                
                nh_ecmascript_consume(P); // skip :
                pair->value = nh_ecmascript_parseNode(P);
                
                nh_core_appendToList(&node->data.Pairs, pair);
                if (strcmp(nh_ecmascript_peek(P)->String.p, ",") == 0) nh_ecmascript_consume(P);
            }
        } else {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_NODES;
            node->data.Nodes = nh_core_initList(8);
            while (strcmp(nh_ecmascript_peek(P)->String.p, "}") != 0) {
                nh_core_appendToList(&node->data.Nodes, nh_ecmascript_parseNode(P));
                if (strcmp(nh_ecmascript_peek(P)->String.p, ",") == 0) nh_ecmascript_consume(P);
            }
        }
        nh_ecmascript_consume(P); // skip }
    }
    // 3. PRIMITIVES & IDENTIFIERS
    else {
        t = nh_ecmascript_consume(P);
        if (t->type == NH_ECMASCRIPT_TOKEN_STRING) {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_STRING;
            node->data.string_p = strdup(t->String.p);
        } else if (t->type == NH_ECMASCRIPT_TOKEN_DECIMAL_LITERAL) {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_NUMBER;
            node->data.number = atof(t->String.p);
        } else if (strcmp(t->String.p, "true") == 0 || strcmp(t->String.p, "false") == 0) {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_BOOLEAN;
            node->data.boolean = (t->String.p[0] == 't');
        } else if (strcmp(t->String.p, "null") == 0) {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_NULL;
        } else {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_IDENTIFIER;
            node->data.string_p = strdup(t->String.p);
        }
    }
    return node;
}

nh_ecmascript_IntrinsicTemplate* nh_ecmascript_parseIntrinsicTemplate(
    nh_ecmascript_Token *tokens,
    int count) 
{
    nh_ecmascript_TemplateParser P = { tokens, count, 0 };
    if (strcmp(nh_ecmascript_consume(&P)->String.p, "intrinsic") != 0) return NULL;

    nh_ecmascript_IntrinsicTemplate *T = calloc(1, sizeof(nh_ecmascript_IntrinsicTemplate));
    T->InternalSlots = nh_core_initList(8);
    T->Properties = nh_core_initList(8);

    // Parse %Name%
    nh_ecmascript_consume(&P); // skip %
    T->name = strdup(nh_ecmascript_consume(&P)->String.p);
    nh_ecmascript_consume(&P); // skip %
    nh_ecmascript_consume(&P); // skip {

    while (strcmp(nh_ecmascript_peek(&P)->String.p, "}") != 0) {
        char *section_name = nh_ecmascript_consume(&P)->String.p;
        
        // internalSlots { ... } or properties { ... }
        if (strcmp(section_name, "internalSlots") == 0 || strcmp(section_name, "properties") == 0) {
            nh_core_List *target_list = (section_name[0] == 'i') ? &T->InternalSlots : &T->Properties;
            nh_ecmascript_consume(&P); // skip {
            
            while (strcmp(nh_ecmascript_peek(&P)->String.p, "}") != 0) {
                nh_ecmascript_KeyValuePair *pair = calloc(1, sizeof(nh_ecmascript_KeyValuePair));
                
                // Handle slot name [[Name]] vs property name "Name"
                nh_ecmascript_Token *keyTok = nh_ecmascript_consume(&P);
                pair->key = strdup(keyTok->String.p);
                pair->key_is_symbol = (pair->key[0] == '@' || pair->key[0] == '[');
                
                nh_ecmascript_consume(&P); // skip :
                pair->value = nh_ecmascript_parseNode(&P);
                
                nh_core_appendToList(target_list, pair);
            }
            nh_ecmascript_consume(&P); // skip }
        }
        else {
            // Top-level fields (like 'freeze: true' or 'isExotic: true')
            // We treat these as Internal Slots for the sake of "No Interpretation"
            nh_ecmascript_KeyValuePair *pair = calloc(1, sizeof(nh_ecmascript_KeyValuePair));
            pair->key = strdup(section_name);
            nh_ecmascript_consume(&P); // skip :
            pair->value = nh_ecmascript_parseNode(&P);
            nh_core_appendToList(&T->InternalSlots, pair);
        }
    }
    
    nh_ecmascript_consume(&P); // skip closing }
    return T;
}
