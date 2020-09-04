// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/String.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

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

// DECLARE =========================================================================================

struct Nh_String {
    char *data_p;
};

// STRING ==========================================================================================

Nh_String *Nh_allocateString(
    char *init_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_String *String_p = Nh_allocate(sizeof(Nh_String));
    NH_CHECK_NULL(NULL, String_p)

    if (init_p == NULL || strlen(init_p) == 0) {String_p->data_p = NULL;}
    else 
    {
        String_p->data_p = Nh_allocate(sizeof(char) * (strlen(init_p) + 1));
        NH_CHECK_NULL(NULL, String_p->data_p)
        strcpy(String_p->data_p, init_p);
    }

#include NH_DEFAULT_CHECK

NH_END(String_p)
}

NH_RESULT Nh_appendToString(
    Nh_String *String_p, char *append_p)
{
NH_BEGIN()

    NH_CHECK_NULL(String_p, append_p)
    if (strlen(append_p) <= 0) {return 1;}

    char *part_p = Nh_allocate(sizeof(char) * (strlen(append_p) + 1));
    NH_CHECK_MEM(part_p)

    strcpy(part_p, append_p);

    if (String_p->data_p == NULL) {String_p->data_p = part_p;}
    else 
    {
        char *tmp_p = Nh_allocate(sizeof(char) * (strlen(String_p->data_p) + strlen(part_p) + 1));
        NH_CHECK_MEM(tmp_p)

        for (int i = 0; i < strlen(String_p->data_p); ++i) {tmp_p[i] = String_p->data_p[i];}
        for (int i = 0; i < (strlen(part_p) + 1); ++i)     {tmp_p[i + strlen(String_p->data_p)] = part_p[i];}

        Nh_free(String_p->data_p);      
        Nh_free(part_p);

        String_p->data_p = tmp_p;
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_appendCharToString(
    Nh_String *String_p, char append)
{
NH_BEGIN()

    NH_CHECK_NULL(String_p)

    char *part_p = Nh_allocate(sizeof(char) * 2);
    NH_CHECK_MEM(part_p)

    part_p[0] = append;
    part_p[1] = '\0';

    if (String_p->data_p == NULL) {String_p->data_p = part_p;}
    else 
    {
        char *tmp_p = Nh_allocate(sizeof(char) * (strlen(String_p->data_p) + strlen(part_p) + 1));
        NH_CHECK_MEM(tmp_p)

        for (int i = 0; i < strlen(String_p->data_p); ++i) {tmp_p[i] = String_p->data_p[i];}
        for (int i = 0; i < (strlen(part_p) + 1); ++i)     {tmp_p[i + strlen(String_p->data_p)] = part_p[i];}

        Nh_free(String_p->data_p);      
        Nh_free(part_p);

        String_p->data_p = tmp_p;
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_appendFormatToString(
    Nh_String *String_p, char *format_p, ...)
{
NH_BEGIN()

    va_list args;
    va_start(args, format_p);

    int charCount = vsnprintf(NULL, 0, format_p, args);

    char *append_p = Nh_allocate(sizeof(char) * (charCount + 1));
    NH_CHECK_MEM(append_p)

    va_end(args);

    va_start(args, format_p);
    vsprintf(append_p, format_p, args);
    va_end(args);

    NH_CHECK(Nh_appendToString(String_p, append_p))
    Nh_free(append_p);

NH_END(NH_SUCCESS)
}

void Nh_freeString(
    Nh_String *String_p, bool freeData)
{
NH_BEGIN()

    if (freeData) {Nh_free(String_p->data_p);}
    Nh_free(String_p);

NH_SILENT_END()
}

void Nh_clearString(
    Nh_String *String_p)
{
NH_BEGIN()

    Nh_free(String_p->data_p);
    String_p->data_p = NULL;

NH_SILENT_END()
}

char *Nh_getChars(
    Nh_String *String_p)
{
NH_BEGIN()
NH_END(String_p->data_p)
}

NH_RESULT Nh_setChars(
    Nh_String *String_p, char c, int pos, int count)
{
NH_BEGIN()

    NH_CHECK_NULL(String_p)

    if (pos == -1) {
        for (int i = strlen(String_p->data_p) - 1, j = count; i >= 0 && j > 0; --i, --j) {
            String_p->data_p[i] = c;
        }
    }

NH_END(NH_SUCCESS)
}

char *Nh_allocateChars(
    char *chars_p)
{
NH_BEGIN()

    char *allocatedChars_p = Nh_allocate(sizeof(char) * (strlen(chars_p) + 1));
    if (allocatedChars_p == NULL) {NH_END(NULL)}
    strcpy(allocatedChars_p, chars_p);

NH_END(allocatedChars_p)
}

int Nh_levenshteinDistance(
    char *s1_p, char *s2_p, NH_BOOL caseSensitive) 
{
NH_BEGIN()

    if (strlen(s1_p) > 255 || strlen(s2_p) > 255) {NH_END(-1)}

    unsigned int s1len, s2len, x, y, lastdiag, olddiag;

    char s1cpy_p[256] = {'\0'}, s2cpy_p[256] = {'\0'};
    strcpy(s1cpy_p, s1_p);
    strcpy(s2cpy_p, s2_p);

    if (!caseSensitive)
    {
        for (int i = 0; i < strlen(s1_p); ++i) {
            s1cpy_p[i] = tolower(s1_p[i]);
        }
        for (int i = 0; i < strlen(s2_p); ++i) {
            s2cpy_p[i] = tolower(s2_p[i]);
        }
    }

    s1len = strlen(s1_p);
    s2len = strlen(s2_p);

    unsigned int column[s1len+1];

    for (y = 1; y <= s1len; y++) {
        column[y] = y;
    }

    for (x = 1; x <= s2len; x++) 
    {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
#define                 MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1cpy_p[y-1] == s2cpy_p[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }

NH_END(column[s1len])
}

