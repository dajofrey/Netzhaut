// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "NodeList.h"

#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define NODES NodeList_p->internal_p
#define LENGTH NodeList_p->Attributes.pp[0]

// INITIALIZE ======================================================================================

NH_API_RESULT nh_dom_initializeNodeList(
    nh_webidl_Object *NodeList_p)
{
NH_DOM_BEGIN()

    NodeList_p->internal_p = nh_core_allocate(sizeof(nh_List));
    NH_DOM_CHECK_MEM(NodeList_p->internal_p)

    *((nh_List*)NodeList_p->internal_p) = nh_core_initList(16);
    NodeList_p->Attributes.pp[0] = &((nh_List*)NodeList_p->internal_p)->size;

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// INTERNAL ========================================================================================

nh_dom_NodeList *nh_dom_createNodeList()
{
NH_DOM_BEGIN()
NH_DOM_END((nh_dom_NodeList*)nh_webidl_createObject("DOM", "NodeList"))
}

NH_API_RESULT nh_dom_appendToNodeList(
    nh_dom_NodeList *NodeList_p, nh_webidl_Object *Node_p)
{
NH_DOM_BEGIN()

    nh_core_appendToList(((nh_webidl_Object*)NodeList_p)->internal_p, Node_p);

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_dom_insertIntoNodeList(
    nh_dom_NodeList *NodeList_p, nh_webidl_Object *Node_p, NH_WEBIDL_UNSIGNED_LONG index)
{
NH_DOM_BEGIN()

    nh_core_insertIntoList(((nh_webidl_Object*)NodeList_p)->internal_p, Node_p, index);

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_webidl_Object *nh_dom_getFromNodeList(
    nh_dom_NodeList *NodeList_p, NH_WEBIDL_UNSIGNED_LONG index)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_List*)((nh_webidl_Object*)NodeList_p)->internal_p)->pp[index])
}

NH_WEBIDL_UNSIGNED_LONG nh_dom_getNodeListLength(
    nh_dom_NodeList *NodeList_p)
{
NH_DOM_BEGIN()
NH_DOM_END(*((NH_WEBIDL_UNSIGNED_LONG*)((nh_webidl_Object*)NodeList_p)->Attributes.pp[0]))
}

nh_List *nh_dom_getNodeList(
    nh_dom_NodeList *NodeList_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)NodeList_p)->internal_p)
}

