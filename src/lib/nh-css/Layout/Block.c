// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Block.h"
#include "Inline.h"
#include "LayoutValues.h"
#include "Float.h"

#include "../Main/FormattingContext.h"
#include "../Common/Log.h"

#include "../../nh-core/Util/List.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

/**
 * Margin collapse.
 */
NH_API_RESULT nh_css_collapse(
    nh_css_Fragment *Fragment_p, int *advance_p)
{
    // first child
    if (Fragment_p->type == NH_CSS_FRAGMENT_BOX && Fragment_p->Parent_p->Children.pp[0] == Fragment_p) {
        if (Fragment_p->Parent_p->Box.Values.marginTop > Fragment_p->Box.Values.marginTop) {
            Fragment_p->Box.Values.marginTop = Fragment_p->Parent_p->Box.Values.marginTop;
        }
        Fragment_p->Parent_p->Box.Values.marginTop = 0;
    }

//    // last child
//    if (Parent_p->Children.pp[Parent_p->Children.size - 1] == Fragment_p->Box_p->Node_p && nh_css_noBottomSeparation(Fragment_p, Parent_p)) {
//        if (Parent_p->Values.marginBottom < Fragment_p->Values.marginBottom) {
//            Parent_p->Values.marginBottom = Fragment_p->Values.marginBottom;
//        }
//        Fragment_p->Values.marginBottom = 0;
//    }

    // previous sibling
    int fragIndex = nh_core_getListIndex(&Fragment_p->Parent_p->Children, Fragment_p);
    if (fragIndex > 0) {
        nh_css_Fragment *Previous_p = Fragment_p->Parent_p->Children.pp[fragIndex-1];
        if (Previous_p->Box.Values.marginBottom > Fragment_p->Box.Values.marginTop) {
            Fragment_p->Box.Values.marginTop = Previous_p->Box.Values.marginBottom;
        }
        *advance_p -= Previous_p->Box.Values.marginBottom;
        Previous_p->Box.Values.marginBottom = 0;
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_arrangeBlockFormattingContext(
    nh_css_Canvas *Canvas_p, nh_css_Fragment *Fragment_p, int *advance_p)
{
    NH_CORE_CHECK(nh_css_computeBoxValues(Fragment_p))

    if (Fragment_p->Parent_p) {
        NH_CORE_CHECK(nh_css_collapse(Fragment_p, advance_p))
        Fragment_p->Block = nh_css_getContentBox(&Fragment_p->Parent_p->Block, &Fragment_p->Box.Values);
        Fragment_p->Block.Position.y += *advance_p;
        Fragment_p->Block.Size.height = 0;
    }

    if (Fragment_p->Box.Values.Width.type == NH_CSS_BOX_SIZING_PERCENTAGE) {
        Fragment_p->Block.Size.width = (int)((float)Fragment_p->Block.Size.width * (Fragment_p->Box.Values.Width.percentage/100.0f));
    }
    else if (Fragment_p->Box.Values.Width.type == NH_CSS_BOX_SIZING_LENGTH) {
        Fragment_p->Block.Size.width = Fragment_p->Box.Values.Width.value;
    }

    if (Fragment_p->Box.Values._float != NH_CSS_FLOAT_NONE) {
        nh_css_arrangeFloat(Canvas_p, Fragment_p);
        Canvas_p->_float = true;
    }

    for (int i = 0, advance = 0; i < Fragment_p->Children.size; ++i) 
    {
        nh_css_Fragment *Child_p = Fragment_p->Children.pp[i];
        int newFragments = 0;

        if (nh_css_startsFormattingContext(Child_p->Node_p, NH_CSS_FORMATTING_CONTEXT_INLINE)) {
            NH_CORE_CHECK(nh_css_arrangeInlineFormattingContext(Canvas_p, Child_p, advance, &newFragments))
        }
        else {
            NH_CORE_CHECK(nh_css_arrangeBlockFormattingContext(Canvas_p, Child_p, &advance))
        }

        if (Child_p->Box.Values._float != NH_CSS_FLOAT_NONE) {continue;}

        if (Child_p->Block.Size.height == 0) {
            Child_p->Box.Values.marginTop = 0;
            Child_p->Box.Values.marginBottom = 0;
        }

        for (int j = 0; j < (newFragments + 1); ++j) 
        {
            Child_p = Fragment_p->Children.pp[i + j];
            nh_css_PixelBox ChildMarginBox = nh_css_getMarginBox(&Child_p->Block, &Child_p->Box.Values);

            if (ChildMarginBox.Position.y == (Fragment_p->Block.Position.y + advance)) {
                // the child did not move its y axis position; advance by its height;
                advance += ChildMarginBox.Size.height;
                if (advance > Fragment_p->Block.Size.height) {Fragment_p->Block.Size.height = advance;}
            }
            else {
                // the child moved its y axis position; advance by its height and position offset;
                advance += (ChildMarginBox.Position.y + ChildMarginBox.Size.height) - (Fragment_p->Block.Position.y + Fragment_p->Block.Size.height);
                if (advance > Fragment_p->Block.Size.height) {Fragment_p->Block.Size.height = advance;}
            }
        }

        i += newFragments;
    }

    Canvas_p->_float = false;

    return NH_API_SUCCESS;
}
