#ifndef NH_DOM_INTERFACES_NODE_H
#define NH_DOM_INTERFACES_NODE_H

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

    NH_API_RESULT nh_dom_appendToNode(
        nh_ecmascript_Object *Node_p, nh_ecmascript_Object *Child_p
    );

    NH_API_RESULT nh_dom_insertIntoNode(
        nh_ecmascript_Object *Node_p, nh_ecmascript_Object *Child_p, NH_WEBIDL_UNSIGNED_LONG index
    );

    nh_core_List *nh_dom_getNodeChildren(
        nh_ecmascript_Object *Node_p
    );

    NH_API_RESULT nh_dom_setNodeDocument(
        nh_ecmascript_Object *Node_p, nh_ecmascript_Object *NodeDocument_p
    );

    nh_ecmascript_Object *nh_dom_getNodeDocument(
        nh_ecmascript_Object *Node_p
    );

    NH_API_RESULT nh_dom_setSpecifiedPropertyValues(
        nh_ecmascript_Object *Node_p, nh_core_Array SpecifiedPropertyValues
    );

    NH_API_RESULT nh_dom_setComputedPropertyValues(
        nh_ecmascript_Object *Node_p, nh_core_List ComputedPropertyValues
    );

    nh_core_Array *nh_dom_getSpecifiedPropertyValues(
        nh_ecmascript_Object *Node_p
    );

    nh_core_List *nh_dom_getComputedPropertyValues(
        nh_ecmascript_Object *Node_p
    );

    bool nh_dom_getUpdateAnnotationsFlag(
        nh_ecmascript_Object *Node_p
    );

    NH_API_RESULT nh_dom_setUpdateAnnotationsFlag(
        nh_ecmascript_Object *Node_p, bool update
    );

    void nh_dom_setParentNode(
        nh_ecmascript_Object *Node_p, nh_ecmascript_Object *Parent_p
    );

    nh_ecmascript_Object *nh_dom_getParentNode(
        nh_ecmascript_Object *Node_p
    );

    nh_ecmascript_Object *nh_dom_getParentElement(
        nh_ecmascript_Object *Node_p
    );

/** @} */

#endif
