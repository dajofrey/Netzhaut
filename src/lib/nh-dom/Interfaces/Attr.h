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

    nh_ecmascript_Object *nh_dom_createAttr(
        nh_ecmascript_Object *Document_p, nh_encoding_UTF8String *LocalName_p, nh_encoding_UTF8String *Namespace_p, 
        nh_encoding_UTF8String *NamespacePrefix_p, nh_ecmascript_Object *OwnerElement_p, nh_encoding_UTF8String *Value_p
    );

    nh_encoding_UTF8String *nh_dom_getAttrNamespace(
        nh_ecmascript_Object *Attr_p
    );
    
    nh_encoding_UTF8String *nh_dom_getAttrLocalName(
        nh_ecmascript_Object *Attr_p
    );

    void nh_dom_setAttrElement(
        nh_ecmascript_Object *Attr_p, nh_ecmascript_Object *Element_p
    );

    nh_ecmascript_Object *nh_dom_getAttrElement(
        nh_ecmascript_Object *Attr_p
    );

    nh_encoding_UTF8String *nh_dom_getAttrValue(
        nh_ecmascript_Object *Attr_p
    );

/** @} */

#endif
