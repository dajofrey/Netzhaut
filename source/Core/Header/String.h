#ifndef NH_STRING_H
#define NH_STRING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"

#include <stdbool.h>

#endif

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_String Nh_String;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    Nh_String *Nh_allocateString(
        char *init_p
    );
    
    NH_RESULT Nh_appendToString(
        Nh_String *String_p, char *append_p
    );

    NH_RESULT Nh_appendCharToString(
        Nh_String *String_p, char append
    );

    NH_RESULT Nh_appendFormatToString(
        Nh_String *String_p, char *format_p, ...
    );
  
    void Nh_freeString(
        Nh_String *String_p, bool freeData
    );
    
    char *Nh_getChars(
        Nh_String *String_p
    );

    NH_RESULT Nh_setChars(
        Nh_String *String_p, char c, int pos, int count
    );

    char *Nh_allocateChars(
        char *chars_p
    );

    int Nh_levenshteinDistance(
        char *s1_p, char *s2_p, NH_BOOL caseSensitive
    );

    void Nh_clearString(
        Nh_String *String_p
    );

/** @} */

#endif 
