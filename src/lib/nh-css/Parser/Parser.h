#ifndef NH_CSS_PARSER_H
#define NH_CSS_PARSER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "SelectorParser.h"
#include "TokenParser.h"
#include "Tokenizer.h"

#include "../Common/Includes.h"

extern nh_webidl_Object *NH_CSS_DEFAULT_STYLE_SHEET_P;

typedef struct nh_css_ComponentValueData {
    nh_core_Array Tokens;
    nh_core_Array ComponentValues;
} nh_css_ComponentValueData;

typedef struct nh_css_DeclarationData {
    nh_core_Array Tokens;
    nh_core_Array Declarations; 
} nh_css_DeclarationData;

nh_webidl_Object *nh_css_parseStyleSheetFromUTF8( 
    char *data_p, size_t length, nh_webidl_Object *Document_p
); 

nh_webidl_Object *nh_css_parseStyleSheetFromUTF32(
    nh_encoding_UTF32String *String_p, nh_webidl_Object *Document_p
);

nh_css_ComponentValueData nh_css_parseComponentValuesFromUTF8Codepoints(
    char *logId_p, char *p, unsigned long long length
);

nh_css_DeclarationData nh_css_parseDeclarationsFromBytes(
    char *logId_p, char *p, unsigned long long length
);

#endif
