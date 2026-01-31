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

    nh_ecmascript_Object *nh_dom_createElement(
        nh_ecmascript_Object *Document_p, nh_encoding_UTF8String *LocalName_p, nh_encoding_UTF8String *Namespace_p, 
        nh_encoding_UTF8String *NamespacePrefix_p, nh_encoding_UTF8String *Is_p, bool synchronousCustomElements, 
        nh_webidl_Interface *Interface_p
    );

    nh_encoding_UTF8String *nh_dom_getLocalName(
        nh_ecmascript_Object *DOMElement_p
    );

    nh_encoding_UTF8String *nh_dom_getNamespaceURI(
        nh_ecmascript_Object *Element_p
    );

    nh_encoding_UTF8String *nh_dom_getTagName(
        nh_ecmascript_Object *Element_p
    );

    nh_ecmascript_Object *nh_dom_getNamedNodeMap(
        nh_ecmascript_Object *Element_p
    );

    nh_ecmascript_Object *nh_dom_getAttrByNamespaceAndLocalName(
        nh_ecmascript_Object *Element_p, nh_encoding_UTF8String *Namespace_p, nh_encoding_UTF8String *LocalName_p
    );
    
    nh_ecmascript_Object *nh_dom_getAttrByLocalName(
        nh_ecmascript_Object *DOMElement_p, char *localName_p
    );

    NH_API_RESULT nh_dom_replaceAttr(
        nh_ecmascript_Object *Element_p, nh_ecmascript_Object *OldAttr_p, nh_ecmascript_Object *NewAttr_p
    );
    
    NH_API_RESULT nh_dom_appendAttr(
        nh_ecmascript_Object *Element_p, nh_ecmascript_Object *Attr_p
    );
    
    nh_ecmascript_Object *nh_dom_setAttr(
        nh_ecmascript_Object *Element_p, nh_ecmascript_Object *Attr_p
    );

/** @} */

#endif
