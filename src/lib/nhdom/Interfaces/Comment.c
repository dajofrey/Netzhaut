// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Node.h"
#include "CharacterData.h"

#include "../Common/Macros.h"

#include "../../nhwebidl/Runtime/String.h"
#include "../../nhwebidl/Runtime/Object.h"

#include "../../nhcore/Util/List.h"
#include "../../nhcore/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_DOM_RESULT nh_dom_initializeComment(
    nh_webidl_Object *Node_p)
{
NH_DOM_BEGIN()

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

nh_dom_Comment *nh_dom_createComment(
    nh_webidl_DOMString DOMString)
{
NH_DOM_BEGIN()

    nh_webidl_Object *Comment_p = nh_webidl_createObject("DOM", "Comment");
    NH_DOM_CHECK_MEM_2(NULL, Comment_p)
    NH_DOM_CHECK_2(NULL, nh_dom_appendToCharacterData((nh_dom_CharacterData*)Comment_p->Child_p, DOMString))

NH_DOM_END((nh_dom_Comment*)Comment_p)
}

