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

typedef struct {
    nh_ecmascript_Token *tokens;
    int length;
    int idx;
} nh_ecmascript_IntrinsicTemplateParser;

static void parser_error(nh_ecmascript_IntrinsicTemplateParser *p, const char *fmt, ...) {
    va_list ap;
    fprintf(stderr, "[parse error] at token %d: ", p->idx);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}

static bool is_token(nh_ecmascript_IntrinsicTemplateParser *p, NH_ECMASCRIPT_TOKEN t) {
    if (p->idx >= p->length) return false;
    return p->tokens[p->idx].type == t;
}

static const char *tokstr(
    nh_ecmascript_IntrinsicTemplateParser *p)
{
    if (p->idx >= p->length) return "<EOF>";
    nh_encoding_UTF8String s = p->tokens[p->idx].String;
    static char buf[256];
    int n = s.length < 200 ? s.length : 200;
    memcpy(buf, s.p, n);
    buf[n] = 0;
    return buf;
}

static bool accept_punct(
    nh_ecmascript_IntrinsicTemplateParser *p, const char *c)
{
    if (!is_token(p, NH_ECMASCRIPT_TOKEN_PUNCTUATOR)) return false;
    nh_encoding_UTF8String s = p->tokens[p->idx].String;
    if ((int)strlen(c) == s.length && memcmp(s.p, c, s.length) == 0) {
        p->idx++;
        return true;
    }
    return false;
}

static bool accept_identifier(
    nh_ecmascript_IntrinsicTemplateParser *p, char **out)
{
    if (!is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) return false;
    nh_encoding_UTF8String s = p->tokens[p->idx].String;
    *out = (char*)malloc(s.length + 1);
    memcpy(*out, s.p, s.length);
    (*out)[s.length] = 0;
    p->idx++;
    return true;
}

static bool accept_string(
    nh_ecmascript_IntrinsicTemplateParser *p, char **out)
{
    if (!is_token(p, NH_ECMASCRIPT_TOKEN_STRING)) return false;
    nh_encoding_UTF8String s = p->tokens[p->idx].String;
    // strip quotes if present
    int start = 0, end = s.length;
    if (s.length >= 2 && (s.p[0] == '"' || s.p[0] == '\'')) {
        start = 1; end = s.length - 1;
    }
    int len = end - start;
    *out = (char*)malloc(len + 1);
    memcpy(*out, s.p + start, len);
    (*out)[len] = 0;
    p->idx++;
    return true;
}

static bool accept_number(
    nh_ecmascript_IntrinsicTemplateParser *p, char **out)
{
    if (!is_token(p, NH_ECMASCRIPT_TOKEN_DECIMAL_LITERAL)
        && !is_token(p, NH_ECMASCRIPT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL)
        && !is_token(p, NH_ECMASCRIPT_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL)
        && !is_token(p, NH_ECMASCRIPT_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL)
        && !is_token(p, NH_ECMASCRIPT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL)) {
        return false;
    }
    nh_encoding_UTF8String s = p->tokens[p->idx].String;
    *out = (char*)malloc(s.length + 1);
    memcpy(*out, s.p, s.length);
    (*out)[s.length] = 0;
    p->idx++;
    return true;
}

/* Recognize an intrinsic reference of form: % Identifier % 
   Tokenizer will produce '%' as punctuator, then IDENTIFIER, then '%' as punctuator.
   If your tokenizer returns the whole %Name% as one token, adapt accordingly. */
static bool accept_intrinsic_ref(
    nh_ecmascript_IntrinsicTemplateParser *p, char **out)
{
    int save = p->idx;
    if (!accept_punct(p, "%")) { p->idx = save; return false; }
    if (!is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) { p->idx = save; return false; }
    nh_encoding_UTF8String s = p->tokens[p->idx].String;
    *out = (char*)malloc(s.length + 1);
    memcpy(*out, s.p, s.length);
    (*out)[s.length] = 0;
    p->idx++;
    if (!accept_punct(p, "%")) { free(*out); p->idx = save; return false; }
    return true;
}

/* Accept literal token "null" or "current" represented as identifier tokens */
static bool accept_special_ident(
    nh_ecmascript_IntrinsicTemplateParser *p, const char *word)
{
    if (!is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) return false;
    nh_encoding_UTF8String s = p->tokens[p->idx].String;
    if ((int)strlen(word) == s.length && memcmp(s.p, word, s.length) == 0) {
        p->idx++;
        return true;
    }
    return false;
}

/* ---------- AST allocation helpers ---------- */

static nh_ecmascript_IntrinsicTemplateValue *ast_new_value(
    NH_ECMASCRIPT_INTRINSIC_VALUE_E k)
{
    nh_ecmascript_IntrinsicTemplateValue *v = (nh_ecmascript_IntrinsicTemplateValue*)malloc(sizeof(nh_ecmascript_IntrinsicTemplateValue));
    memset(v, 0, sizeof(nh_ecmascript_IntrinsicTemplateValue));
    v->kind = k;
    return v;
}

static nh_ecmascript_IntrinsicTemplateProperty *ast_new_property(
    const char *name, nh_ecmascript_IntrinsicTemplateValue *val)
{
    nh_ecmascript_IntrinsicTemplateProperty *p = (nh_ecmascript_IntrinsicTemplateProperty*)malloc(sizeof(nh_ecmascript_IntrinsicTemplateProperty));
    p->name = strdup(name);
    p->value = val;
    return p;
}

static nh_ecmascript_InternalSlot *ast_new_internalslot(const char *name, nh_ecmascript_IntrinsicTemplateValue *val) {
    nh_ecmascript_InternalSlot *s = (nh_ecmascript_InternalSlot*)malloc(sizeof(nh_ecmascript_InternalSlot));
    s->name = strdup(name);
    s->value = val;
    return s;
}

static nh_ecmascript_IntrinsicTemplate *ast_new_intrinsic(const char *name) {
    nh_ecmascript_IntrinsicTemplate *i = (nh_ecmascript_IntrinsicTemplate*)malloc(sizeof(nh_ecmascript_IntrinsicTemplate));
    memset(i, 0, sizeof(*i));
    i->name = name ? strdup(name) : NULL;
    return i;
}

/* ---------- Parsing functions ---------- */

static nh_ecmascript_IntrinsicTemplateValue *parse_value(nh_ecmascript_IntrinsicTemplateParser *p); // forward

/* parse an attributes set: { writable, configurable } -> returns NH_ECMASCRIPT_INTRINSIC_VALUE_ATTRIBUTES_SET */
static nh_ecmascript_IntrinsicTemplateValue *parse_attributes_set(nh_ecmascript_IntrinsicTemplateParser *p) {
    if (!accept_punct(p, "{")) return NULL;
    nh_ecmascript_IntrinsicTemplateValue *v = ast_new_value(NH_ECMASCRIPT_INTRINSIC_VALUE_ATTRIBUTES_SET);
    v->attr_names = NULL;
    v->attr_count = 0;

    while (!accept_punct(p, "}")) {
        // expect identifier
        if (!is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
            parser_error(p, "expected attribute identifier inside attributes set, got '%s'", tokstr(p));
            // try to recover: skip token
            p->idx++;
            continue;
        }
        nh_encoding_UTF8String s = p->tokens[p->idx].String;
        char *name = (char*)malloc(s.length + 1);
        memcpy(name, s.p, s.length);
        name[s.length] = 0;
        p->idx++;

        v->attr_names = (char**)realloc(v->attr_names, sizeof(char*) * (v->attr_count + 1));
        v->attr_names[v->attr_count++] = name;

        // optional comma
        accept_punct(p, ",");
    }

    return v;
}

/* parse a small object literal used in DSL: { key: value, ... } -> NH_ECMASCRIPT_INTRINSIC_VALUE_OBJECT */
static nh_ecmascript_IntrinsicTemplateValue *parse_object_literal(nh_ecmascript_IntrinsicTemplateParser *p) {
    if (!accept_punct(p, "{")) return NULL;
    nh_ecmascript_IntrinsicTemplateValue *v = ast_new_value(NH_ECMASCRIPT_INTRINSIC_VALUE_OBJECT);
    v->props = NULL;
    v->props_count = 0;

    while (!accept_punct(p, "}")) {
        // key can be string or identifier
        char *key = NULL;
        if (is_token(p, NH_ECMASCRIPT_TOKEN_STRING)) {
            accept_string(p, &key);
        } else if (is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
            accept_identifier(p, &key);
        } else {
            parser_error(p, "expected property key (string or identifier) in object literal, got '%s'", tokstr(p));
            // attempt to resync: skip token
            p->idx++;
            continue;
        }

        if (!accept_punct(p, ":")) {
            parser_error(p, "expected ':' after property key '%s'", key);
            // attempt recovery
        }

        // special-case: attributes: { writable, configurable } (i.e. value can be attributes set)
        nh_ecmascript_IntrinsicTemplateValue *val = NULL;
        if (is_token(p, NH_ECMASCRIPT_TOKEN_PUNCTUATOR) && p->tokens[p->idx].String.length == 1 && p->tokens[p->idx].String.p[0] == '{') {
            // Could be attributes set or nested object. We try parsing attributes set first by peeking inside:
            // If inside starts with identifier -> attributes set, else usual object.
            // Save index
            int save = p->idx;
            p->idx++; // skip '{' to peek
            bool firstIsId = is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER);
            p->idx = save;
            if (firstIsId) {
                val = parse_attributes_set(p);
            } else {
                val = parse_object_literal(p);
            }
        } else {
            val = parse_value(p);
        }

        nh_ecmascript_IntrinsicTemplateProperty *prop = ast_new_property(key, val);
        v->props = (nh_ecmascript_IntrinsicTemplateProperty**)realloc(v->props, sizeof(nh_ecmascript_IntrinsicTemplateProperty*) * (v->props_count + 1));
        v->props[v->props_count++] = prop;

        // optional comma
        accept_punct(p, ",");
    }

    return v;
}

/* Parse a general value token: string, number, identifier, intrinsic ref, null, current, object literal */
static nh_ecmascript_IntrinsicTemplateValue *parse_value(nh_ecmascript_IntrinsicTemplateParser *p) {
    // intrinsic ref %Name%
    if (is_token(p, NH_ECMASCRIPT_TOKEN_PUNCTUATOR) && p->tokens[p->idx].String.length == 1 && p->tokens[p->idx].String.p[0] == '%') {
        char *name = NULL;
        if (accept_intrinsic_ref(p, &name)) {
            nh_ecmascript_IntrinsicTemplateValue *v = ast_new_value(NH_ECMASCRIPT_INTRINSIC_VALUE_INTRINSIC_REF);
            v->raw = name;
            return v;
        }
    }

    if (is_token(p, NH_ECMASCRIPT_TOKEN_STRING)) {
        char *s = NULL;
        accept_string(p, &s);
        nh_ecmascript_IntrinsicTemplateValue *v = ast_new_value(NH_ECMASCRIPT_INTRINSIC_VALUE_STRING);
        v->raw = s;
        return v;
    }

    if (is_token(p, NH_ECMASCRIPT_TOKEN_DECIMAL_LITERAL)
        || is_token(p, NH_ECMASCRIPT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL)
        || is_token(p, NH_ECMASCRIPT_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL)
        || is_token(p, NH_ECMASCRIPT_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL)
        || is_token(p, NH_ECMASCRIPT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL)) {
        char *n = NULL;
        accept_number(p, &n);
        nh_ecmascript_IntrinsicTemplateValue *v = ast_new_value(NH_ECMASCRIPT_INTRINSIC_VALUE_NUMBER);
        v->raw = n;
        return v;
    }

    if (is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
        // identifiers might be "null", "current", or a bare identifier value (e.g., constructor)
        nh_encoding_UTF8String s = p->tokens[p->idx].String;
        if ((int)strlen("null") == s.length && memcmp(s.p, "null", s.length) == 0) {
            p->idx++;
            nh_ecmascript_IntrinsicTemplateValue *v = ast_new_value(NH_ECMASCRIPT_INTRINSIC_VALUE_NULL);
            v->raw = strdup("null");
            return v;
        }
        if ((int)strlen("current") == s.length && memcmp(s.p, "current", s.length) == 0) {
            p->idx++;
            nh_ecmascript_IntrinsicTemplateValue *v = ast_new_value(NH_ECMASCRIPT_INTRINSIC_VALUE_IDENTIFIER);
            v->raw = strdup("current");
            return v;
        }
        // fallback: identifier as a bare value
        char *id = NULL;
        accept_identifier(p, &id);
        nh_ecmascript_IntrinsicTemplateValue *v = ast_new_value(NH_ECMASCRIPT_INTRINSIC_VALUE_IDENTIFIER);
        v->raw = id;
        return v;
    }

    // object literal
    if (is_token(p, NH_ECMASCRIPT_TOKEN_PUNCTUATOR) && p->tokens[p->idx].String.length == 1 && p->tokens[p->idx].String.p[0] == '{') {
        return parse_object_literal(p);
    }

    parser_error(p, "unexpected token when parsing value: '%s'", tokstr(p));
    return NULL;
}

/* parse internalSlots { [[Call]]: ObjectConstructorCall ... } */
static bool parse_internalSlots(nh_ecmascript_IntrinsicTemplateParser *p, nh_ecmascript_IntrinsicTemplate *out) {
    if (!accept_punct(p, "internalSlots")) return false;
    if (!accept_punct(p, "{")) { parser_error(p, "expected '{' after internalSlots"); return false; }

    while (!accept_punct(p, "}")) {
        // slot name expected as identifier but many of these use [[...]] which your tokenizer will treat
        // as an identifier token if it considered brackets part of identifier; otherwise it's likely tokenized as punctuators.
        // We'll accept either IDENTIFIER (like [[Call]]) or sequence of punctuators + identifier + punctuator.
        char *slotName = NULL;
        if (is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
            accept_identifier(p, &slotName);
        } else {
            // try to build name from current token sequence until colon
            // collect raw token texts up to ':'
            int start = p->idx;
            int j = start;
            while (j < p->length) {
                if (is_token((nh_ecmascript_IntrinsicTemplateParser*)&(nh_ecmascript_IntrinsicTemplateParser){p->tokens, p->length, j}, NH_ECMASCRIPT_TOKEN_PUNCTUATOR)) {
                    nh_encoding_UTF8String ts = p->tokens[j].String;
                    if (ts.length == 1 && ts.p[0] == ':') break;
                } else if (is_token((nh_ecmascript_IntrinsicTemplateParser*)&(nh_ecmascript_IntrinsicTemplateParser){p->tokens, p->length, j}, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
                    // maybe the last identifier before ':'; continue
                }
                // break if colon reached in tokens
                if (is_token((nh_ecmascript_IntrinsicTemplateParser*)&(nh_ecmascript_IntrinsicTemplateParser){p->tokens, p->length, j}, NH_ECMASCRIPT_TOKEN_PUNCTUATOR)) {
                    nh_encoding_UTF8String ts = p->tokens[j].String;
                    if (ts.length == 1 && ts.p[0] == ':') break;
                }
                j++;
            }
            // join tokens start..j into slotName
            int total = 0;
            for (int k = start; k < j; ++k) total += p->tokens[k].String.length;
            slotName = (char*)malloc(total + 1 + (j - start)); // extra for spaces
            char *wp = slotName;
            for (int k = start; k < j; ++k) {
                nh_encoding_UTF8String ts = p->tokens[k].String;
                memcpy(wp, ts.p, ts.length); wp += ts.length;
                if (k + 1 < j) { *wp++ = ' '; }
            }
            *wp = 0;
            p->idx = j;
        }

        if (!accept_punct(p, ":")) {
            parser_error(p, "expected ':' after internal slot name '%s'", slotName ? slotName : "<null>");
        }

        nh_ecmascript_IntrinsicTemplateValue *val = parse_value(p);
        nh_ecmascript_InternalSlot *slot = ast_new_internalslot(slotName, val);
        out->internalSlots = (nh_ecmascript_InternalSlot**)realloc(out->internalSlots, sizeof(nh_ecmascript_InternalSlot*) * (out->internalSlotsCount + 1));
        out->internalSlots[out->internalSlotsCount++] = slot;

        accept_punct(p, ",");
    }

    return true;
}

/* parse one property inside properties { ... } - a top-level property like "assign": { length: 2, attributes: { ... } } */
static nh_ecmascript_IntrinsicTemplateProperty *parse_property_definition(nh_ecmascript_IntrinsicTemplateParser *p) {
    // key as string or identifier
    char *key = NULL;
    if (is_token(p, NH_ECMASCRIPT_TOKEN_STRING)) {
        accept_string(p, &key);
    } else if (is_token(p, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
        accept_identifier(p, &key);
    } else {
        parser_error(p, "expected property name (string or identifier) in properties block, got '%s'", tokstr(p));
        return NULL;
    }

    if (!accept_punct(p, ":")) {
        parser_error(p, "expected ':' after property name '%s'", key);
    }

    // value is usually an object literal
    nh_ecmascript_IntrinsicTemplateValue *val = NULL;
    if (is_token(p, NH_ECMASCRIPT_TOKEN_PUNCTUATOR) && p->tokens[p->idx].String.length == 1 && p->tokens[p->idx].String.p[0] == '{') {
        val = parse_object_literal(p);
    } else {
        // allow shorthand "foo": { length: 1, attributes: { ... } } or "foo": <value>
        val = parse_value(p);
    }

    // optional comma
    accept_punct(p, ",");

    return ast_new_property(key, val);
}

static bool parse_properties_block(nh_ecmascript_IntrinsicTemplateParser *p, nh_ecmascript_IntrinsicTemplate *out) {
    if (!accept_punct(p, "properties")) return false;
    if (!accept_punct(p, "{")) { parser_error(p, "expected '{' after properties"); return false; }

    while (!accept_punct(p, "}")) {
        nh_ecmascript_IntrinsicTemplateProperty *prop = parse_property_definition(p);
        if (!prop) {
            // try to skip one token and continue
            p->idx++;
            continue;
        }
        out->properties = (nh_ecmascript_IntrinsicTemplateProperty**)realloc(out->properties, sizeof(nh_ecmascript_IntrinsicTemplateProperty*) * (out->propertiesCount + 1));
        out->properties[out->propertiesCount++] = prop;
    }

    return true;
}

/* parse intrinsic declaration */
nh_ecmascript_IntrinsicTemplate *nh_ecmascript_parseIntrinsicTemplate(
    nh_core_Array tokens)
{
    nh_ecmascript_IntrinsicTemplateParser Parser = { .tokens = tokens.p, .length = tokens.length, .idx = 0 };

    // expect "intrinsic"
    if (!is_token(&Parser, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
        parser_error(&Parser, "expected 'intrinsic' at start, got '%s'", tokstr(&Parser));
        return NULL;
    }
    // check identifier text
    nh_encoding_UTF8String startTok = Parser.tokens[Parser.idx].String;
    if (startTok.length != (int)strlen("intrinsic") || memcmp(startTok.p, "intrinsic", startTok.length) != 0) {
        parser_error(&Parser, "expected 'intrinsic' keyword, got '%s'", tokstr(&Parser));
        return NULL;
    }
    Parser.idx++;

    // name: identifier or %Name%
    char *intrName = NULL;
    if (is_token(&Parser, NH_ECMASCRIPT_TOKEN_PUNCTUATOR) && Parser.tokens[Parser.idx].String.length == 1 && Parser.tokens[Parser.idx].String.p[0] == '%') {
        if (!accept_intrinsic_ref(&Parser, &intrName)) {
            parser_error(&Parser, "failed to parse intrinsic name (%%Name%%)");
            return NULL;
        }
    } else {
        if (!accept_identifier(&Parser, &intrName)) {
            parser_error(&Parser, "expected intrinsic name identifier, got '%s'", tokstr(&Parser));
            return NULL;
        }
    }

    // open brace
    if (!accept_punct(&Parser, "{")) {
        parser_error(&Parser, "expected '{' after intrinsic name");
        return NULL;
    }

    nh_ecmascript_IntrinsicTemplate *Template_p = ast_new_intrinsic(intrName);

    // loop over body
    while (!accept_punct(&Parser, "}")) {
        // we expect identifiers like "type", "prototype", "instancePrototype", "internalSlots", "properties"
        if (!is_token(&Parser, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
            parser_error(&Parser, "expected field name inside intrinsic body, got '%s'", tokstr(&Parser));
            // try skip single token
            Parser.idx++;
            continue;
        }

        // get field name
        nh_encoding_UTF8String keyTok = Parser.tokens[Parser.idx].String;
        char *field = (char*)malloc(keyTok.length + 1);
        memcpy(field, keyTok.p, keyTok.length); field[keyTok.length] = 0;
        Parser.idx++;

        if (!accept_punct(&Parser, ":")) {
            parser_error(&Parser, "expected ':' after field name '%s'", field);
        }

        // handle known fields
        if (strcmp(field, "kind") == 0) {
            // value is an identifier
            if (!is_token(&Parser, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
                parser_error(&Parser, "expected identifier for type: got '%s'", tokstr(&Parser));
            } else {
                nh_encoding_UTF8String v = Parser.tokens[Parser.idx].String;
                char type_p[64] = {0};
                memcpy(type_p, v.p, v.length); 
                Template_p->kind = NH_ECMASCRIPT_OBJECT_ORDINARY;
                if (!strcmp(type_p, "function")) {
                    Template_p->kind = NH_ECMASCRIPT_OBJECT_FUNCTION;
                } else if (!strcmp(type_p, "array")) {
                    Template_p->kind = NH_ECMASCRIPT_OBJECT_ARRAY;
                }
                Parser.idx++;
            }
            accept_punct(&Parser, ",");
            free(field);
            continue;
        }

        if (strcmp(field, "prototype") == 0) {
            // value may be %Name% or identifier
            char *name = NULL;
            if (accept_intrinsic_ref(&Parser, &name)) {
                Template_p->prototype_p = name;
            } else {
                // maybe an identifier or string
                if (is_token(&Parser, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
                    accept_identifier(&Parser, &name);
                    Template_p->prototype_p = name;
                } else {
                    parser_error(&Parser, "expected intrinsic reference or identifier for prototype");
                }
            }
            accept_punct(&Parser, ",");
            free(field);
            continue;
        }

        if (strcmp(field, "instancePrototype") == 0) {
            char *name = NULL;
            if (accept_intrinsic_ref(&Parser, &name)) {
                Template_p->instancePrototype = name;
            } else {
                if (is_token(&Parser, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
                    accept_identifier(&Parser, &name);
                    Template_p->instancePrototype = name;
                } else {
                    parser_error(&Parser, "expected intrinsic reference or identifier for instancePrototype");
                }
            }
            accept_punct(&Parser, ",");
            free(field);
            continue;
        }

        if (strcmp(field, "internalSlots") == 0) {
            // p currently after ':'; we expect a block
            // we rewound earlier expecting the 'internalSlots' literal; but our accept_punct for 'internalSlots' won't match since it's an identifier.
            // call specialized parser:
            // but parse_internalSlots expects the token stream at 'internalSlots' token, so adjust: we've already consumed the name and ':'
            // parse_internalSlots expects the next token to be '{'
            // call parse_internalSlots by passing current state
            // so we just call parse_internalSlots which expects that name already consumed; but our parse_internalSlots checks for accept_punct(p, "internalSlots")
            // instead call parse_internalSlots_body here:
            if (!accept_punct(&Parser, "{")) {
                parser_error(&Parser, "expected '{' after internalSlots:");
            } else {
                // parse entries until '}'
                while (!accept_punct(&Parser, "}")) {
                    // slot name as identifier or bracketed form
                    char *slotName = NULL;
                    if (is_token(&Parser, NH_ECMASCRIPT_TOKEN_IDENTIFIER)) {
                        accept_identifier(&Parser, &slotName);
                    } else {
                        // attempt to capture [[Call]] form as series of tokens until ':'
                        int start = Parser.idx;
                        int j = start;
                        while (j < Parser.length) {
                            nh_encoding_UTF8String ts = Parser.tokens[j].String;
                            if (ts.length == 1 && ts.p[0] == ':') break;
                            j++;
                        }
                        int tot = 0;
                        for (int k = start; k < j; ++k) tot += Parser.tokens[k].String.length + 1;
                        slotName = (char*)malloc(tot + 1);
                        char *wp = slotName;
                        for (int k = start; k < j; ++k) {
                            nh_encoding_UTF8String tks = Parser.tokens[k].String;
                            memcpy(wp, tks.p, tks.length); wp += tks.length;
                            *wp++ = ' ';
                        }
                        wp[-1] = 0; // replace last space
                        Parser.idx = j;
                    }
                    if (!accept_punct(&Parser, ":")) {
                        parser_error(&Parser, "expected ':' after internal slot name");
                    }
                    nh_ecmascript_IntrinsicTemplateValue *v = parse_value(&Parser);
                    nh_ecmascript_InternalSlot *slot = ast_new_internalslot(slotName, v);
                    Template_p->internalSlots = (nh_ecmascript_InternalSlot**)realloc(Template_p->internalSlots, sizeof(nh_ecmascript_InternalSlot*) * (Template_p->internalSlotsCount + 1));
                    Template_p->internalSlots[Template_p->internalSlotsCount++] = slot;
                    accept_punct(&Parser, ",");
                }
            }
            accept_punct(&Parser, ",");
            free(field);
            continue;
        }

        if (strcmp(field, "properties") == 0) {
            if (!accept_punct(&Parser, "{")) {
                parser_error(&Parser, "expected '{' after properties");
            } else {
                while (!accept_punct(&Parser, "}")) {
                    nh_ecmascript_IntrinsicTemplateProperty *prop = parse_property_definition(&Parser);
                    if (prop) {
                        Template_p->properties = (nh_ecmascript_IntrinsicTemplateProperty**)realloc(Template_p->properties, sizeof(nh_ecmascript_IntrinsicTemplateProperty*) * (Template_p->propertiesCount + 1));
                        Template_p->properties[Template_p->propertiesCount++] = prop;
                    } else {
                        // skip token to try to move on
                        Parser.idx++;
                    }
                }
            }
            accept_punct(&Parser, ",");
            free(field);
            continue;
        }

        // unknown field: skip a value or object
        nh_ecmascript_IntrinsicTemplateValue *v = parse_value(&Parser);
        accept_punct(&Parser, ",");
        free(field);
        // no storage for unknown fields
    }

    return Template_p;
}

/* ---------- Utilities: dump and free ---------- */

static void dump_value(nh_ecmascript_IntrinsicTemplateValue *v, int indent);

static void dump_indent(int n) {
    for (int i = 0; i < n; ++i) putchar(' ');
}

static void dump_value(nh_ecmascript_IntrinsicTemplateValue *v, int indent) {
    if (!v) { dump_indent(indent); printf("<null>\n"); return; }
    switch (v->kind) {
        case NH_ECMASCRIPT_INTRINSIC_VALUE_STRING:
            dump_indent(indent); printf("STRING: \"%s\"\n", v->raw); break;
        case NH_ECMASCRIPT_INTRINSIC_VALUE_NUMBER:
            dump_indent(indent); printf("NUMBER: %s\n", v->raw); break;
        case NH_ECMASCRIPT_INTRINSIC_VALUE_IDENTIFIER:
            dump_indent(indent); printf("IDENT: %s\n", v->raw); break;
        case NH_ECMASCRIPT_INTRINSIC_VALUE_INTRINSIC_REF:
            dump_indent(indent); printf("INTRINSIC_REF: %s\n", v->raw); break;
        case NH_ECMASCRIPT_INTRINSIC_VALUE_NULL:
            dump_indent(indent); printf("NULL\n"); break;
        case NH_ECMASCRIPT_INTRINSIC_VALUE_ATTRIBUTES_SET:
            dump_indent(indent); printf("ATTRIBUTES_SET: [");
            for (int i = 0; i < v->attr_count; ++i) {
                if (i) printf(", ");
                printf("%s", v->attr_names[i]);
            }
            printf("]\n");
            break;
        case NH_ECMASCRIPT_INTRINSIC_VALUE_OBJECT:
            dump_indent(indent); printf("OBJECT {\n");
            for (int i = 0; i < v->props_count; ++i) {
                dump_indent(indent + 2); printf("%s: \n", v->props[i]->name);
                dump_value(v->props[i]->value, indent + 4);
            }
            dump_indent(indent); printf("}\n");
            break;
    }
}

static void nh_ecmascript_dump(nh_ecmascript_IntrinsicTemplate *i) {
    if (!i) return;
    printf("Intrinsic %s\n", i->name ? i->name : "<anon>");
    if (i->kind) printf("  kind: %d\n", i->kind);
    if (i->prototype_p) printf("  prototype: %s\n", i->prototype_p);
    if (i->instancePrototype) printf("  instancePrototype: %s\n", i->instancePrototype);
    if (i->internalSlotsCount) {
        printf("  internalSlots:\n");
        for (int k = 0; k < i->internalSlotsCount; ++k) {
            printf("    %s: \n", i->internalSlots[k]->name);
            dump_value(i->internalSlots[k]->value, 8);
        }
    }
    if (i->propertiesCount) {
        printf("  properties:\n");
        for (int k = 0; k < i->propertiesCount; ++k) {
            printf("    %s:\n", i->properties[k]->name);
            dump_value(i->properties[k]->value, 8);
        }
    }
}

/* Free helpers (partial, frees main allocations) */
static void nh_ecmascript_freeIntrinsicValue(nh_ecmascript_IntrinsicTemplateValue *v) {
    if (!v) return;
    if (v->raw) free(v->raw);
    if (v->kind == NH_ECMASCRIPT_INTRINSIC_VALUE_OBJECT) {
        for (int i = 0; i < v->props_count; ++i) {
            if (v->props[i]) {
                free(v->props[i]->name);
                nh_ecmascript_freeIntrinsicValue(v->props[i]->value);
                free(v->props[i]);
            }
        }
        free(v->props);
    } else if (v->kind == NH_ECMASCRIPT_INTRINSIC_VALUE_ATTRIBUTES_SET) {
        for (int i = 0; i < v->attr_count; ++i) free(v->attr_names[i]);
        free(v->attr_names);
    }
    free(v);
}

void nh_ecmascript_freeIntrinsicTemplate(
    nh_ecmascript_IntrinsicTemplate *Intrinsic_p)
{
    if (!Intrinsic_p) return;
    free(Intrinsic_p->name);
    free(Intrinsic_p->prototype_p);
    free(Intrinsic_p->instancePrototype);
    for (int k = 0; k < Intrinsic_p->internalSlotsCount; ++k) {
        free(Intrinsic_p->internalSlots[k]->name);
        nh_ecmascript_freeIntrinsicValue(Intrinsic_p->internalSlots[k]->value);
        free(Intrinsic_p->internalSlots[k]);
    }
    free(Intrinsic_p->internalSlots);
    for (int k = 0; k < Intrinsic_p->propertiesCount; ++k) {
        free(Intrinsic_p->properties[k]->name);
        nh_ecmascript_freeIntrinsicValue(Intrinsic_p->properties[k]->value);
        free(Intrinsic_p->properties[k]);
    }
    free(Intrinsic_p->properties);
    free(Intrinsic_p);
}

nh_core_List nh_ecmascript_parseIntrinsicTemplates()
{
    nh_core_List Templates = nh_core_initList(16);

    for (int i = 0; i < NH_ECMASCRIPT_INTRINSIC_TEMPLATE_NAMES_PP_COUNT; ++i) {
        nh_core_Array Tokens = nh_ecmascript_tokenizeIntrinsicTemplate(NULL, NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP[i]);
        nh_ecmascript_IntrinsicTemplate *Template_p = nh_ecmascript_parseIntrinsicTemplate(Tokens);
        nh_core_freeArray(&Tokens);
        nh_core_appendToList(&Templates, Template_p);
    }

    return Templates;
}
