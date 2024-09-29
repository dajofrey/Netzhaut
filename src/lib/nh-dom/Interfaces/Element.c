// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Element.h"
#include "Node.h"
#include "NamedNodeMap.h"
#include "Attr.h"

#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-webidl/Runtime/Object.h"
#include "../../nh-webidl/Runtime/String.h"
#include "../../nh-webidl/Runtime/Runtime.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

typedef struct nh_dom_Element {
    int TODO;
} nh_dom_Element;

// INITIALIZE ======================================================================================

NH_API_RESULT nh_dom_initializeElement(
    nh_webidl_Object *Element_p)
{
NH_DOM_BEGIN()

    Element_p->internal_p = nh_core_allocate(sizeof(nh_dom_Element));
    NH_DOM_CHECK_MEM(Element_p->internal_p)

    Element_p->Attributes.pp[9] = nh_dom_createNamedNodeMap((nh_dom_Element*)Element_p);
    NH_DOM_CHECK_MEM(Element_p->Attributes.pp[9])

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// INTERNAL ========================================================================================

nh_dom_Element *nh_dom_getElement(
    nh_webidl_Object *Object_p)
{
NH_DOM_BEGIN()
NH_DOM_END((nh_dom_Element*)nh_webidl_getObject(Object_p, "DOM", "Element"))
}

static nh_webidl_DOMString *nh_dom_allocateQualifiedName(
    nh_dom_Element *Element_p)
{
NH_DOM_BEGIN()

    nh_webidl_DOMString *QualifiedName_p = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_DOM_CHECK_MEM_2(NULL, QualifiedName_p)

    *QualifiedName_p = nh_webidl_initDOMString(16);

    nh_webidl_DOMString *LocalName_p = ((nh_webidl_Object*)Element_p)->Attributes.pp[2];
    nh_webidl_DOMString *NamespacePrefix_p = ((nh_webidl_Object*)Element_p)->Attributes.pp[1];

    if (!NamespacePrefix_p) {
        nh_webidl_appendToDOMString(QualifiedName_p, LocalName_p->p, LocalName_p->length);
    }
    else {
        char qualifiedName_p[255] = {'\0'};
        sprintf(qualifiedName_p, "%s:%s", NamespacePrefix_p->p, LocalName_p->p);
        nh_webidl_appendToDOMString(QualifiedName_p, qualifiedName_p, strlen(qualifiedName_p));
    }

NH_DOM_END(QualifiedName_p)
}

// https://dom.spec.whatwg.org/#element-html-uppercased-qualified-name
static nh_webidl_DOMString *nh_dom_allocateHTMLUppercasedQualifiedName(
    nh_dom_Element *Element_p)
{
NH_DOM_BEGIN()

    nh_webidl_DOMString *QualifiedName_p = nh_dom_allocateQualifiedName(Element_p);
    NH_DOM_CHECK_NULL_2(NULL, QualifiedName_p)

    if (((nh_webidl_Object*)Element_p)->Attributes.pp[0] == &NH_WEBIDL_HTML_NAMESPACE) {
        for (long unsigned int i = 0; i < QualifiedName_p->length; ++i) {
            QualifiedName_p->p[i] = toupper(QualifiedName_p->p[i]);
        }
    }

NH_DOM_END(QualifiedName_p)
}

// https://dom.spec.whatwg.org/#concept-create-element
nh_webidl_Object *nh_dom_createElement(
    nh_webidl_Object *Document_p, nh_webidl_DOMString *LocalName_p, nh_webidl_DOMString *Namespace_p, nh_webidl_DOMString *NamespacePrefix_p,
    nh_webidl_DOMString *Is_p, bool synchronousCustomElements, nh_webidl_Interface *Interface_p)
{
NH_DOM_BEGIN()

    NH_DOM_CHECK_NULL_2(NULL, Document_p)
    NH_DOM_CHECK_NULL_2(NULL, LocalName_p)

    if (Interface_p == NULL) {
        Interface_p = nh_webidl_getInterface("DOM", "Element");
    }

    NH_DOM_CHECK_NULL_2(NULL, Interface_p)

    nh_webidl_Object *Object_p = nh_webidl_createObjectFromInterface(Interface_p);
    NH_DOM_CHECK_NULL_2(NULL, Object_p)

    nh_dom_Element *Element_p = nh_dom_getElement(Object_p);
    NH_DOM_CHECK_NULL_2(NULL, Element_p)

    ((nh_webidl_Object*)Element_p)->Attributes.pp[0] = Namespace_p;
    ((nh_webidl_Object*)Element_p)->Attributes.pp[1] = NamespacePrefix_p;
    ((nh_webidl_Object*)Element_p)->Attributes.pp[2] = LocalName_p;
    ((nh_webidl_Object*)Element_p)->Attributes.pp[3] = nh_dom_allocateHTMLUppercasedQualifiedName(Element_p);

    NH_DOM_CHECK_NULL_2(NULL, ((nh_webidl_Object*)Element_p)->Attributes.pp[3])
    NH_DOM_CHECK_2(NULL, nh_dom_setNodeDocument(nh_dom_getNode((nh_webidl_Object*)Element_p), Document_p))

NH_DOM_END(Object_p)
}

nh_webidl_DOMString *nh_dom_getNamespaceURI(
    nh_dom_Element *Element_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Element_p)->Attributes.pp[0])
}

nh_webidl_DOMString *nh_dom_getLocalName(
    nh_dom_Element *Element_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Element_p)->Attributes.pp[2])
}

nh_webidl_DOMString *nh_dom_getTagName(
    nh_dom_Element *Element_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Element_p)->Attributes.pp[3])
}

nh_dom_NamedNodeMap *nh_dom_getNamedNodeMap(
    nh_dom_Element *Element_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Element_p)->Attributes.pp[9])
}

// ATTRIBUTES ======================================================================================

nh_dom_Attr *nh_dom_getAttrByNamespaceAndLocalName(
    nh_dom_Element *Element_p, nh_webidl_DOMString *Namespace_p, nh_webidl_DOMString *LocalName_p)
{
NH_DOM_BEGIN()

    nh_List *AttrList_p = nh_dom_getAttrList(((nh_webidl_Object*)Element_p)->Attributes.pp[9]);

    for (int i = 0; i < AttrList_p->size; ++i) 
    {
        nh_dom_Attr *Attr_p = AttrList_p->pp[i];

        nh_webidl_DOMString *AttrNamespace_p = nh_dom_getAttrNamespace(Attr_p);
        nh_webidl_DOMString *AttrLocalName_p = nh_dom_getAttrLocalName(Attr_p);

        if (AttrNamespace_p &&  Namespace_p 
        &&  !strcmp(AttrNamespace_p->p, Namespace_p->p) 
        &&  !strcmp(AttrLocalName_p->p, LocalName_p->p)) {
            NH_DOM_END(Attr_p)
        }
    }

NH_DOM_END(NULL)
}

nh_dom_Attr *nh_dom_getAttrByLocalName(
    nh_dom_Element *Element_p, char *localName_p)
{
NH_DOM_BEGIN()

    nh_List *AttrList_p = nh_dom_getAttrList(((nh_webidl_Object*)Element_p)->Attributes.pp[9]);

    for (int i = 0; i < AttrList_p->size; ++i) 
    {
        nh_dom_Attr *Attr_p = AttrList_p->pp[i];
        nh_webidl_DOMString *AttrLocalName_p = nh_dom_getAttrLocalName(Attr_p);

        if (!strcmp(AttrLocalName_p->p, localName_p)) {
            NH_DOM_END(Attr_p)
        }
    }

NH_DOM_END(NULL)
}

NH_API_RESULT nh_dom_replaceAttr(
    nh_dom_Element *Element_p, nh_dom_Attr *OldAttr_p, nh_dom_Attr *NewAttr_p)
{
NH_DOM_BEGIN()

    // TODO handle attr changes

    nh_List *AttrList_p = nh_dom_getAttrList(((nh_webidl_Object*)Element_p)->Attributes.pp[9]);

    for (int i = 0; i < AttrList_p->size; ++i) {
        if (AttrList_p->pp[i] == OldAttr_p) {
            AttrList_p->pp[i] = NewAttr_p;
        }
    }

    // TODO set element

NH_DOM_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_dom_appendAttr(
    nh_dom_Element *Element_p, nh_dom_Attr *Attr_p)
{
NH_DOM_BEGIN()

    // TODO handle attr changes

    nh_List *AttrList_p = nh_dom_getAttrList(((nh_webidl_Object*)Element_p)->Attributes.pp[9]);
    nh_core_appendToList(AttrList_p, Attr_p);
    nh_dom_setAttrElement(Attr_p, Element_p);

NH_DOM_END(NH_API_SUCCESS)
}

nh_dom_Attr *nh_dom_setAttr(
    nh_dom_Element *Element_p, nh_dom_Attr *Attr_p)
{
NH_DOM_BEGIN()

    nh_dom_Element *AttrElement_p = nh_dom_getAttrElement(Attr_p);
    if (AttrElement_p != NULL || Element_p != AttrElement_p) {
        NH_DOM_END(NULL) 
    }

    nh_dom_Attr *OldAttr_p = nh_dom_getAttrByNamespaceAndLocalName(
        Element_p, nh_dom_getAttrNamespace(Attr_p), nh_dom_getAttrLocalName(Attr_p)
    );

    if (OldAttr_p == Attr_p) {NH_DOM_END(NULL)}

    if (OldAttr_p) {
        NH_DOM_CHECK_2(NULL, nh_dom_replaceAttr(Element_p, OldAttr_p, Attr_p))
    }
    else {
        nh_dom_appendAttr(Element_p, Attr_p);
    }

NH_DOM_END(OldAttr_p)
}

