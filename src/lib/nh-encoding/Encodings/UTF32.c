// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "UTF32.h"
#include "UTF8.h"

#include "../Common/Log.h"

#include "../../nh-core/Util/String.h"
#include "../../nh-core/Util/List.h"
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

// UNICODE CODEPOINT STRING ========================================================================

nh_encoding_UTF32String nh_encoding_initUTF32(
    int chunkSize)
{
    nh_encoding_UTF32String String;
    String.Array  = nh_core_initArray(sizeof(NH_ENCODING_UTF32), chunkSize);
    String.p      = (NH_ENCODING_UTF32*)String.Array.p; 
    String.length = 0;
    return String;
}

NH_API_RESULT nh_encoding_appendUTF32(
    nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 *codepoints_p, unsigned long length)
{
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_core_appendToArray(&String_p->Array, codepoints_p, length))
    String_p->p = (NH_ENCODING_UTF32*)String_p->Array.p; 
    String_p->length = String_p->Array.length;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_encoding_appendUTF32Codepoint(
    nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 codepoint)
{
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_core_appendToArray(&String_p->Array, &codepoint, 1))
    String_p->p = (NH_ENCODING_UTF32*)String_p->Array.p; 
    String_p->length = String_p->Array.length;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_encoding_appendUTF8ToUTF32(
    nh_encoding_UTF32String *String_p, unsigned char *bytes_p, unsigned long length)
{
    nh_encoding_UTF32String Append = nh_encoding_decodeUTF8(bytes_p, length, NULL);
    NH_CORE_CHECK(nh_encoding_appendUTF32(String_p, Append.p, Append.length))
    nh_encoding_freeUTF32(&Append);
    return NH_API_SUCCESS;
}

NH_ENCODING_UTF32 *nh_encoding_incrementUTF32(
    nh_encoding_UTF32String *String_p)
{
    NH_ENCODING_UTF32 *codepoint_p = nh_core_incrementArray(&String_p->Array);
    String_p->p = (NH_ENCODING_UTF32*)String_p->Array.p; 
    String_p->length = String_p->Array.length;
    return codepoint_p;
}

void nh_encoding_freeUTF32(
    nh_encoding_UTF32String *String_p)
{
    nh_core_freeArray(&String_p->Array);
    String_p->p = NULL;
    String_p->length = 0;
    return;
}

unsigned int nh_encoding_getUTF32Length(
    NH_ENCODING_UTF32 *codepoints_p)
{
    unsigned int length = 0;
    for (; codepoints_p[length] != 0; ++length);
    return length;
}

NH_API_RESULT nh_encoding_removeUTF32Tail(
    nh_encoding_UTF32String *String_p, unsigned long count)
{
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_core_removeTailFromArray(&String_p->Array, count))
    String_p->p = (NH_ENCODING_UTF32*)String_p->Array.p;
    String_p->length = String_p->Array.length;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_encoding_removeUTF32(
    nh_encoding_UTF32String *String_p, int index, unsigned int count)
{
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_core_removeFromArray(&String_p->Array, index, count))
    String_p->p = (NH_ENCODING_UTF32*)String_p->Array.p;
    String_p->length = String_p->Array.length;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_encoding_insertUTF32(
    nh_encoding_UTF32String *String_p, int index, NH_ENCODING_UTF32 *codepoints_p, int length)
{
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_core_insertIntoArray(&String_p->Array, index, codepoints_p, length))
    String_p->p = (NH_ENCODING_UTF32*)String_p->Array.p;
    String_p->length = String_p->Array.length;
    return NH_API_SUCCESS;
}

nh_core_List nh_encoding_splitUTF32(
    nh_encoding_UTF32String *String_p, NH_ENCODING_UTF32 delimiter)
{
    int i = 0;
    nh_core_List Tokens = nh_core_initList(8);
   
    while (i < String_p->length)
    {
        if (i > 0) {++i;}

        nh_encoding_UTF32String Token = nh_encoding_initUTF32(16);
        while (i < String_p->length && String_p->p[i] != delimiter) {
            NH_CORE_CHECK_2(Tokens, nh_encoding_appendUTF32Codepoint(&Token, String_p->p[i++]))
        }
    
        nh_encoding_UTF32String *Token_p = nh_core_allocate(sizeof(nh_encoding_UTF32String));
        NH_CORE_CHECK_MEM_2(Tokens, Token_p)
    
        *Token_p = Token;
        nh_core_appendToList(&Tokens, Token_p);
    }

    return Tokens;
}

void nh_encoding_replaceNonCharacters(
    nh_encoding_UTF32String *String_p)
{
    for (int i = 0; i < String_p->length; ++i) {
        if (nh_encoding_isNonCharacter(String_p->p[i]) || nh_encoding_isControl(String_p->p[i])) {
            String_p->p[i] = 0xFFD;
        }
    }
    return;
}

nh_encoding_UTF32String nh_encoding_replaceNonCharactersExpressively(
    nh_encoding_UTF32String *String_p)
{
    nh_encoding_UTF32String String = nh_encoding_initUTF32(String_p->length);

    static uint32_t CR_p[] = {94, 67, 82};
    static uint32_t LF_p[] = {94, 76, 70};
    static uint32_t HT_p[] = {94, 72, 84};
    static uint32_t DT_p[] = {94, 68, 84};

    for (int i = 0; i < String_p->length; ++i) {
        if (nh_encoding_isNonCharacter(String_p->p[i]) || nh_encoding_isControl(String_p->p[i])) {
            switch (String_p->p[i])
            {
                case 0x0D : nh_encoding_appendUTF32(&String, CR_p, 3); break;
                case 0x0A : nh_encoding_appendUTF32(&String, LF_p, 3); break;
                case 0x09 : nh_encoding_appendUTF32(&String, HT_p, 3); break;
                case 0x7F : nh_encoding_appendUTF32(&String, DT_p, 3); break;
            }
        }
        else {nh_encoding_appendUTF32(&String, &String_p->p[i], 1);}
    }

    return String;
}

void nh_encoding_printUTF32(
    nh_encoding_UTF32String *String_p) 
{
    nh_encoding_UTF8String String = nh_encoding_encodeUTF8(String_p->p, String_p->length);
    puts(String.p);
    nh_encoding_freeUTF8(&String);
}

// CHECKS ==========================================================================================

bool nh_encoding_isASCIIWhitespace(
    NH_ENCODING_UTF32 codepoint)
{
    return codepoint == 0x09 || codepoint == 0x0A || codepoint == 0x0C || codepoint == 0x0D || codepoint == 0x20;
}

bool nh_encoding_isASCIIUpperAlpha(
    NH_ENCODING_UTF32 codepoint)
{
    return codepoint >= 0x41 && codepoint <= 0x5A;
}

bool nh_encoding_isASCIILowerAlpha(
    NH_ENCODING_UTF32 codepoint)
{
    return codepoint >= 0x61 && codepoint <= 0x7A;
}

bool nh_encoding_isASCIIAlpha(
    NH_ENCODING_UTF32 codepoint)
{
    return nh_encoding_isASCIIUpperAlpha(codepoint) || nh_encoding_isASCIILowerAlpha(codepoint);
}

bool nh_encoding_isASCIIDigit(
    NH_ENCODING_UTF32 codepoint)
{
    return codepoint >= 0x30 && codepoint <= 0x39;
}

bool nh_encoding_isASCIIDigit2(
    NH_ENCODING_UTF32 codepoint)
{
    return codepoint == 0 || codepoint == 48 || codepoint == 49 || (codepoint >= 27 && codepoint <= 31) || codepoint == 127 || codepoint == 57;
}

bool nh_encoding_isASCIIHexDigit(
    NH_ENCODING_UTF32 codepoint)
{
    return nh_encoding_isASCIIDigit(codepoint) || (codepoint >= 0x41 && codepoint <= 0x46) || (codepoint >= 0x61 && codepoint <= 0x66);
}

bool nh_encoding_isASCIIAlphaNumeric(
    NH_ENCODING_UTF32 codepoint)
{
    return nh_encoding_isASCIIAlpha(codepoint) || nh_encoding_isASCIIDigit(codepoint);
}

bool nh_encoding_isASCIICaseInsensitiveMatch(
    char *str1_p, char *str2_p)
{
    if (strlen(str1_p) != strlen(str2_p)) {return false;}

    for (int i = 0; i < strlen(str1_p); ++i) {
        int d = tolower((unsigned char)str1_p[i]) - tolower((unsigned char)str2_p[i]);
        if (d != 0) {
            return false;
        }
    }
   
    return true;
}

bool nh_encoding_isASCIICodepoint(
    NH_ENCODING_UTF32 codepoint)
{
    return codepoint >= 0x00 && codepoint <= 0x7F;
}

bool nh_encoding_isSurrogate(
    NH_ENCODING_UTF32 codepoint)
{
    return codepoint >= 0xD800 && codepoint <= 0xDFFF;
}

bool nh_encoding_isScalarValue(
    NH_ENCODING_UTF32 codepoint)
{
    return !nh_encoding_isSurrogate(codepoint);
}

bool nh_encoding_isNonCharacter(
    NH_ENCODING_UTF32 codepoint)
{
    return (codepoint >= 0xFDD0 && codepoint <= 0xFDEF) || codepoint == 0xFFFE || codepoint == 0xFFFF || codepoint == 0x1FFFE || codepoint == 0x1FFFF || codepoint == 0x2FFFE || codepoint == 0x2FFFF || codepoint == 0x3FFFE || codepoint == 0x3FFFF || codepoint == 0x4FFFE || codepoint == 0x4FFFF || codepoint == 0x5FFFE || codepoint == 0x5FFFF || codepoint == 0x6FFFE || codepoint == 0x6FFFF || codepoint == 0x7FFFE || codepoint == 0x7FFFF || codepoint == 0x8FFFE || codepoint == 0x8FFFF || codepoint == 0x9FFFE || codepoint == 0x9FFFF || codepoint == 0xAFFFE || codepoint == 0xAFFFF || codepoint == 0xBFFFE || codepoint == 0xBFFFF || codepoint == 0xCFFFE || codepoint == 0xCFFFF || codepoint == 0xDFFFE || codepoint == 0xDFFFF || codepoint == 0xEFFFE || codepoint == 0xEFFFF || codepoint == 0xFFFFE || codepoint == 0xFFFFF || codepoint == 0x10FFFE || codepoint == 0x10FFFF;
}

bool nh_encoding_isC0Control(
    NH_ENCODING_UTF32 codepoint)
{
    return codepoint >= 0x0000 && codepoint <= 0x001F;
}

bool nh_encoding_isControl(
    NH_ENCODING_UTF32 codepoint)
{
    return nh_encoding_isC0Control(codepoint) || (codepoint >= 0x007F && codepoint <= 0x009F);
}

// MATCH ===========================================================================================

static nh_core_List nh_encoding_matchCodepoint(
    nh_core_List *Strings_p, int index, NH_ENCODING_UTF32 codepoint)
{
    nh_core_List Matches = nh_core_initList(16);

    for (int i = 0; i < Strings_p->size; ++i) {
        nh_encoding_UTF32String *String_p = Strings_p->pp[i];
        if (index < String_p->length) {
            if (String_p->p[index] == codepoint) {
                nh_core_appendToList(&Matches, String_p); 
            }
        }
    } 

    return Matches;
}

nh_encoding_UTF32String *nh_encoding_getBestMatch(
    nh_core_List *Strings_p, nh_encoding_UTF32String *String_p)
{
    nh_core_List Matches = nh_encoding_matchCodepoint(Strings_p, 0, String_p->p[0]);

    if (Matches.size == 0) {
        return NULL;
    } 
    if (Matches.size == 1) {
        nh_encoding_UTF32String *Match_p = Matches.pp[0];
        nh_core_freeList(&Matches, false);
        return Match_p;
    }

    for (int index = 1; index < String_p->length; ++index)
    {
        nh_core_List NewMatches = nh_encoding_matchCodepoint(&Matches, index, String_p->p[index]);
        if (Matches.size <= 1) {break;}
        else {
            nh_core_freeList(&Matches, false);
            Matches = NewMatches;
        }
    }

    if (Matches.size == 1) {
        nh_encoding_UTF32String *Match_p = Matches.pp[0];
        nh_core_freeList(&Matches, false);
        return Match_p;
    }

    return NULL;
}

// COMPARE =========================================================================================

bool nh_encoding_compareUTF32(
    NH_ENCODING_UTF32 *p1, NH_ENCODING_UTF32 *p2)
{
    if (!p1 && !p2) {return true;}
    if (!p1 || !p2) {return false;}

    while (*p1 || *p2) {
        if (*p1 != *p2) {return false;}
        p1++; p2++;
    }

    return true;
}

bool nh_encoding_compareUTF32ASCII(
    NH_ENCODING_UTF32 *utf32_p, char *ascii_p)
{
    if (!utf32_p && !ascii_p) {return true;}
    if (!utf32_p || !ascii_p) {return false;}

    while (*utf32_p || *ascii_p) {
        if (*utf32_p != *ascii_p) {return false;}
        utf32_p++; ascii_p++;
    }

    return true;
}

// DIGITS ==========================================================================================

int nh_encoding_hexDigitToNumber(
    NH_ENCODING_UTF32 codepoint) 
{
    if (nh_encoding_isASCIIDigit(codepoint)) {
        return codepoint-0x30;
    }
    if (codepoint >= 0x41 && codepoint <= 0x46) {
        return (codepoint-0x41)+0xA;
    }
    if (codepoint >= 0x61 && codepoint <= 0x66) {
        return (codepoint-0x61)+0xA;
    }

    return 0;
}

long nh_encoding_digitsToNumber(
    NH_ENCODING_UTF32 *codepoints_p, int length, int base) 
{
    nh_encoding_UTF8String String = nh_encoding_encodeUTF8(codepoints_p, length);
    long number = strtol(String.p, NULL, base);
    nh_core_freeString(&String);
    return number;
}

int nh_encoding_integerToUTF32(
    int number, NH_ENCODING_UTF32 *codepoints_p, int maxLength)
{
    char num_p[255];
    sprintf(num_p, "%d", number);

    for (int i = 0; i < strlen(num_p) && i < maxLength; ++i) {
        codepoints_p[i] = num_p[i];
    }

    return strlen(num_p);
}

