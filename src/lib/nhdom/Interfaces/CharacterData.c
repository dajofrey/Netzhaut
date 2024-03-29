// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "CharacterData.h"

#include "../Common/Macros.h"

#include "../../nhwebidl/Runtime/String.h"
#include "../../nhwebidl/Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define DATA 0
#define LENGTH 1 

// INITIALIZE ======================================================================================

NH_DOM_RESULT nh_dom_initializeCharacterData(
    nh_webidl_Object *CharacterData_p)
{
NH_DOM_BEGIN()

    CharacterData_p->Attributes.pp[DATA] = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_DOM_CHECK_MEM(CharacterData_p->Attributes.pp[DATA])

    *((nh_webidl_DOMString*)CharacterData_p->Attributes.pp[DATA]) = nh_webidl_initDOMString(64);
    CharacterData_p->Attributes.pp[LENGTH] = &((nh_webidl_DOMString*)CharacterData_p->Attributes.pp[DATA])->length;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT nh_dom_appendToCharacterData(
    nh_dom_CharacterData *CharacterData_p, nh_webidl_DOMString DOMString)
{
NH_DOM_BEGIN()

    NH_WEBIDL_CHECK_2(NH_DOM_ERROR_BAD_STATE, nh_webidl_appendToDOMString(
        ((nh_webidl_Object*)CharacterData_p)->Attributes.pp[DATA], DOMString.p, DOMString.length
    ))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

nh_webidl_DOMString *nh_dom_getCharacterDataString(
    nh_dom_CharacterData *CharacterData_p)
{
NH_DOM_BEGIN()
NH_DOM_DIAGNOSTIC_END(((nh_webidl_Object*)CharacterData_p)->Attributes.pp[DATA])
}

