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
#include "../Common/Macros.h"

#include "../../nhcore/Util/Array.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/List.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DEFAULT STYLE SHEET =============================================================================

nh_css_StyleSheetObject *NH_CSS_DEFAULT_STYLE_SHEET_P = NULL;
	
// PARSER INTERFACE ================================================================================

nh_css_StyleSheetObject *nh_css_parseStyleSheetFromUTF32(
    nh_encoding_UTF32String *String_p, nh_css_DocumentObject *Document_p)
{
NH_CSS_BEGIN()

    nh_Array Tokens = nh_css_tokenize(String_p);

    nh_List TokenList = nh_core_initList(Tokens.length);
    for (int i = 0; i < Tokens.length; ++i) {
        nh_core_appendToList(&TokenList, &((nh_css_Token*)Tokens.p)[i]);
    }

    nh_css_TokenParser TokenParser = nh_css_initTokenParser((nh_css_Token**)TokenList.pp, TokenList.size);
    nh_css_StyleSheetObject *StyleSheet_p = nh_css_parseStyleSheet(&TokenParser, Document_p);

    if (StyleSheet_p) {
        nh_css_setStyleSheetTokens(StyleSheet_p, Tokens);
//        nh_css_logTokens(logId_p, &Tokens);
    }

    nh_core_freeList(&TokenList, NH_FALSE);

NH_CSS_END(StyleSheet_p)
}

nh_css_ComponentValueData nh_css_parseComponentValuesFromUTF8Codepoints(
    NH_BYTE *logId_p, NH_BYTE *p, unsigned long long length)
{
NH_CSS_BEGIN()

    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(p, length, NULL);

    nh_Array Tokens = nh_css_tokenize(&String);

    nh_List TokenList = nh_core_initList(Tokens.length);
    for (int i = 0; i < Tokens.length; ++i) {
        nh_core_appendToList(&TokenList, &((nh_css_Token*)Tokens.p)[i]);
    }

    nh_css_TokenParser Parser = nh_css_initTokenParser((nh_css_Token**)TokenList.pp, TokenList.size);
    nh_Array ComponentValues = nh_css_parseComponentValues(&Parser);

    nh_core_freeList(&TokenList, NH_FALSE);
    nh_encoding_freeUTF32(&String);

    nh_css_ComponentValueData Result;
    Result.Tokens          = Tokens;
    Result.ComponentValues = ComponentValues;

NH_CSS_END(Result)
}

nh_css_DeclarationData nh_css_parseDeclarationsFromBytes(
    NH_BYTE *logId_p, NH_BYTE *p, unsigned long long length)
{
NH_CSS_BEGIN()

    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(p, length, NULL);

    nh_Array Tokens = nh_css_tokenize(&String);

    nh_List TokenList = nh_core_initList(Tokens.length);
    for (int i = 0; i < Tokens.length; ++i) {
        nh_core_appendToList(&TokenList, &((nh_css_Token*)Tokens.p)[i]);
    }

    nh_css_TokenParser Parser = nh_css_initTokenParser((nh_css_Token**)TokenList.pp, TokenList.size);
    nh_Array Declarations = nh_css_parseDeclarations(&Parser);

    nh_core_freeList(&TokenList, NH_FALSE);
    nh_encoding_freeUTF32(&String);

    nh_css_DeclarationData Result;
    Result.Tokens       = Tokens;
    Result.Declarations = Declarations;

NH_CSS_END(Result)
}

