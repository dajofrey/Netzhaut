#ifndef NH_CSS_PARSER_H
#define NH_CSS_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "SelectorParser.h"
#include "TokenParser.h"
#include "Tokenizer.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_vars
 *  @{
 */

    extern nh_css_StyleSheetObject *NH_CSS_DEFAULT_STYLE_SHEET_P;

/** @} */

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_ComponentValueData {
        nh_Array Tokens;
        nh_Array ComponentValues;
    } nh_css_ComponentValueData;

    typedef struct nh_css_DeclarationData {
        nh_Array Tokens;
        nh_Array Declarations; 
    } nh_css_DeclarationData;

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_StyleSheetObject *nh_css_parseStyleSheetFromUTF32(
        nh_encoding_UTF32String *String_p, nh_css_DocumentObject *Document_p
    );

    nh_css_ComponentValueData nh_css_parseComponentValuesFromUTF8Codepoints(
        char *logId_p, char *p, unsigned long long length
    );

    nh_css_DeclarationData nh_css_parseDeclarationsFromBytes(
        char *logId_p, char *p, unsigned long long length
    );

/** @} */

#endif
