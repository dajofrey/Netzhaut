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

/** @addtogroup lib_nh-dom_functions
 *  @{
 */
    nh_webidl_Object *nh_dom_createNamedNodeMap(
        nh_webidl_Object *Element_p
    );

    nh_webidl_Object* nh_dom_getAttr(
        nh_webidl_Object *NamedNodeMap_p, NH_WEBIDL_UNSIGNED_LONG index
    );

    nh_core_List *nh_dom_getAttrList(
        nh_webidl_Object *NamedNodeMap_p
    );

/** @} */

#endif
