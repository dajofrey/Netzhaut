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
    nh_webidl_Object *CharacterData_p)
{
    CharacterData_p->Attributes.pp[DATA] = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_CORE_CHECK_MEM(CharacterData_p->Attributes.pp[DATA])

    *((nh_webidl_DOMString*)CharacterData_p->Attributes.pp[DATA]) = nh_webidl_initDOMString(64);
    CharacterData_p->Attributes.pp[LENGTH] = &((nh_webidl_DOMString*)CharacterData_p->Attributes.pp[DATA])->length;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_appendToCharacterData(
    nh_webidl_Object *CharacterData_p, nh_webidl_DOMString DOMString)
{
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_webidl_appendToDOMString(
        CharacterData_p->Attributes.pp[DATA], DOMString.p, DOMString.length
    ))

    return NH_API_SUCCESS;
}

nh_webidl_DOMString *nh_dom_getCharacterDataString(
    nh_webidl_Object *CharacterData_p)
{
    return CharacterData_p->Attributes.pp[DATA];
}
