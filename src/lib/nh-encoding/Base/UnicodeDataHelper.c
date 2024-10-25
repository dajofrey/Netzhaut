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
#include "UnicodeData.gen.h"

#include "../Common/Log.h"
#include "../../nh-core/System/Memory.h"

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

// FUNCTIONS =======================================================================================

static int nh_encoding_getUnicodeDataIndex(
    NH_ENCODING_UTF32 codepoint)
{
    for (int i = 0; i < sizeof(NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP) / sizeof(NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[0]); ++i) {
        if (codepoint > NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][0] && codepoint < NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][1]) {
            return -1;
        }
    }

    for (int i = 0; i < sizeof(NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP) / sizeof(NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[0]); ++i) {
        if (codepoint > NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][0]) {
            codepoint -= NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][1] - NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[i][0] - 1;
        }
        else {break;}
    }

    return codepoint;
}

const char *nh_encoding_getDescription(
    NH_ENCODING_UTF32 codepoint)
{
    codepoint = nh_encoding_getUnicodeDataIndex(codepoint);
    if (codepoint == -1) {return NULL;}

    return NH_ENCODING_UNICODE_DATA_PP[codepoint];
}

static bool nh_encoding_inCategory(
    const char *codepointDescription_p, const char *categoryAbbreviation_p)
{
    char copy_p[512] = {'\0'};
    strcpy(copy_p, codepointDescription_p);

    char *p = copy_p;
    for (int i = 0; i < strlen(copy_p); ++i) {
        if (copy_p[i] == ';') {
            copy_p[i] = '\0';
            if (!strcmp(p, categoryAbbreviation_p)) {
                return true;
            }
            copy_p[i] = ';';
            p = &copy_p[i + 1];
        }
    }

    return false;
}

// https://www.unicode.org/reports/tr41/tr41-26.html#UAX31
bool nh_encoding_inIDSTART(
    NH_ENCODING_UTF32 codepoint)
{
    const char *description_p = nh_encoding_getDescription(codepoint);
    if (description_p == NULL) {return false;}

    if (nh_encoding_inCategory(description_p, "L")
    ||  nh_encoding_inCategory(description_p, "Nl")) {
        return true;
    }

    return false;
}

// https://www.unicode.org/reports/tr41/tr41-26.html#UAX31
bool nh_encoding_inIDCONTINUE(
    NH_ENCODING_UTF32 codepoint)
{
    if (nh_encoding_inIDSTART(codepoint)) {
        return true;
    }

    const char *description_p = nh_encoding_getDescription(codepoint);
    if (description_p == NULL) {return false;}

    if (nh_encoding_inCategory(description_p, "Mn")
    ||  nh_encoding_inCategory(description_p, "Mc")
    ||  nh_encoding_inCategory(description_p, "Nd")
    ||  nh_encoding_inCategory(description_p, "Pc")) {
        return true;
    }

    return false;
}

static char *nh_encoding_getUnicodeDataProperty(
    const char *codepointDescription_p, NH_ENCODING_UNICODE_DATA_PROPERTY property)
{
    char copy_p[512] = {'\0'};
    strcpy(copy_p, codepointDescription_p);

    char *p = copy_p;
    int counter = 0;
    for (int i = 0; i < strlen(copy_p); ++i) {
        if (copy_p[i] == ';') {
            if (counter == property) {
                copy_p[i] = '\0';
                char *allocated_p = nh_core_allocate(sizeof(char)*(strlen(p)+1));
                strcpy(allocated_p, p);
                return allocated_p;
            }
            p = &copy_p[i + 1];
            counter++;
        }
    }

    return NULL;
}

NH_ENCODING_UTF32 nh_encoding_getUnicodeDataSimpleLowerCase(
    NH_ENCODING_UTF32 codepoint)
{
    const char *description_p = nh_encoding_getDescription(codepoint);
    if (description_p == NULL) {return 0;}

    char *property_p = nh_encoding_getUnicodeDataProperty(
        description_p, NH_ENCODING_UNICODE_DATA_PROPERTY_SIMPLE_LOWERCASE_MAPPING);
    if (property_p == NULL) {return 0;}

    codepoint = strtol(property_p, NULL, 16);

    nh_core_free(property_p);

    return codepoint;
}

NH_ENCODING_UTF32 nh_encoding_getUnicodeDataSimpleUpperCase(
    NH_ENCODING_UTF32 codepoint)
{
    const char *description_p = nh_encoding_getDescription(codepoint);
    if (description_p == NULL) {return 0;}

    char *property_p = nh_encoding_getUnicodeDataProperty(
        description_p, NH_ENCODING_UNICODE_DATA_PROPERTY_SIMPLE_UPPERCASE_MAPPING);
    if (property_p == NULL) {return 0;}

    codepoint = strtol(property_p, NULL, 16);

    nh_core_free(property_p);

    return codepoint;
}

