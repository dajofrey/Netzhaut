// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

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

const char *nh_css_inPrimaryFormattingContextName(
    NH_CSS_FORMATTING_CONTEXT type)
{
    static const char *formattingContextNames_pp[] = 
    {
        "Block Formatting Context",
        "Inline Formatting Context",
    };

    return formattingContextNames_pp[type];
}

NH_API_RESULT nh_css_createFormattingContext(
    nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type)
{
    if (!Node_p->FormattingContexts_pp[0]) {
        Node_p->FormattingContexts_pp[0] = nh_core_allocate(sizeof(nh_css_FormattingContext));
        NH_CORE_CHECK_MEM(Node_p->FormattingContexts_pp[0])
        Node_p->FormattingContexts_pp[0]->type = type;
    }
    else if (!Node_p->FormattingContexts_pp[1]) {
        Node_p->FormattingContexts_pp[1] = nh_core_allocate(sizeof(nh_css_FormattingContext));
        NH_CORE_CHECK_MEM(Node_p->FormattingContexts_pp[1])
        Node_p->FormattingContexts_pp[1]->type = type;
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

bool nh_css_startsFormattingContext(
    nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type)
{
    if (Node_p->FormattingContexts_pp[0] && Node_p->FormattingContexts_pp[0]->type == type) {
        return true;
    }
    else if (Node_p->FormattingContexts_pp[1] && Node_p->FormattingContexts_pp[1]->type == type) {
        return true;
    }

    return false;
}

nh_css_BoxTreeNode *nh_css_inFormattingContext(
    nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type, int level)
{
    while (Node_p)
    {
        if (Node_p->FormattingContexts_pp[0] && Node_p->FormattingContexts_pp[0]->type == type) {
            return Node_p;
        }
        else if (Node_p->FormattingContexts_pp[1] && Node_p->FormattingContexts_pp[1]->type == type) {
            return Node_p;
        }
        else if (Node_p->FormattingContexts_pp[0] || Node_p->FormattingContexts_pp[1]) {
            if (level == 0) {return NULL;}
            level--;
        }

        Node_p = Node_p->Parent_p;
    }

    return NULL;
}

nh_css_BoxTreeNode *nh_css_inPrimaryFormattingContext(
    nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type)
{
    return nh_css_inFormattingContext(Node_p, type, 0);
}

