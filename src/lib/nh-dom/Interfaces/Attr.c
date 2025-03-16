// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Attr.h"
#include "Node.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-webidl/Runtime/Object.h"
#include "../../nh-webidl/Runtime/String.h"
#include "../../nh-webidl/Runtime/Runtime.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_dom_initializeAttr(
    nh_webidl_Object *Attr_p)
{
    Attr_p->Attributes.pp[3] = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_CORE_CHECK_MEM(Attr_p->Attributes.pp[3])
    *((nh_webidl_DOMString*)Attr_p->Attributes.pp[3]) = nh_webidl_initDOMString(16);

    Attr_p->Attributes.pp[4] = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_CORE_CHECK_MEM(Attr_p->Attributes.pp[4])
    *((nh_webidl_DOMString*)Attr_p->Attributes.pp[4]) = nh_webidl_initDOMString(16);

    return NH_API_SUCCESS;
}

nh_webidl_Object *nh_dom_createAttr(
    nh_webidl_Object *Document_p, nh_webidl_DOMString *LocalName_p, nh_webidl_DOMString *Namespace_p, 
    nh_webidl_DOMString *NamespacePrefix_p, nh_webidl_Object *OwnerElement_p, nh_webidl_DOMString *Value_p)
{
    NH_CORE_CHECK_NULL_2(NULL, Document_p)
    NH_CORE_CHECK_NULL_2(NULL, LocalName_p)

    nh_webidl_Object *Attr_p = (nh_webidl_Object*)nh_webidl_createObject("DOM", "Attr");
    NH_CORE_CHECK_NULL_2(NULL, Attr_p)

    ((nh_webidl_Object*)Attr_p)->Attributes.pp[0] = Namespace_p;
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[1] = NamespacePrefix_p;
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[2] = LocalName_p;
//    ((nh_webidl_Object*)Attr_p)->Attributes.pp[3] = nh_dom_allocateHTMLUppercasedQualifiedName(Element_p);
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[5] = OwnerElement_p;
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[6] = (void*)1;

    nh_webidl_appendToDOMString(((nh_webidl_Object*)Attr_p)->Attributes.pp[4], Value_p->p, Value_p->length);

    NH_CORE_CHECK_2(NULL, nh_dom_setNodeDocument(NH_WEBIDL_GET_DOM_NODE((nh_webidl_Object*)Attr_p), Document_p))

    return Attr_p;
}

nh_webidl_DOMString *nh_dom_getAttrNamespace(
    nh_webidl_Object *Attr_p)
{
    return ((nh_webidl_Object*)Attr_p)->Attributes.pp[0];
}

nh_webidl_DOMString *nh_dom_getAttrLocalName(
    nh_webidl_Object *Attr_p)
{
    return ((nh_webidl_Object*)Attr_p)->Attributes.pp[2];
}

void nh_dom_setAttrElement(
    nh_webidl_Object *Attr_p, nh_webidl_Object *Element_p)
{
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[5] = Element_p;
    return;
}

nh_webidl_Object *nh_dom_getAttrElement(
    nh_webidl_Object *Attr_p)
{
    return ((nh_webidl_Object*)Attr_p)->Attributes.pp[5];
}

nh_webidl_DOMString *nh_dom_getAttrValue(
    nh_webidl_Object *Attr_p)
{
    return ((nh_webidl_Object*)Attr_p)->Attributes.pp[4];
}
