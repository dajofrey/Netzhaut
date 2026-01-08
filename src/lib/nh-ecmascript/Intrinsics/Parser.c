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

// Helper: Peek and verify token type/string
static bool nh_ecmascript_is(nh_ecmascript_TemplateParser *P, const char *s) {
    nh_ecmascript_Token *t = nh_ecmascript_peek(P);
    return t && strcmp(t->String.p, s) == 0;
}

static char* nh_ecmascript_parseKey(nh_ecmascript_TemplateParser *P) {
    nh_ecmascript_Token *t = nh_ecmascript_peek(P);
    if (!t) return strdup("EOF");

    // Case 1 & 2: Brackets (Slots or Symbols)
    if (strcmp(t->String.p, "[") == 0) {
        char buffer[1024] = {0};
        nh_ecmascript_Token *next = nh_ecmascript_peekNext(P);
        
        if (next && strcmp(next->String.p, "[") == 0) {
            nh_ecmascript_consume(P); // [
            nh_ecmascript_consume(P); // [
            char *name = nh_ecmascript_consume(P)->String.p;
            if (nh_ecmascript_is(P, "]")) nh_ecmascript_consume(P);
            if (nh_ecmascript_is(P, "]")) nh_ecmascript_consume(P);
            snprintf(buffer, sizeof(buffer), "[[%s]]", name);
            return strdup(buffer);
        }

        int depth = 0;
        while (P->pos < P->count) {
            nh_ecmascript_Token *part = nh_ecmascript_consume(P);
            strncat(buffer, part->String.p, sizeof(buffer) - strlen(buffer) - 1);
            if (strcmp(part->String.p, "[") == 0) depth++;
            else if (strcmp(part->String.p, "]") == 0) {
                depth--;
                if (depth == 0) break;
            }
        }
        return strdup(buffer);
    }

    // Case 3: Strings (Strip quotes)
    if (t->type == NH_ECMASCRIPT_TOKEN_STRING) {
        nh_ecmascript_consume(P);
        char *src = t->String.p;
        size_t len = strlen(src);
        if (len >= 2 && src[0] == '"' && src[len-1] == '"') {
            return strndup(src + 1, len - 2);
        }
        return strdup(src);
    }

    // Case 4: Standard Identifiers
    return strdup(nh_ecmascript_consume(P)->String.p);
}

// Fixed Node Parser
static nh_ecmascript_TemplateNode* nh_ecmascript_parseNode(nh_ecmascript_TemplateParser *P) {
    nh_ecmascript_Token *t = nh_ecmascript_peek(P);
    if (!t) return NULL;

    nh_ecmascript_TemplateNode *node = calloc(1, sizeof(nh_ecmascript_TemplateNode));

    // 1. INTRINSIC REFERENCE: %Name%
    if (nh_ecmascript_is(P, "%")) {
        nh_ecmascript_consume(P); // skip %
        node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_INTRINSIC;
        node->data.string_p = strdup(nh_ecmascript_consume(P)->String.p);
        nh_ecmascript_consume(P); // skip closing %
    }
    // 2. PAIRS or NODES: { ... }
    else if (nh_ecmascript_is(P, "{")) {
        nh_ecmascript_consume(P); // skip {
        
        nh_ecmascript_Token *next = nh_ecmascript_peekNext(P);
        if (next && strcmp(next->String.p, ":") == 0) {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_PAIRS;
            node->data.Pairs = nh_core_initList(8);

            // Inside nh_ecmascript_parseIntrinsicTemplate, the internalSlots/properties loop:
            while (!nh_ecmascript_is(P, "}")) {
                nh_ecmascript_KeyValuePair *pair = calloc(1, sizeof(nh_ecmascript_KeyValuePair));
                
                // 1. Get the key
                pair->key = nh_ecmascript_parseKey(P);
                pair->key_is_symbol = (pair->key[0] == '[' && pair->key[1] != '[');
            
                // 2. MUST consume the colon
                if (nh_ecmascript_is(P, ":")) {
                    nh_ecmascript_consume(P); 
                }
            
                // 3. Parse the value node
                pair->value = nh_ecmascript_parseNode(P);
                nh_core_appendToList(&node->data.Pairs, pair);
            
                // 4. IMPORTANT: Skip the comma if it exists! 
                // This stops the next loop iteration from seeing ',' as a key.
                if (nh_ecmascript_is(P, ",")) {
                    nh_ecmascript_consume(P);
                }
            }
        } else {
            node->kind = NH_ECMASCRIPT_TEMPLATE_NODE_NODES;
            node->data.Nodes = nh_core_initList(8);
            while (!nh_ecmascript_is(P, "}")) {
                nh_core_appendToList(&node->data.Nodes, nh_ecmascript_parseNode(P));
                if (nh_ecmascript_is(P, ",")) nh_ecmascript_consume(P);
            }
        }
        nh_ecmascript_consume(P); // skip }
    }
    // 3. PRIMITIVES
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

nh_ecmascript_IntrinsicTemplate* nh_ecmascript_parseIntrinsicTemplate(nh_ecmascript_Token *tokens, int count) {
    nh_ecmascript_TemplateParser P = { tokens, count, 0 };
    
    if (!nh_ecmascript_is(&P, "intrinsic")) return NULL;
    nh_ecmascript_consume(&P); 

    nh_ecmascript_IntrinsicTemplate *T = calloc(1, sizeof(nh_ecmascript_IntrinsicTemplate));
    T->InternalSlots = nh_core_initList(8);
    T->Properties = nh_core_initList(8);

    // Parse %Name%
    nh_ecmascript_consume(&P); // %
    T->name = strdup(nh_ecmascript_consume(&P)->String.p);
    nh_ecmascript_consume(&P); // %
    nh_ecmascript_consume(&P); // {

    while (!nh_ecmascript_is(&P, "}")) {
        nh_ecmascript_Token *sectionTok = nh_ecmascript_consume(&P);
        char *section_name = sectionTok->String.p;

        if (strcmp(section_name, "internalSlots") == 0 || strcmp(section_name, "properties") == 0) {
            nh_core_List *target_list = (section_name[0] == 'i') ? &T->InternalSlots : &T->Properties;
            nh_ecmascript_consume(&P); // {

            // Inside nh_ecmascript_parseIntrinsicTemplate
            while (!nh_ecmascript_is(&P, "}")) {
                nh_ecmascript_KeyValuePair *pair = calloc(1, sizeof(nh_ecmascript_KeyValuePair));
            
                // Delegate ALL key parsing to the specialized function
                pair->key = nh_ecmascript_parseKey(&P);
                pair->key_is_symbol = (pair->key[0] == '[' && pair->key[1] != '[');
            
                nh_ecmascript_consume(&P); // skip :
                pair->value = nh_ecmascript_parseNode(&P);
                nh_core_appendToList(target_list, pair);
                
                // Optional: skip comma if your template uses them
                if (nh_ecmascript_is(&P, ",")) nh_ecmascript_consume(&P);
            }

            nh_ecmascript_consume(&P); // }
        } else {
            // Top-level field handling
            nh_ecmascript_KeyValuePair *pair = calloc(1, sizeof(nh_ecmascript_KeyValuePair));
            pair->key = strdup(section_name);
            nh_ecmascript_consume(&P); // :
            pair->value = nh_ecmascript_parseNode(&P);
            nh_core_appendToList(&T->InternalSlots, pair);
        }
    }
    
    nh_ecmascript_consume(&P); // }
    return T;
}
