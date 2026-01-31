// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "NodeList.h"

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
    nh_ecmascript_Object *NodeList_p)
{
    NodeList_p->internal_p = nh_core_allocate(sizeof(nh_core_List));
    NH_CORE_CHECK_MEM(NodeList_p->internal_p)

    *((nh_core_List*)NodeList_p->internal_p) = nh_core_initList(16);
    NodeList_p->Attributes.pp[0] = &((nh_core_List*)NodeList_p->internal_p)->size;

    return NH_API_SUCCESS;
}

// INTERNAL ========================================================================================

nh_ecmascript_Object *nh_dom_createNodeList()
{
    return (nh_ecmascript_Object*)nh_webidl_createObject("DOM", "NodeList");
}

NH_API_RESULT nh_dom_appendToNodeList(
    nh_ecmascript_Object *NodeList_p, nh_ecmascript_Object *Node_p)
{
    nh_core_appendToList(((nh_ecmascript_Object*)NodeList_p)->internal_p, Node_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_insertIntoNodeList(
    nh_ecmascript_Object *NodeList_p, nh_ecmascript_Object *Node_p, NH_WEBIDL_UNSIGNED_LONG index)
{
    nh_core_insertIntoList(((nh_ecmascript_Object*)NodeList_p)->internal_p, Node_p, index);

    return NH_API_SUCCESS;
}

nh_ecmascript_Object *nh_dom_getFromNodeList(
    nh_ecmascript_Object *NodeList_p, NH_WEBIDL_UNSIGNED_LONG index)
{
    return ((nh_core_List*)((nh_ecmascript_Object*)NodeList_p)->internal_p)->pp[index];
}

NH_WEBIDL_UNSIGNED_LONG NH_WEBIDL_GET_DOM_NODEListLength(
    nh_ecmascript_Object *NodeList_p)
{
    return *((NH_WEBIDL_UNSIGNED_LONG*)((nh_ecmascript_Object*)NodeList_p)->Attributes.pp[0]);
}

nh_core_List *NH_WEBIDL_GET_DOM_NODEList(
    nh_ecmascript_Object *NodeList_p)
{
    return ((nh_ecmascript_Object*)NodeList_p)->internal_p;
}

