#ifndef NH_DOM_ATTR_H
#define NH_DOM_ATTR_H

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

    nh_dom_Attr *nh_dom_createAttr(
        nh_webidl_Object *Document_p, nh_webidl_DOMString *LocalName_p, nh_webidl_DOMString *Namespace_p, 
        nh_webidl_DOMString *NamespacePrefix_p, nh_dom_Element *OwnerElement_p, nh_webidl_DOMString *Value_p
    );

    nh_webidl_DOMString *nh_dom_getAttrNamespace(
        nh_dom_Attr *Attr_p
    );
    
    nh_webidl_DOMString *nh_dom_getAttrLocalName(
        nh_dom_Attr *Attr_p
    );

    void nh_dom_setAttrElement(
        nh_dom_Attr *Attr_p, nh_dom_Element *Element_p
    );

    nh_dom_Element *nh_dom_getAttrElement(
        nh_dom_Attr *Attr_p
    );

    nh_webidl_DOMString *nh_dom_getAttrValue(
        nh_dom_Attr *Attr_p
    );

/** @} */

#endif
