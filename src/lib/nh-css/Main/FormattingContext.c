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
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-gfx/Base/Viewport.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// NAMES ===========================================================================================

const char *nh_css_inPrimaryFormattingContextName(
    NH_CSS_FORMATTING_CONTEXT type)
{
NH_CSS_BEGIN()

    static const char *formattingContextNames_pp[] = 
    {
        "Block Formatting Context",
        "Inline Formatting Context",
    };

NH_CSS_END(formattingContextNames_pp[type])
}

// FORMATTING CONTEXT ==============================================================================

NH_API_RESULT nh_css_createFormattingContext(
    nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type)
{
NH_CSS_BEGIN()

    if (!Node_p->FormattingContexts_pp[0]) {
        Node_p->FormattingContexts_pp[0] = nh_core_allocate(sizeof(nh_css_FormattingContext));
        NH_CSS_CHECK_MEM(Node_p->FormattingContexts_pp[0])
        Node_p->FormattingContexts_pp[0]->type = type;
    }
    else if (!Node_p->FormattingContexts_pp[1]) {
        Node_p->FormattingContexts_pp[1] = nh_core_allocate(sizeof(nh_css_FormattingContext));
        NH_CSS_CHECK_MEM(Node_p->FormattingContexts_pp[1])
        Node_p->FormattingContexts_pp[1]->type = type;
    }
    else {NH_CSS_END(NH_API_ERROR_BAD_STATE)}

NH_CSS_END(NH_API_SUCCESS)
}

bool nh_css_startsFormattingContext(
    nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type)
{
NH_CSS_BEGIN()
    
    if (Node_p->FormattingContexts_pp[0] && Node_p->FormattingContexts_pp[0]->type == type) {
        NH_CSS_END(true)
    }
    else if (Node_p->FormattingContexts_pp[1] && Node_p->FormattingContexts_pp[1]->type == type) {
        NH_CSS_END(true)
    }

NH_CSS_END(false)
}

nh_css_BoxTreeNode *nh_css_inFormattingContext(
    nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type, int level)
{
NH_CSS_BEGIN()
    
    while (Node_p)
    {
        if (Node_p->FormattingContexts_pp[0] && Node_p->FormattingContexts_pp[0]->type == type) {
            NH_CSS_END(Node_p)
        }
        else if (Node_p->FormattingContexts_pp[1] && Node_p->FormattingContexts_pp[1]->type == type) {
            NH_CSS_END(Node_p)
        }
        else if (Node_p->FormattingContexts_pp[0] || Node_p->FormattingContexts_pp[1]) {
            if (level == 0) {NH_CSS_END(NULL)}
            level--;
        }

        Node_p = Node_p->Parent_p;
    }

NH_CSS_END(NULL)
}

nh_css_BoxTreeNode *nh_css_inPrimaryFormattingContext(
    nh_css_BoxTreeNode *Node_p, NH_CSS_FORMATTING_CONTEXT type)
{
NH_CSS_BEGIN()
NH_CSS_END(nh_css_inFormattingContext(Node_p, type, 0))
}

