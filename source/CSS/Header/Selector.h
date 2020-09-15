#ifndef NH_CSS_SELECTOR_H
#define NH_CSS_SELECTOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/HashMap.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_SELECTOR {
        NH_CSS_SELECTOR_UNDEFINED,
        NH_CSS_SELECTOR_UNIVERSAL,
        NH_CSS_SELECTOR_TYPE,
        NH_CSS_SELECTOR_CLASS,
        NH_CSS_SELECTOR_ID,
        NH_CSS_SELECTOR_ATTRIBUTE,
        NH_CSS_SELECTOR_LIST,
        NH_CSS_SELECTOR_DESCENDANT_COMBINATOR,
        NH_CSS_SELECTOR_CHILD_COMBINATOR,
        NH_CSS_SELECTOR_GENERAL_SIBLING_COMBINATOR,
        NH_CSS_SELECTOR_ADJACENT_SIBLING_COMBINATOR,
        NH_CSS_SELECTOR_COLUMN_COMBINATOR,
        NH_CSS_SELECTOR_PSEUDO_CLASS,
        NH_CSS_SELECTOR_PSEUDO_ELEMENT,
    } NH_CSS_SELECTOR;

/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct Nh_CSS_Selector {

        NH_CSS_SELECTOR type;

        struct Nh_CSS_Pseudo {
            NH_CSS_PSEUDO_CLASS _class;
            NH_CSS_PSEUDO_ELEMENT element;
        } Pseudo;

        char *string_p;
        Nh_List Parts;

    } Nh_CSS_Selector;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_BOOL Nh_CSS_createGenericProperty(
        Nh_HTML_Node *Node_p, char *selectorString_p, Nh_CSS_Selector *Selector_p
    );

    NH_BOOL Nh_CSS_selectorHit(
        Nh_HTML_Node *Node_p, Nh_CSS_Selector *Selector_p
    );

/** @} */

#endif 
