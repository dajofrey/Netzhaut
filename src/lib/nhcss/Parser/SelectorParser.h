#ifndef NH_CSS_SELECTOR_PARSER_H
#define NH_CSS_SELECTOR_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Tokenizer.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_enums
 *  @{
 */

    typedef enum NH_CSS_SELECTOR_PARSE_NODE {
        NH_CSS_SELECTOR_PARSE_NODE_TERMINAL,
        NH_CSS_SELECTOR_PARSE_NODE_SELECTOR_LIST,
        NH_CSS_SELECTOR_PARSE_NODE_COMPLEX_SELECTOR_LIST,
        NH_CSS_SELECTOR_PARSE_NODE_COMPOUND_SELECTOR_LIST,
        NH_CSS_SELECTOR_PARSE_NODE_SIMPLE_SELECTOR_LIST,
        NH_CSS_SELECTOR_PARSE_NODE_RELATIVE_SELECTOR_LIST,
        NH_CSS_SELECTOR_PARSE_NODE_COMPLEX_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_RELATIVE_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_COMPOUND_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_SIMPLE_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_COMBINATOR,
        NH_CSS_SELECTOR_PARSE_NODE_TYPE_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_NS_PREFIX,
        NH_CSS_SELECTOR_PARSE_NODE_WQ_NAME,
        NH_CSS_SELECTOR_PARSE_NODE_SUBCLASS_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_ID_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_CLASS_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_ATTRIBUTE_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_ATTR_MATCHER,
        NH_CSS_SELECTOR_PARSE_NODE_ATTR_MODIFIER,
        NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_CLASS_SELECTOR,
        NH_CSS_SELECTOR_PARSE_NODE_PSEUDO_ELEMENT_SELECTOR,
    } NH_CSS_SELECTOR_PARSE_NODE;

/** @} */

/** @addtogroup lib_nhcss_structs
 *  @{
 */

    typedef struct nh_css_SelectorParseNode {
        NH_CSS_SELECTOR_PARSE_NODE type;
        nh_css_Token *Token_p;
        nh_List Children;
    } nh_css_SelectorParseNode;

/** @} */

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    const NH_BYTE *nh_css_getSelectorParseNodeName(
        nh_css_SelectorParseNode *Node_p
    );

    nh_css_SelectorParseNode *nh_css_parseSelectorList(
        nh_Array ComponentValues
    );

/** @} */

#endif
