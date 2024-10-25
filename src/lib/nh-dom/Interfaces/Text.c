// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Text.h"
#include "Node.h"
#include "CharacterData.h"

#include "../../nh-webidl/Runtime/Object.h"
#include "../../nh-webidl/Runtime/String.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_API_RESULT nh_dom_initializeText(
    nh_webidl_Object *CharacterData_p)
{
    return NH_API_SUCCESS;
}

// INTERNAL ========================================================================================

nh_dom_Text *nh_dom_getText(
    nh_webidl_Object *Object_p)
{
    return (nh_dom_Text*)nh_webidl_getObject(Object_p, "DOM", "Text");
}

nh_dom_Text *nh_dom_createText(
    nh_webidl_DOMString Data, nh_webidl_Object *NodeDocument_p)
{
    nh_webidl_Object *Text_p = nh_webidl_createObject("DOM", "Text");
    NH_CORE_CHECK_MEM_2(NULL, Text_p)

    NH_CORE_CHECK_2(NULL, nh_dom_setNodeDocument(nh_dom_getNode(Text_p), NodeDocument_p))
    NH_CORE_CHECK_2(NULL, nh_dom_appendToCharacterData((nh_dom_CharacterData*)Text_p->Parent_p, Data))

    return (nh_dom_Text*)Text_p;
}

NH_API_RESULT nh_dom_appendToText(
    nh_dom_Text *Text_p, nh_webidl_DOMString DOMString)
{
    NH_CORE_CHECK(nh_dom_appendToCharacterData(
        (nh_dom_CharacterData*)((nh_webidl_Object*)Text_p)->Parent_p, DOMString))
    return NH_API_SUCCESS;
}

nh_webidl_DOMString *nh_dom_getTextString(
    nh_dom_Text *Text_p)
{
    return nh_dom_getCharacterDataString((nh_dom_CharacterData*)((nh_webidl_Object*)Text_p)->Parent_p);
}

