// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Node.h"
#include "CharacterData.h"

#include "../../nh-webidl/Runtime/String.h"
#include "../../nh-webidl/Runtime/Object.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_API_RESULT nh_dom_initializeComment(
    nh_webidl_Object *Node_p)
{
    return NH_API_SUCCESS;
}

// INTERNAL ========================================================================================

nh_webidl_Object *nh_dom_createComment(
    nh_webidl_DOMString DOMString)
{
    nh_webidl_Object *Comment_p = nh_webidl_createObject("DOM", "Comment");
    NH_CORE_CHECK_MEM_2(NULL, Comment_p)
    NH_CORE_CHECK_2(NULL, nh_dom_appendToCharacterData((nh_webidl_Object*)Comment_p->Parent_p, DOMString))
    return (nh_webidl_Object*)Comment_p;
}

