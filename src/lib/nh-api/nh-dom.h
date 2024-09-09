#ifndef NH_DOM_H
#define NH_DOM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-core.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @defgroup api_nh-dom nh-dom
 *  @brief TODO.
 */

/** @addtogroup api_nh-dom
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_DOM_RESULT {
        NH_DOM_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_DOM_ERROR_NULL_POINTER,
        NH_DOM_ERROR_BAD_STATE,
        NH_DOM_ERROR_MEMORY_ALLOCATION,
    } NH_DOM_RESULT;

// TYPEDEFS ========================================================================================

    typedef struct nh_dom_Node nh_dom_Node;
    typedef struct nh_dom_Element nh_dom_Element;
    typedef struct nh_dom_NodeList nh_dom_NodeList;
    typedef struct nh_dom_Attr nh_dom_Attr;
    typedef struct nh_dom_NamedNodeMap nh_dom_NamedNodeMap;
    typedef struct nh_dom_CharacterData nh_dom_CharacterData;
    typedef struct nh_dom_Text nh_dom_Text;
    typedef struct nh_dom_Comment nh_dom_Comment;

/** @} */

#endif // NH_DOM_H
