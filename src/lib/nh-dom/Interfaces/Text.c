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

// FUNCTIONS =======================================================================================

nh_webidl_Object *nh_dom_createText(
    nh_webidl_DOMString Data, nh_webidl_Object *NodeDocument_p)
{
    nh_webidl_Object *Text_p = nh_webidl_createObject("DOM", "Text");
    NH_CORE_CHECK_MEM_2(NULL, Text_p)

    NH_CORE_CHECK_2(NULL, nh_dom_setNodeDocument(NH_WEBIDL_GET_DOM_NODE(Text_p), NodeDocument_p))
    NH_CORE_CHECK_2(NULL, nh_dom_appendToCharacterData(Text_p->Parent_p, Data))

    return Text_p;
}

NH_API_RESULT nh_dom_appendToText(
    nh_webidl_Object *Text_p, nh_webidl_DOMString DOMString)
{
    return nh_dom_appendToCharacterData(Text_p->Parent_p, DOMString);
}

nh_webidl_DOMString *nh_dom_getTextString(
    nh_webidl_Object *Text_p)
{
    return nh_dom_getCharacterDataString(Text_p->Parent_p);
}
