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
    nh_dom_NodeList *nh_dom_createNodeList(
    );

    NH_API_RESULT nh_dom_appendToNodeList(
        nh_dom_NodeList *NodeList_p, nh_webidl_Object *Node_p
    );

    NH_API_RESULT nh_dom_insertIntoNodeList(
        nh_dom_NodeList *NodeList_p, nh_webidl_Object *Child_p, NH_WEBIDL_UNSIGNED_LONG index
    );

    nh_webidl_Object *nh_dom_getFromNodeList(
        nh_dom_NodeList *NodeList_p, NH_WEBIDL_UNSIGNED_LONG index
    );
    
    NH_WEBIDL_UNSIGNED_LONG nh_dom_getNodeListLength(
        nh_dom_NodeList *NodeList_p
    );

    nh_List *nh_dom_getNodeList(
        nh_dom_NodeList *NodeList_p
    );

/** @} */

#endif
