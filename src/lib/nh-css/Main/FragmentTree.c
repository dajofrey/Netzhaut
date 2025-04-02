// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "FragmentTree.h"
#include "Canvas.h"

#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// FRAGMENT ========================================================================================

nh_css_Fragment nh_css_initFragment(
    nh_css_BoxTreeNode *Node_p, nh_css_Fragment *Parent_p, NH_CSS_FRAGMENT type, nh_api_CanvasType *Type_p)
{
    nh_css_Fragment Fragment;
    memset(&Fragment, 0 , sizeof(nh_css_Fragment));

    Fragment.type     = type;
    Fragment.Node_p   = Node_p;
    Fragment.Parent_p = Parent_p;
    Fragment.Children = nh_core_initList(8);
    Fragment.data_p   = NULL;

    if (!Node_p->Parent_p && !Fragment.Parent_p && Type_p) {
        Fragment.ContentBox.Size.width  = Type_p->Size.width;
        Fragment.ContentBox.Size.height = Type_p->Size.height;
    }

    return Fragment;
}

static nh_css_Fragment *nh_css_insertFragment(
    nh_css_BoxTreeNode *Node_p, nh_css_Fragment *Parent_p, NH_CSS_FRAGMENT type, nh_api_CanvasType *Type_p)
{
    nh_css_Fragment *Fragment_p = nh_core_allocate(sizeof(nh_css_Fragment));
    NH_CORE_CHECK_MEM_2(NULL, Fragment_p)

    *Fragment_p = nh_css_initFragment(Node_p, Parent_p, type, Type_p);

    if (Parent_p) {nh_core_appendToList(&Parent_p->Children, Fragment_p);}

    return Fragment_p;
}

static nh_css_Fragment *nh_css_createFragment(
    nh_css_BoxTreeNode *Node_p, nh_css_Fragment *Parent_p, nh_api_CanvasType *Type_p)
{
    nh_css_Fragment *Fragment_p = NULL;

    if (Node_p->type != NH_CSS_BOX_TREE_NODE_TEXT_RUN) {
        Fragment_p = nh_css_insertFragment(Node_p, Parent_p, NH_CSS_FRAGMENT_BOX, Type_p);
        NH_CORE_CHECK_MEM_2(NULL, Fragment_p)
    }
    else {
        Fragment_p = nh_css_insertFragment(Node_p, Parent_p, NH_CSS_FRAGMENT_TEXT, Type_p);
        NH_CORE_CHECK_MEM_2(NULL, Fragment_p)
        Fragment_p->Text.p = Node_p->Source_p->TextNode.String.p;
        Fragment_p->Text.length = Node_p->Source_p->TextNode.String.length;
    }

    for (int i = 0; i < Node_p->Children.size; ++i) {
        NH_CORE_CHECK_NULL_2(NULL, nh_css_createFragment(Node_p->Children.pp[i], Fragment_p, Type_p))
    }

    return Fragment_p;
}

// BOX TREE ========================================================================================

nh_css_FragmentTree nh_css_createFragmentTree(
    nh_css_BoxTreeNode *Node_p, nh_api_CanvasType *Type_p)
{
    nh_css_FragmentTree Tree;
    Tree.Root_p = nh_css_createFragment(Node_p, NULL, Type_p);
    return Tree;
}
