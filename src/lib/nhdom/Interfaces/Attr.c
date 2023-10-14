// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Attr.h"
#include "Node.h"

#include "../Common/Macros.h"

#include "../../nhcore/Util/List.h"
#include "../../nhcore/System/Memory.h"

#include "../../nhwebidl/Runtime/Object.h"
#include "../../nhwebidl/Runtime/String.h"
#include "../../nhwebidl/Runtime/Runtime.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_DOM_RESULT nh_dom_initializeAttr(
    nh_webidl_Object *Attr_p)
{
NH_DOM_BEGIN()

    Attr_p->Attributes.pp[3] = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_DOM_CHECK_MEM(Attr_p->Attributes.pp[3])
    *((nh_webidl_DOMString*)Attr_p->Attributes.pp[3]) = nh_webidl_initDOMString(16);

    Attr_p->Attributes.pp[4] = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_DOM_CHECK_MEM(Attr_p->Attributes.pp[4])
    *((nh_webidl_DOMString*)Attr_p->Attributes.pp[4]) = nh_webidl_initDOMString(16);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

nh_dom_Attr *nh_dom_createAttr(
    nh_webidl_Object *Document_p, nh_webidl_DOMString *LocalName_p, nh_webidl_DOMString *Namespace_p, 
    nh_webidl_DOMString *NamespacePrefix_p, nh_dom_Element *OwnerElement_p, nh_webidl_DOMString *Value_p)
{
NH_DOM_BEGIN()

    NH_DOM_CHECK_NULL_2(NULL, Document_p)
    NH_DOM_CHECK_NULL_2(NULL, LocalName_p)

    nh_dom_Attr *Attr_p = (nh_dom_Attr*)nh_webidl_createObject("DOM", "Attr");
    NH_DOM_CHECK_NULL_2(NULL, Attr_p)

    ((nh_webidl_Object*)Attr_p)->Attributes.pp[0] = Namespace_p;
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[1] = NamespacePrefix_p;
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[2] = LocalName_p;
//    ((nh_webidl_Object*)Attr_p)->Attributes.pp[3] = nh_dom_allocateHTMLUppercasedQualifiedName(Element_p);
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[5] = OwnerElement_p;
    ((nh_webidl_Object*)Attr_p)->Attributes.pp[6] = (void*)1;

    nh_webidl_appendToDOMString(((nh_webidl_Object*)Attr_p)->Attributes.pp[4], Value_p->p, Value_p->length);

    NH_DOM_CHECK_2(NULL, nh_dom_setNodeDocument(nh_dom_getNode((nh_webidl_Object*)Attr_p), Document_p))

NH_DOM_END(Attr_p)
}

nh_webidl_DOMString *nh_dom_getAttrNamespace(
    nh_dom_Attr *Attr_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Attr_p)->Attributes.pp[0])
}

nh_webidl_DOMString *nh_dom_getAttrLocalName(
    nh_dom_Attr *Attr_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Attr_p)->Attributes.pp[2])
}

void nh_dom_setAttrElement(
    nh_dom_Attr *Attr_p, nh_dom_Element *Element_p)
{
NH_DOM_BEGIN()

    ((nh_webidl_Object*)Attr_p)->Attributes.pp[5] = Element_p;

NH_DOM_SILENT_END()
}

nh_dom_Element *nh_dom_getAttrElement(
    nh_dom_Attr *Attr_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Attr_p)->Attributes.pp[5])
}

nh_webidl_DOMString *nh_dom_getAttrValue(
    nh_dom_Attr *Attr_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Attr_p)->Attributes.pp[4])
}

