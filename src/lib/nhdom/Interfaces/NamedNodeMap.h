#ifndef NH_DOM_NAMED_NODE_MAP_H
#define NH_DOM_NAMED_NODE_MAP_H

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

    nh_dom_NamedNodeMap *nh_dom_createNamedNodeMap(
        nh_dom_Element *Element_p
    );

    nh_dom_Attr* nh_dom_getAttr(
        nh_dom_NamedNodeMap *NamedNodeMap_p, NH_WEBIDL_UNSIGNED_LONG index
    );

    nh_List *nh_dom_getAttrList(
        nh_dom_NamedNodeMap *NamedNodeMap_p
    );

/** @} */

#endif
