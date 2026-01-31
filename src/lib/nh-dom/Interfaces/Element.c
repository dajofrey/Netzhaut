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

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_dom_initializeElement(
    nh_ecmascript_Object *Element_p)
{
    Element_p->internal_p = nh_core_allocate(sizeof(nh_ecmascript_Object));
    NH_CORE_CHECK_MEM(Element_p->internal_p)

    Element_p->Attributes.pp[9] = nh_dom_createNamedNodeMap((nh_ecmascript_Object*)Element_p);
    NH_CORE_CHECK_MEM(Element_p->Attributes.pp[9])

    return NH_API_SUCCESS;
}

static nh_encoding_UTF8String *nh_dom_allocateQualifiedName(
    nh_ecmascript_Object *Element_p)
{
    nh_encoding_UTF8String *QualifiedName_p = (nh_encoding_UTF8String*)nh_core_allocate(sizeof(nh_encoding_UTF8String));
    NH_CORE_CHECK_MEM_2(NULL, QualifiedName_p)

    *QualifiedName_p = nh_encoding_initUTF8(16);

    nh_encoding_UTF8String *LocalName_p = ((nh_ecmascript_Object*)Element_p)->Attributes.pp[2];
    nh_encoding_UTF8String *NamespacePrefix_p = ((nh_ecmascript_Object*)Element_p)->Attributes.pp[1];

    if (!NamespacePrefix_p) {
        nh_core_appendToString(QualifiedName_p, LocalName_p->p, LocalName_p->length);
    }
    else {
        char qualifiedName_p[255] = {'\0'};
        sprintf(qualifiedName_p, "%s:%s", NamespacePrefix_p->p, LocalName_p->p);
        nh_core_appendToString(QualifiedName_p, qualifiedName_p, strlen(qualifiedName_p));
    }

    return QualifiedName_p;
}

// https://dom.spec.whatwg.org/#element-html-uppercased-qualified-name
static nh_encoding_UTF8String *nh_dom_allocateHTMLUppercasedQualifiedName(
    nh_ecmascript_Object *Element_p)
{
    nh_encoding_UTF8String *QualifiedName_p = nh_dom_allocateQualifiedName(Element_p);
    NH_CORE_CHECK_NULL_2(NULL, QualifiedName_p)

    if (((nh_ecmascript_Object*)Element_p)->Attributes.pp[0] == &NH_WEBIDL_HTML_NAMESPACE) {
        for (long unsigned int i = 0; i < QualifiedName_p->length; ++i) {
            QualifiedName_p->p[i] = toupper(QualifiedName_p->p[i]);
        }
    }

    return QualifiedName_p;
}

// https://dom.spec.whatwg.org/#concept-create-element
nh_ecmascript_Object *nh_dom_createElement(
    nh_ecmascript_Object *Document_p, nh_encoding_UTF8String *LocalName_p, nh_encoding_UTF8String *Namespace_p, nh_encoding_UTF8String *NamespacePrefix_p,
    nh_encoding_UTF8String *Is_p, bool synchronousCustomElements, nh_webidl_Interface *Interface_p)
{
    NH_CORE_CHECK_NULL_2(NULL, Document_p)
    NH_CORE_CHECK_NULL_2(NULL, LocalName_p)

    if (Interface_p == NULL) {
        Interface_p = nh_webidl_getInterface("DOM", "Element");
    }

    NH_CORE_CHECK_NULL_2(NULL, Interface_p)

    nh_ecmascript_Object *Object_p = nh_webidl_createObjectFromInterface(Interface_p);
    NH_CORE_CHECK_NULL_2(NULL, Object_p)

    nh_ecmascript_Object *Element_p = NH_WEBIDL_GET_DOM_ELEMENT(Object_p);
    NH_CORE_CHECK_NULL_2(NULL, Element_p)

    ((nh_ecmascript_Object*)Element_p)->Attributes.pp[0] = Namespace_p;
    ((nh_ecmascript_Object*)Element_p)->Attributes.pp[1] = NamespacePrefix_p;
    ((nh_ecmascript_Object*)Element_p)->Attributes.pp[2] = LocalName_p;
    ((nh_ecmascript_Object*)Element_p)->Attributes.pp[3] = nh_dom_allocateHTMLUppercasedQualifiedName(Element_p);

    NH_CORE_CHECK_NULL_2(NULL, ((nh_ecmascript_Object*)Element_p)->Attributes.pp[3])
    NH_CORE_CHECK_2(NULL, nh_dom_setNodeDocument(NH_WEBIDL_GET_DOM_NODE((nh_ecmascript_Object*)Element_p), Document_p))

    return Object_p;
}

nh_encoding_UTF8String *nh_dom_getNamespaceURI(
    nh_ecmascript_Object *Element_p)
{
    return ((nh_ecmascript_Object*)Element_p)->Attributes.pp[0];
}

nh_encoding_UTF8String *nh_dom_getLocalName(
    nh_ecmascript_Object *DOMElement_p)
{
    return DOMElement_p->Attributes.pp[2];
}

nh_encoding_UTF8String *nh_dom_getTagName(
    nh_ecmascript_Object *Element_p)
{
    return ((nh_ecmascript_Object*)Element_p)->Attributes.pp[3];
}

nh_ecmascript_Object *nh_dom_getNamedNodeMap(
    nh_ecmascript_Object *Element_p)
{
    return ((nh_ecmascript_Object*)Element_p)->Attributes.pp[9];
}

// ATTRIBUTES ======================================================================================

nh_ecmascript_Object *nh_dom_getAttrByNamespaceAndLocalName(
    nh_ecmascript_Object *Element_p, nh_encoding_UTF8String *Namespace_p, nh_encoding_UTF8String *LocalName_p)
{
    nh_core_List *AttrList_p = nh_dom_getAttrList(((nh_ecmascript_Object*)Element_p)->Attributes.pp[9]);

    for (int i = 0; i < AttrList_p->size; ++i) 
    {
        nh_ecmascript_Object *Attr_p = AttrList_p->pp[i];

        nh_encoding_UTF8String *AttrNamespace_p = nh_dom_getAttrNamespace(Attr_p);
        nh_encoding_UTF8String *AttrLocalName_p = nh_dom_getAttrLocalName(Attr_p);

        if (AttrNamespace_p &&  Namespace_p 
        &&  !strcmp(AttrNamespace_p->p, Namespace_p->p) 
        &&  !strcmp(AttrLocalName_p->p, LocalName_p->p)) {
            return Attr_p;
        }
    }

    return NULL;
}

nh_ecmascript_Object *nh_dom_getAttrByLocalName(
    nh_ecmascript_Object *DOMElement_p, char *localName_p)
{
    nh_core_List *AttrList_p = nh_dom_getAttrList(DOMElement_p->Attributes.pp[9]);

    for (int i = 0; i < AttrList_p->size; ++i) 
    {
        nh_ecmascript_Object *Attr_p = AttrList_p->pp[i];
        nh_encoding_UTF8String *AttrLocalName_p = nh_dom_getAttrLocalName(Attr_p);

        if (!strcmp(AttrLocalName_p->p, localName_p)) {
            return Attr_p;
        }
    }

    return NULL;
}

NH_API_RESULT nh_dom_replaceAttr(
    nh_ecmascript_Object *Element_p, nh_ecmascript_Object *OldAttr_p, nh_ecmascript_Object *NewAttr_p)
{
    // TODO handle attr changes

    nh_core_List *AttrList_p = nh_dom_getAttrList(((nh_ecmascript_Object*)Element_p)->Attributes.pp[9]);

    for (int i = 0; i < AttrList_p->size; ++i) {
        if (AttrList_p->pp[i] == OldAttr_p) {
            AttrList_p->pp[i] = NewAttr_p;
        }
    }

    // TODO set element

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_appendAttr(
    nh_ecmascript_Object *Element_p, nh_ecmascript_Object *Attr_p)
{
    // TODO handle attr changes

    nh_core_List *AttrList_p = nh_dom_getAttrList(((nh_ecmascript_Object*)Element_p)->Attributes.pp[9]);
    nh_core_appendToList(AttrList_p, Attr_p);
    nh_dom_setAttrElement(Attr_p, Element_p);

    return NH_API_SUCCESS;
}

nh_ecmascript_Object *nh_dom_setAttr(
    nh_ecmascript_Object *Element_p, nh_ecmascript_Object *Attr_p)
{
    nh_ecmascript_Object *AttrElement_p = nh_dom_getAttrElement(Attr_p);
    if (AttrElement_p != NULL || Element_p != AttrElement_p) {
        return NULL; 
    }

    nh_ecmascript_Object *OldAttr_p = nh_dom_getAttrByNamespaceAndLocalName(
        Element_p, nh_dom_getAttrNamespace(Attr_p), nh_dom_getAttrLocalName(Attr_p)
    );

    if (OldAttr_p == Attr_p) {return NULL;}

    if (OldAttr_p) {
        NH_CORE_CHECK_2(NULL, nh_dom_replaceAttr(Element_p, OldAttr_p, Attr_p))
    }
    else {
        nh_dom_appendAttr(Element_p, Attr_p);
    }

    return OldAttr_p;
}

