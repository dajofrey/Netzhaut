// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 *
 * Indices of properties: https://www.unicode.org/reports/tr44/#UnicodeData.txt
 */

// INCLUDES =======================================================================================

#include "UnicodeDataHelper.h"
#include "UnicodeData.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

// LOOKUP ==========================================================================================

static int nh_encoding_getUnicodeDataIndex(
    NH_ENCODING_UTF32 codepoint)
{
NH_ENCODING_BEGIN()

    for (int i = 0; i < sizeof(NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP) / sizeof(NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[0]); ++i) {
        if (codepoint > NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][0] && codepoint < NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][1]) {
            NH_ENCODING_END(-1)
        }
    }

    for (int i = 0; i < sizeof(NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP) / sizeof(NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[0]); ++i) {
        if (codepoint > NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][0]) {
            codepoint -= NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][1] - NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][0] - 1;
        }
        else {break;}
    }

NH_ENCODING_END(codepoint)
}

const NH_BYTE *nh_encoding_getDescription(
    NH_ENCODING_UTF32 codepoint)
{
NH_ENCODING_BEGIN()

    codepoint = nh_encoding_getUnicodeDataIndex(codepoint);
    if (codepoint == -1) {NH_ENCODING_END(NULL)}

NH_ENCODING_END(NH_ENCODING_UNICODE_DATA_PP[codepoint])
}

static NH_BOOL nh_encoding_inCategory(
    const NH_BYTE *codepointDescription_p, const NH_BYTE *categoryAbbreviation_p)
{
NH_ENCODING_BEGIN()

    NH_BYTE copy_p[512] = {'\0'};
    strcpy(copy_p, codepointDescription_p);

    NH_BYTE *p = copy_p;
    for (int i = 0; i < strlen(copy_p); ++i) {
        if (copy_p[i] == ';') {
            copy_p[i] = '\0';
            if (!strcmp(p, categoryAbbreviation_p)) {
                NH_ENCODING_END(NH_TRUE)
            }
            copy_p[i] = ';';
            p = &copy_p[i + 1];
        }
    }

NH_ENCODING_END(NH_FALSE)
}

// https://www.unicode.org/reports/tr41/tr41-26.html#UAX31
NH_BOOL nh_encoding_inIDSTART(
    NH_ENCODING_UTF32 codepoint)
{
NH_ENCODING_BEGIN()

    const NH_BYTE *description_p = nh_encoding_getDescription(codepoint);
    if (description_p == NULL) {NH_ENCODING_END(NH_FALSE)}

    if (nh_encoding_inCategory(description_p, "L")
    ||  nh_encoding_inCategory(description_p, "Nl")) {
        NH_ENCODING_END(NH_TRUE)
    }

NH_ENCODING_END(NH_FALSE)
}

// https://www.unicode.org/reports/tr41/tr41-26.html#UAX31
NH_BOOL nh_encoding_inIDCONTINUE(
    NH_ENCODING_UTF32 codepoint)
{
NH_ENCODING_BEGIN()

    if (nh_encoding_inIDSTART(codepoint)) {
        NH_ENCODING_END(NH_TRUE)
    }

    const NH_BYTE *description_p = nh_encoding_getDescription(codepoint);
    if (description_p == NULL) {NH_ENCODING_END(NH_FALSE)}

    if (nh_encoding_inCategory(description_p, "Mn")
    ||  nh_encoding_inCategory(description_p, "Mc")
    ||  nh_encoding_inCategory(description_p, "Nd")
    ||  nh_encoding_inCategory(description_p, "Pc")) {
        NH_ENCODING_END(NH_TRUE)
    }

NH_ENCODING_END(NH_FALSE)
}

static NH_BYTE *nh_encoding_getUnicodeDataProperty(
    const NH_BYTE *codepointDescription_p, NH_ENCODING_UNICODE_DATA_PROPERTY property)
{
NH_ENCODING_BEGIN()

    NH_BYTE copy_p[512] = {'\0'};
    strcpy(copy_p, codepointDescription_p);

    NH_BYTE *p = copy_p;
    int counter = 0;
    for (int i = 0; i < strlen(copy_p); ++i) {
        if (copy_p[i] == ';') {
            if (counter == property) {
                copy_p[i] = '\0';
                NH_BYTE *allocated_p = nh_core_allocate(sizeof(NH_BYTE)*(strlen(p)+1));
                strcpy(allocated_p, p);
                NH_ENCODING_END(allocated_p)
            }
            p = &copy_p[i + 1];
            counter++;
        }
    }

NH_ENCODING_END(NULL)
}

NH_ENCODING_UTF32 nh_encoding_getUnicodeDataSimpleLowerCase(
    NH_ENCODING_UTF32 codepoint)
{
NH_ENCODING_BEGIN()

    const NH_BYTE *description_p = nh_encoding_getDescription(codepoint);
    if (description_p == NULL) {NH_ENCODING_END(0)}

    NH_BYTE *property_p = nh_encoding_getUnicodeDataProperty(
        description_p, NH_ENCODING_UNICODE_DATA_PROPERTY_SIMPLE_LOWERCASE_MAPPING);
    if (property_p == NULL) {NH_ENCODING_END(0)}

    codepoint = strtol(property_p, NULL, 16);

    nh_core_free(property_p);

NH_ENCODING_END(codepoint)
}

NH_ENCODING_UTF32 nh_encoding_getUnicodeDataSimpleUpperCase(
    NH_ENCODING_UTF32 codepoint)
{
NH_ENCODING_BEGIN()

    const NH_BYTE *description_p = nh_encoding_getDescription(codepoint);
    if (description_p == NULL) {NH_ENCODING_END(0)}

    NH_BYTE *property_p = nh_encoding_getUnicodeDataProperty(
        description_p, NH_ENCODING_UNICODE_DATA_PROPERTY_SIMPLE_UPPERCASE_MAPPING);
    if (property_p == NULL) {NH_ENCODING_END(0)}

    codepoint = strtol(property_p, NULL, 16);

    nh_core_free(property_p);

NH_ENCODING_END(codepoint)
}

