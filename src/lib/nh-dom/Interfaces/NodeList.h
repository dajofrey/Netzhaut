#ifndef NH_DOM_NODE_LIST_H
#define NH_DOM_NODE_LIST_H

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
    nh_ecmascript_Object *nh_dom_createNodeList(
    );

    NH_API_RESULT nh_dom_appendToNodeList(
        nh_ecmascript_Object *NodeList_p, nh_ecmascript_Object *Node_p
    );

    NH_API_RESULT nh_dom_insertIntoNodeList(
        nh_ecmascript_Object *NodeList_p, nh_ecmascript_Object *Child_p, NH_WEBIDL_UNSIGNED_LONG index
    );

    nh_ecmascript_Object *nh_dom_getFromNodeList(
        nh_ecmascript_Object *NodeList_p, NH_WEBIDL_UNSIGNED_LONG index
    );
    
    NH_WEBIDL_UNSIGNED_LONG NH_WEBIDL_GET_DOM_NODEListLength(
        nh_ecmascript_Object *NodeList_p
    );

    nh_core_List *NH_WEBIDL_GET_DOM_NODEList(
        nh_ecmascript_Object *NodeList_p
    );

/** @} */

#endif
