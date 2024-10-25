// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "BoxTree.h"
#include "FormattingContext.h"

#include "../Layout/Block.h"
#include "../Layout/Depth.h"
#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-gfx/Base/Viewport.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

const char *nh_css_getBoxTreeNodeName(
    NH_CSS_BOX_TREE_NODE type)
{
    static const char *names_pp[] = 
    {
        "Undefined",
        "Block Container Box",
        "Inline Box",
        "Root Inline Box",
        "Text Run",
    };

    return names_pp[type];
}

static NH_CSS_DISPLAY nh_css_getOuterDisplayType(
    nh_css_Source *Source_p)
{
    nh_css_Value *Value_p = Source_p->ComputedValues_p->pp[NH_CSS_PROPERTY_DISPLAY];

    if (Value_p->Common.type != NH_CSS_VALUE_KEYWORD) {
        return NH_CSS_DISPLAY_BLOCK;
    }

    if (!strcmp(Value_p->String.p, "block") && !Value_p->Common.Next_p) {
        return NH_CSS_DISPLAY_BLOCK;
    }
    else if (!strcmp(Value_p->String.p, "inline") && !Value_p->Common.Next_p) {
        return NH_CSS_DISPLAY_INLINE;
    }

    return NH_CSS_DISPLAY_BLOCK;
}

static NH_CSS_DISPLAY nh_css_getInnerDisplayType(
    nh_css_Source *Source_p)
{
    nh_css_Value *Value_p = Source_p->ComputedValues_p->pp[NH_CSS_PROPERTY_DISPLAY];

    if (Value_p->Common.type != NH_CSS_VALUE_KEYWORD) {
        return NH_CSS_DISPLAY_FLOW;
    }

    if (!strcmp(Value_p->String.p, "block") && !Value_p->Common.Next_p) {
        return NH_CSS_DISPLAY_FLOW;
    }
    else if (!strcmp(Value_p->String.p, "inline") && !Value_p->Common.Next_p) {
        return NH_CSS_DISPLAY_FLOW;
    }

    return NH_CSS_DISPLAY_FLOW;
}

static bool nh_css_isInlineContent(
    nh_css_Source *Source_p)
{
    if (Source_p->type == NH_CSS_SOURCE_ELEMENT) {
        if (nh_css_getOuterDisplayType(Source_p) == NH_CSS_DISPLAY_INLINE) {
            return true;
        }
    }
    else {return true;}

    return false;
}

static bool nh_css_hasMixedContent(
    nh_css_Source *Source_p)
{
    NH_CSS_DISPLAY outer;

    if (Source_p->Element.Children.size) {
        if (((nh_css_Source*)Source_p->Element.Children.pp[0])->type == NH_CSS_SOURCE_ELEMENT) {
            outer = nh_css_getOuterDisplayType(Source_p->Element.Children.pp[0]);
        }
        else {outer = NH_CSS_DISPLAY_INLINE;}
    }

    for (int i = 1; i < Source_p->Element.Children.size; ++i) 
    {
        if (((nh_css_Source*)Source_p->Element.Children.pp[i])->type == NH_CSS_SOURCE_ELEMENT) {
            if (outer != nh_css_getOuterDisplayType(Source_p->Element.Children.pp[i])) {
                return true;
            }
        }
        else if (outer != NH_CSS_DISPLAY_INLINE) {return true;}
    }

    return false;
}

static NH_CSS_BOX_TREE_NODE nh_css_getBoxType(
    nh_css_BoxTreeNode *Node_p)
{
    NH_CSS_BOX_TREE_NODE type;

    if (Node_p->Source_p->type == NH_CSS_SOURCE_TEXT_NODE) {
        type = NH_CSS_BOX_TREE_NODE_INLINE;
    }
    else {
        NH_CSS_DISPLAY outerDisplay = nh_css_getOuterDisplayType(Node_p->Source_p);
        NH_CSS_DISPLAY innerDisplay = nh_css_getInnerDisplayType(Node_p->Source_p);
    
        if (innerDisplay == NH_CSS_DISPLAY_FLOW) {
            if (outerDisplay != NH_CSS_DISPLAY_BLOCK && (nh_css_inPrimaryFormattingContext(Node_p->Parent_p, NH_CSS_FORMATTING_CONTEXT_BLOCK) || nh_css_inPrimaryFormattingContext(Node_p->Parent_p, NH_CSS_FORMATTING_CONTEXT_INLINE))) {
                type = NH_CSS_BOX_TREE_NODE_INLINE;
            }
            else {
                type = NH_CSS_BOX_TREE_NODE_BLOCK_CONTAINER;
            }
        }
    }

    return type;
}

static nh_css_BoxTreeNode *nh_css_insertBoxTreeNode(
    nh_css_Source *Source_p, nh_css_BoxTreeNode *Parent_p)
{
    nh_css_BoxTreeNode *Node_p = nh_core_allocate(sizeof(nh_css_BoxTreeNode));
    NH_CORE_CHECK_MEM_2(NULL, Node_p)

    Node_p->FormattingContexts_pp[0] = NULL;
    Node_p->FormattingContexts_pp[1] = NULL;

    Node_p->Parent_p = Parent_p;
    Node_p->Source_p = Source_p;
    Node_p->Children = nh_core_initList(8);

    if (Parent_p) {nh_core_appendToList(&Parent_p->Children, Node_p);}

    return Node_p;
}

static void nh_css_removeBoxTreeNode(
    nh_css_BoxTreeNode *Node_p)
{
    nh_core_removeFromList2(&Node_p->Parent_p->Children, true, Node_p);

    return;
}

static nh_css_BoxTreeNode *nh_css_createRootInlineBox(
    nh_css_BoxTreeNode *Parent_p)
{
    nh_css_BoxTreeNode *Node_p = nh_css_insertBoxTreeNode(NULL, Parent_p);
    NH_CORE_CHECK_NULL_2(NULL, Node_p)

    Node_p->type = NH_CSS_BOX_TREE_NODE_ROOT_INLINE;
    NH_CORE_CHECK_2(NULL, nh_css_createFormattingContext(Node_p, NH_CSS_FORMATTING_CONTEXT_INLINE))

    return Node_p;
}

static nh_css_BoxTreeNode *nh_css_createAnonymousBlockBox(
    nh_css_BoxTreeNode *Parent_p)
{
    nh_css_BoxTreeNode *Node_p = nh_css_insertBoxTreeNode(NULL, Parent_p);
    NH_CORE_CHECK_NULL_2(NULL, Node_p)

    Node_p->type = NH_CSS_BOX_TREE_NODE_BLOCK_CONTAINER;
    NH_CORE_CHECK_2(NULL, nh_css_createFormattingContext(Node_p, NH_CSS_FORMATTING_CONTEXT_BLOCK))

    return Node_p;
}

static NH_API_RESULT nh_css_createTextRun(
    nh_css_Source *Source_p, nh_css_BoxTreeNode *Parent_p)
{
    nh_css_BoxTreeNode *Node_p = nh_css_insertBoxTreeNode(Source_p, Parent_p);
    NH_CORE_CHECK_MEM(Node_p)
    Node_p->type = NH_CSS_BOX_TREE_NODE_TEXT_RUN;
    Source_p->mark = true;

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_css_createInlineBox(
    nh_css_Source *Source_p, nh_css_BoxTreeNode *Parent_p, nh_css_Source **Hoist_pp)
{
    if (Source_p->mark) {return NH_API_SUCCESS;}
    if (Source_p->type == NH_CSS_SOURCE_TEXT_NODE) {
        return nh_css_createTextRun(Source_p, Parent_p);
    }

    nh_css_BoxTreeNode *Node_p = nh_css_insertBoxTreeNode(Source_p, Parent_p);
    NH_CORE_CHECK_NULL(Node_p)

    Node_p->type = nh_css_getBoxType(Node_p);
    if (Node_p->type != NH_CSS_BOX_TREE_NODE_INLINE) {
        nh_css_removeBoxTreeNode(Node_p);
        *Hoist_pp = Source_p;
        return NH_API_SUCCESS;
    }

    for (int i = 0; i < Source_p->Element.Children.size; ++i) {
        NH_CORE_CHECK(nh_css_createInlineBox(Source_p->Element.Children.pp[i], Node_p, Hoist_pp))
        if (*Hoist_pp) {return NH_API_SUCCESS;}
    }

    Source_p->mark = true;

    return NH_API_SUCCESS;
}

static nh_css_BoxTreeNode *nh_css_createBlockBox(
    nh_css_Source *Source_p, nh_css_BoxTreeNode *Parent_p
);

static NH_API_RESULT nh_css_hoist(
    nh_css_Source *Source_p, nh_css_BoxTreeNode **BlockBox_pp, nh_css_BoxTreeNode **RootInlineBox_pp)
{
    nh_css_Source *Hoist_p = NULL;
    do {
        Hoist_p = NULL;
        NH_CORE_CHECK(nh_css_createInlineBox(Source_p, *RootInlineBox_pp, &Hoist_p))
        if (Hoist_p) {
            nh_css_createBlockBox(Hoist_p, (*BlockBox_pp)->Parent_p);
            *BlockBox_pp = nh_css_insertBoxTreeNode((*BlockBox_pp)->Source_p, (*BlockBox_pp)->Parent_p);
            NH_CORE_CHECK_NULL(*BlockBox_pp)
            (*BlockBox_pp)->type = NH_CSS_BOX_TREE_NODE_BLOCK_CONTAINER;
            *RootInlineBox_pp = nh_css_createRootInlineBox(*BlockBox_pp);
        }
    }
    while (Hoist_p);

    return NH_API_SUCCESS;
}

static nh_css_BoxTreeNode *nh_css_createBlockBox(
    nh_css_Source *Source_p, nh_css_BoxTreeNode *Parent_p)
{
    nh_css_BoxTreeNode *Node_p = nh_css_insertBoxTreeNode(Source_p, Parent_p);
    NH_CORE_CHECK_NULL_2(NULL, Node_p)

    Node_p->type = NH_CSS_BOX_TREE_NODE_BLOCK_CONTAINER;
    NH_CORE_CHECK_2(NULL, nh_css_createFormattingContext(Node_p, NH_CSS_FORMATTING_CONTEXT_BLOCK))

    bool mixed = nh_css_hasMixedContent(Source_p);

    nh_css_BoxTreeNode *BlockBox_p = NULL;
    nh_css_BoxTreeNode *RootInlineBox_p = NULL;

    for (int i = 0; i < Source_p->Element.Children.size; ++i) 
    {
        if (!mixed) {BlockBox_p = Node_p;}
        if (nh_css_isInlineContent(Source_p->Element.Children.pp[i])) {
            if (!BlockBox_p) {
                BlockBox_p = nh_css_createAnonymousBlockBox(Node_p);
            }
            if (!RootInlineBox_p) {
                RootInlineBox_p = nh_css_createRootInlineBox(BlockBox_p);
            }
            NH_CORE_CHECK_2(NULL, nh_css_hoist(Source_p->Element.Children.pp[i], &BlockBox_p, &RootInlineBox_p))
        }
        else {
            nh_css_createBlockBox(Source_p->Element.Children.pp[i], Node_p);
            BlockBox_p = NULL;
            RootInlineBox_p = NULL;
        }
    }

    Source_p->mark = true;

    return Node_p;
}

nh_css_BoxTree nh_css_createBoxTree(
    nh_css_Source *Source_p)
{
    nh_css_BoxTree Tree;
    Tree.Root_p = nh_css_createBlockBox(Source_p, NULL);

    return Tree;
}
