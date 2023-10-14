#ifndef NH_CSS_BOX_TREE_H
#define NH_CSS_BOX_TREE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "SourceTree.h"

#include "../Layout/BoxModel.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_enums
 *  @{
 */

    typedef enum NH_CSS_DISPLAY {
        NH_CSS_DISPLAY_FLOW,
        NH_CSS_DISPLAY_FLOW_ROOT,
        NH_CSS_DISPLAY_TABLE,
        NH_CSS_DISPLAY_FLEX,
        NH_CSS_DISPLAY_GRID,
        NH_CSS_DISPLAY_RUBY,
        NH_CSS_DISPLAY_BLOCK,
        NH_CSS_DISPLAY_INLINE,
        NH_CSS_DISPLAY_LIST_ITEM,
    } NH_CSS_DISPLAY;

    typedef enum NH_CSS_BOX_TREE_NODE {
        NH_CSS_BOX_TREE_NODE_UNDEFINED,
        NH_CSS_BOX_TREE_NODE_BLOCK_CONTAINER,
        NH_CSS_BOX_TREE_NODE_INLINE,
        NH_CSS_BOX_TREE_NODE_ROOT_INLINE,
        NH_CSS_BOX_TREE_NODE_TEXT_RUN,
    } NH_CSS_BOX_TREE_NODE;

    typedef enum NH_CSS_FORMATTING_CONTEXT {
        NH_CSS_FORMATTING_CONTEXT_BLOCK,
        NH_CSS_FORMATTING_CONTEXT_INLINE,
    } NH_CSS_FORMATTING_CONTEXT;

/** @} */

/** @addtogroup lib_nhcss_structs
 *  @{
 */

    typedef struct nh_css_FormattingContext {
        NH_CSS_FORMATTING_CONTEXT type;
    } nh_css_FormattingContext;

    typedef struct nh_css_BoxTreeNode {
        NH_CSS_BOX_TREE_NODE type;
        nh_css_FormattingContext *FormattingContexts_pp[2];
        nh_css_Source *Source_p;
        nh_List Children;
        struct nh_css_BoxTreeNode *Parent_p;
    } nh_css_BoxTreeNode;

    typedef struct nh_css_BoxTree {
        nh_css_BoxTreeNode *Root_p;
    } nh_css_BoxTree;

/** @} */

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    const NH_BYTE *nh_css_getBoxTreeNodeName(
        NH_CSS_BOX_TREE_NODE type
    );

    nh_css_BoxTree nh_css_createBoxTree(
        nh_css_Source *Source_p
    );
    
/** @} */

#endif
