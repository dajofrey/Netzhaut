// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "NamedNodeMap.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef struct nh_dom_NamedNodeMapInternal {
    nh_dom_Element *Element_p;
    nh_core_List AttributeList;
} nh_dom_NamedNodeMapInternal;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_dom_initializeNamedNodeMap(
    nh_webidl_Object *NamedNodeMap_p)
{
    NamedNodeMap_p->internal_p = nh_core_allocate(sizeof(nh_dom_NamedNodeMapInternal));
    NH_CORE_CHECK_MEM(NamedNodeMap_p->internal_p)

    ((nh_dom_NamedNodeMapInternal*)NamedNodeMap_p->internal_p)->AttributeList = nh_core_initList(8);
    NamedNodeMap_p->Attributes.pp[0] = &((nh_dom_NamedNodeMapInternal*)NamedNodeMap_p->internal_p)->AttributeList.size;

    return NH_API_SUCCESS;
}

nh_dom_NamedNodeMap *nh_dom_createNamedNodeMap(
    nh_dom_Element *Element_p)
{
    nh_webidl_Object *NamedNodeMap_p = nh_webidl_createObject("DOM", "NamedNodeMap");
    NH_CORE_CHECK_MEM_2(NULL, NamedNodeMap_p)

    ((nh_dom_NamedNodeMapInternal*)NamedNodeMap_p->internal_p)->Element_p = Element_p;

    return (nh_dom_NamedNodeMap*)NamedNodeMap_p;
}

nh_dom_Attr* nh_dom_getAttr(
    nh_dom_NamedNodeMap *NamedNodeMap_p, NH_WEBIDL_UNSIGNED_LONG index)
{
    nh_core_List *AttributeList_p = 
        &((nh_dom_NamedNodeMapInternal*)((nh_webidl_Object*)NamedNodeMap_p)->internal_p)->AttributeList;

    if (index >= AttributeList_p->size) {
        return NULL;
    }
 
    return AttributeList_p->pp[index];
}

nh_core_List *nh_dom_getAttrList(
    nh_dom_NamedNodeMap *NamedNodeMap_p)
{
    return &((nh_dom_NamedNodeMapInternal*)((nh_webidl_Object*)NamedNodeMap_p)->internal_p)->AttributeList;
}
