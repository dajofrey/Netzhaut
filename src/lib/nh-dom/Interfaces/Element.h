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

/** @addtogroup lib_nh-dom_functions
 *  @{
 */

    nh_webidl_Object *nh_dom_createElement(
        nh_webidl_Object *Document_p, nh_webidl_DOMString *LocalName_p, nh_webidl_DOMString *Namespace_p, 
        nh_webidl_DOMString *NamespacePrefix_p, nh_webidl_DOMString *Is_p, bool synchronousCustomElements, 
        nh_webidl_Interface *Interface_p
    );

    nh_webidl_DOMString *nh_dom_getLocalName(
        nh_webidl_Object *DOMElement_p
    );

    nh_webidl_DOMString *nh_dom_getNamespaceURI(
        nh_webidl_Object *Element_p
    );

    nh_webidl_DOMString *nh_dom_getTagName(
        nh_webidl_Object *Element_p
    );

    nh_webidl_Object *nh_dom_getNamedNodeMap(
        nh_webidl_Object *Element_p
    );

    nh_webidl_Object *nh_dom_getAttrByNamespaceAndLocalName(
        nh_webidl_Object *Element_p, nh_webidl_DOMString *Namespace_p, nh_webidl_DOMString *LocalName_p
    );
    
    nh_webidl_Object *nh_dom_getAttrByLocalName(
        nh_webidl_Object *DOMElement_p, char *localName_p
    );

    NH_API_RESULT nh_dom_replaceAttr(
        nh_webidl_Object *Element_p, nh_webidl_Object *OldAttr_p, nh_webidl_Object *NewAttr_p
    );
    
    NH_API_RESULT nh_dom_appendAttr(
        nh_webidl_Object *Element_p, nh_webidl_Object *Attr_p
    );
    
    nh_webidl_Object *nh_dom_setAttr(
        nh_webidl_Object *Element_p, nh_webidl_Object *Attr_p
    );

/** @} */

#endif
