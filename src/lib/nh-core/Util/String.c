// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "String.h"
#include "LinkedList.h"

#include "../System/Memory.h"
#include "../Common/Macros.h"

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

// DATA STORAGE UNITS ==============================================================================

nh_DataStorageUnits nh_humanizeBytes(
    long long bytes)
{
NH_CORE_BEGIN()

    nh_DataStorageUnits Units;
    Units.gigabytes = bytes / (1024*1024*1024);
    Units.megabytes = (bytes / (1024*1024)) % 1024; 
    Units.kilobytes = (bytes / 1024) % 1024;
    Units.bytes     = bytes % 1024;
   
NH_CORE_END(Units)
}

// BYTES ===========================================================================================

char *nh_core_allocateBytes(
    char *bytes_p)
{
    if (bytes_p == NULL || strlen(bytes_p) == 0) {return NULL;}
    char *allocatedBytes_p = nh_core_allocate(sizeof(char) * (strlen(bytes_p) + 1));
    if (allocatedBytes_p == NULL) {return NULL;}
    strcpy(allocatedBytes_p, bytes_p);
    return allocatedBytes_p;
}

// LEVENSHTEIN =====================================================================================

int nh_levenshteinDistance(
    char *s1_p, char *s2_p, bool caseSensitive) 
{
NH_CORE_BEGIN()

    if (strlen(s1_p) > 255 || strlen(s2_p) > 255) {NH_CORE_END(-1)}

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

NH_CORE_END(column[s1len])
}

// STRING ===========================================================================================

nh_String nh_core_initString(
    int chunkSize)
{
NH_CORE_BEGIN()
NH_CORE_END(nh_core_initArray(sizeof(char), chunkSize))
}

NH_API_RESULT nh_core_appendFormatToString(
    nh_String *String_p, char *format_p, ...)
{
NH_CORE_BEGIN()

    if (format_p == NULL) {NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)}

    va_list args;
    va_start(args, format_p);

    int byteCount = vsnprintf(NULL, 0, format_p, args);
    char *append_p = nh_core_allocate(sizeof(char) * (byteCount + 1));
    NH_CORE_CHECK_MEM(append_p)

    va_end(args);

    va_start(args, format_p);
    vsprintf(append_p, format_p, args);
    va_end(args);

    NH_CORE_CHECK(nh_core_appendToArray(String_p, append_p, byteCount * sizeof(char)))

    nh_core_free(append_p);

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_core_appendByteToString(
    nh_String *String_p, char byte)
{
NH_CORE_BEGIN()
NH_CORE_DIAGNOSTIC_END(nh_core_appendToArray(String_p, &byte, 1))
}

NH_API_RESULT nh_core_appendByteToStringRepeatedly(
    nh_String *String_p, char byte, int count)
{
NH_CORE_BEGIN()
NH_CORE_DIAGNOSTIC_END(nh_core_appendToArrayRepeatedly(String_p, &byte, count))
}

NH_API_RESULT nh_core_appendToString(
    nh_String *String_p, char *bytes_p, int length)
{
NH_CORE_BEGIN()
NH_CORE_DIAGNOSTIC_END(nh_core_appendToArray(String_p, bytes_p, length))
}

NH_API_RESULT nh_core_removeFromString(
    nh_String *String_p, int index, unsigned int count)
{
NH_CORE_BEGIN()
NH_CORE_DIAGNOSTIC_END(nh_core_removeFromString(String_p, index, count))
}

NH_API_RESULT nh_core_removeTailFromString(
    nh_String *String_p, unsigned int count)
{
NH_CORE_BEGIN()
NH_CORE_DIAGNOSTIC_END(nh_core_removeTailFromArray(String_p, count))
}

void nh_core_freeString(
    nh_String *String_p)
{
    nh_core_freeArray(String_p);
}

