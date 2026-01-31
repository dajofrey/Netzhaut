// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "CharacterData.h"

#include "../../nh-webidl/Runtime/String.h"
#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define DATA 0
#define LENGTH 1 

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_dom_initializeCharacterData(
    nh_ecmascript_Object *CharacterData_p)
{
    CharacterData_p->Attributes.pp[DATA] = nh_core_allocate(sizeof(nh_encoding_UTF8String));
    NH_CORE_CHECK_MEM(CharacterData_p->Attributes.pp[DATA])

    *((nh_encoding_UTF8String*)CharacterData_p->Attributes.pp[DATA]) = nh_encoding_initUTF8(64);
    CharacterData_p->Attributes.pp[LENGTH] = &((nh_encoding_UTF8String*)CharacterData_p->Attributes.pp[DATA])->length;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_appendToCharacterData(
    nh_ecmascript_Object *CharacterData_p, nh_encoding_UTF8String DOMString)
{
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_core_appendToString(
        CharacterData_p->Attributes.pp[DATA], DOMString.p, DOMString.length
    ))

    return NH_API_SUCCESS;
}

nh_encoding_UTF8String *nh_dom_getCharacterDataString(
    nh_ecmascript_Object *CharacterData_p)
{
    return CharacterData_p->Attributes.pp[DATA];
}
