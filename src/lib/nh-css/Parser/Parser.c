// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Parser.h"

#include "../Interfaces/StyleSheet.h"
#include "../Common/Log.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

nh_ecmascript_Object *NH_CSS_DEFAULT_STYLE_SHEET_P = NULL;
	
// FUNCTIONS =======================================================================================

nh_ecmascript_Object *nh_css_parseStyleSheetFromUTF8(
    char *data_p, size_t length, nh_ecmascript_Object *Document_p)
{
    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(data_p, length, NULL);
    nh_ecmascript_Object *CSSStyleSheet_p = nh_css_parseStyleSheetFromUTF32(&String, Document_p);
    nh_encoding_freeUTF32(&String);
    return CSSStyleSheet_p;
}

nh_ecmascript_Object *nh_css_parseStyleSheetFromUTF32(
    nh_encoding_UTF32String *String_p, nh_ecmascript_Object *Document_p)
{
    nh_core_Array Tokens = nh_css_tokenize(String_p);

    nh_core_List TokenList = nh_core_initList(Tokens.length);
    for (int i = 0; i < Tokens.length; ++i) {
        nh_core_appendToList(&TokenList, &((nh_css_Token*)Tokens.p)[i]);
    }

    nh_css_TokenParser TokenParser = nh_css_initTokenParser((nh_css_Token**)TokenList.pp, TokenList.size);
    nh_ecmascript_Object *CSSStyleSheet_p = nh_css_parseStyleSheet(&TokenParser, Document_p);

    if (CSSStyleSheet_p) {
        nh_css_setStyleSheetTokens(CSSStyleSheet_p, Tokens);
        nh_css_logTokens(CSSStyleSheet_p, &Tokens);
    }

    nh_core_freeList(&TokenList, false);

    return CSSStyleSheet_p;
}

nh_css_ComponentValueData nh_css_parseComponentValuesFromUTF8Codepoints(
    char *logId_p, char *p, unsigned long long length)
{
    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(p, length, NULL);

    nh_core_Array Tokens = nh_css_tokenize(&String);

    nh_core_List TokenList = nh_core_initList(Tokens.length);
    for (int i = 0; i < Tokens.length; ++i) {
        nh_core_appendToList(&TokenList, &((nh_css_Token*)Tokens.p)[i]);
    }

    nh_css_TokenParser Parser = nh_css_initTokenParser((nh_css_Token**)TokenList.pp, TokenList.size);
    nh_core_Array ComponentValues = nh_css_parseComponentValues(&Parser);

    nh_core_freeList(&TokenList, false);
    nh_encoding_freeUTF32(&String);

    nh_css_ComponentValueData Result;
    Result.Tokens          = Tokens;
    Result.ComponentValues = ComponentValues;

    return Result;
}

nh_css_DeclarationData nh_css_parseDeclarationsFromBytes(
    char *logId_p, char *p, unsigned long long length)
{
    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(p, length, NULL);

    nh_core_Array Tokens = nh_css_tokenize(&String);

    nh_core_List TokenList = nh_core_initList(Tokens.length);
    for (int i = 0; i < Tokens.length; ++i) {
        nh_core_appendToList(&TokenList, &((nh_css_Token*)Tokens.p)[i]);
    }

    nh_css_TokenParser Parser = nh_css_initTokenParser((nh_css_Token**)TokenList.pp, TokenList.size);
    nh_core_Array Declarations = nh_css_parseDeclarations(&Parser);

    nh_core_freeList(&TokenList, false);
    nh_encoding_freeUTF32(&String);

    nh_css_DeclarationData Result;
    Result.Tokens       = Tokens;
    Result.Declarations = Declarations;

    return Result;
}
