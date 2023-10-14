#ifndef NH_DOM_ELEMENT_H
#define NH_DOM_ELEMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhdom_functions
 *  @{
 */

    nh_dom_Element *nh_dom_getElement(
        nh_webidl_Object *Object_p
    );

    nh_webidl_Object *nh_dom_createElement(
        nh_webidl_Object *Document_p, nh_webidl_DOMString *LocalName_p, nh_webidl_DOMString *Namespace_p, 
        nh_webidl_DOMString *NamespacePrefix_p, nh_webidl_DOMString *Is_p, NH_BOOL synchronousCustomElements, 
        nh_webidl_Interface *Interface_p
    );

    nh_webidl_DOMString *nh_dom_getLocalName(
        nh_dom_Element *Element_p
    );

    nh_webidl_DOMString *nh_dom_getNamespaceURI(
        nh_dom_Element *Element_p
    );

    nh_webidl_DOMString *nh_dom_getTagName(
        nh_dom_Element *Element_p
    );

    nh_dom_NamedNodeMap *nh_dom_getNamedNodeMap(
        nh_dom_Element *Element_p
    );

    nh_dom_Attr *nh_dom_getAttrByNamespaceAndLocalName(
        nh_dom_Element *Element_p, nh_webidl_DOMString *Namespace_p, nh_webidl_DOMString *LocalName_p
    );
    
    nh_dom_Attr *nh_dom_getAttrByLocalName(
        nh_dom_Element *Element_p, NH_BYTE *localName_p
    );

    NH_DOM_RESULT nh_dom_replaceAttr(
        nh_dom_Element *Element_p, nh_dom_Attr *OldAttr_p, nh_dom_Attr *NewAttr_p
    );
    
    NH_DOM_RESULT nh_dom_appendAttr(
        nh_dom_Element *Element_p, nh_dom_Attr *Attr_p
    );
    
    nh_dom_Attr *nh_dom_setAttr(
        nh_dom_Element *Element_p, nh_dom_Attr *Attr_p
    );

/** @} */

#endif
