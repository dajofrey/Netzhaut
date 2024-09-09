// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Float.h"
#include "Block.h"
#include "Inline.h"
#include "LayoutValues.h"
#include "Float.h"

#include "../Main/FormattingContext.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// LINE BOXES ======================================================================================

NH_CSS_RESULT nh_css_shiftLineBoxDownward(
    nh_css_Canvas *Canvas_p, nh_css_LineBox *LineBox_p)
{
NH_CSS_BEGIN()

    NH_PIXEL shift = 10000;

    // get minimal vertical shift value;
    for (int i = 0; i < Canvas_p->Floats.size; ++i) {
        nh_css_Fragment *Float_p = Canvas_p->Floats.pp[i];
        nh_css_PixelBox FloatBox = nh_css_getMarginBox(&Float_p->Block, &Float_p->Box.Values);
        NH_PIXEL upperY = FloatBox.Position.y;
        NH_PIXEL lowerY = upperY + FloatBox.Size.height;
        // check for vertical overlap;
        if (upperY < LineBox_p->Position.y && lowerY > LineBox_p->Position.y
        ||  upperY < LineBox_p->Position.y + LineBox_p->Size.height && lowerY > LineBox_p->Position.y + LineBox_p->Size.height
        ||  upperY == LineBox_p->Position.y || lowerY == LineBox_p->Position.y + LineBox_p->Size.height) {
            if (shift > FloatBox.Size.height) {shift = FloatBox.Size.height;}
        }
    }

    LineBox_p->RootInline_p->Block.Position.y += shift;
    LineBox_p->Position.y = LineBox_p->RootInline_p->Block.Position.y;
    LineBox_p->RootInline_p->Block.Position.x = LineBox_p->RootInline_p->Parent_p->Block.Position.x;
    LineBox_p->currentX = LineBox_p->RootInline_p->Block.Position.x;
    LineBox_p->Position.x = LineBox_p->currentX;
    LineBox_p->Size.width = LineBox_p->RootInline_p->Parent_p->Block.Size.width;
    LineBox_p->RootInline_p->Block.Size.width = 0;

NH_CSS_END(NH_CSS_SUCCESS)
}

NH_CSS_RESULT nh_css_offsetLineBoxFromFloats(
    nh_css_Canvas *Canvas_p, nh_css_LineBox *LineBox_p)
{
NH_CSS_BEGIN()

    for (int i = 0; i < Canvas_p->Floats.size; ++i) {
        nh_css_Fragment *Float_p = Canvas_p->Floats.pp[i];
        nh_css_PixelBox FloatBox = nh_css_getMarginBox(&Float_p->Block, &Float_p->Box.Values);
        NH_PIXEL lowerY = FloatBox.Position.y + FloatBox.Size.height;
        // check for vertical overlap
        if (FloatBox.Position.y < LineBox_p->Position.y && lowerY > LineBox_p->Position.y
        ||  FloatBox.Position.y < LineBox_p->Position.y + LineBox_p->Size.height && lowerY > LineBox_p->Position.y + LineBox_p->Size.height
        ||  FloatBox.Position.y == LineBox_p->Position.y || lowerY == LineBox_p->Position.y + LineBox_p->Size.height) {
            // check for horizontal overlap
            if (FloatBox.Position.x + FloatBox.Size.width >= LineBox_p->Position.x) {
                LineBox_p->shortened = NH_TRUE;
                LineBox_p->Size.width -= (FloatBox.Position.x + FloatBox.Size.width) - LineBox_p->Position.x;
                LineBox_p->Position.x = FloatBox.Position.x + FloatBox.Size.width;
                LineBox_p->currentX = LineBox_p->Position.x;
                LineBox_p->RootInline_p->Block.Size.width = 0;
                LineBox_p->RootInline_p->Block.Position.x = LineBox_p->Position.x;
            }
        }
    }

NH_CSS_END(NH_CSS_SUCCESS)
}

// FLOAT ===========================================================================================

NH_CSS_RESULT nh_css_arrangeFloat(
    nh_css_Canvas *Canvas_p, nh_css_Fragment *Fragment_p)
{
NH_CSS_BEGIN()

    nh_css_PixelBox CurrentBox = nh_css_getMarginBox(&Fragment_p->Block, &Fragment_p->Box.Values);

    for (int i = 0; i < Canvas_p->Floats.size; ++i) {
        nh_css_Fragment *Float_p = Canvas_p->Floats.pp[i];
        nh_css_PixelBox FloatBox = nh_css_getMarginBox(&Float_p->Block, &Float_p->Box.Values);
        // check for vertical overlap;
        if (FloatBox.Position.y < CurrentBox.Position.y && FloatBox.Position.y + FloatBox.Size.height > CurrentBox.Position.y
        ||  FloatBox.Position.y < CurrentBox.Position.y + CurrentBox.Size.height && FloatBox.Position.y + FloatBox.Size.height > CurrentBox.Position.y + CurrentBox.Size.height
        ||  FloatBox.Position.y == CurrentBox.Position.y) {
            // check for horizontal overlap;
            if (FloatBox.Position.x + FloatBox.Size.width > CurrentBox.Position.x) {
                // check if there is sufficient space next to float;
                if (FloatBox.Position.x + FloatBox.Size.width + CurrentBox.Size.width > Fragment_p->Parent_p->Block.Position.x + Fragment_p->Parent_p->Block.Size.width) {
                    // move below float and rerun;
                    Fragment_p->Block.Position.y = FloatBox.Position.y + FloatBox.Size.height;
                    Fragment_p->Block.Position.x = Fragment_p->Parent_p->Block.Position.x;
                    CurrentBox = nh_css_getMarginBox(&Fragment_p->Block, &Fragment_p->Box.Values);
                    i = -1;
                }
                else {
                    // move to right of float;
                    Fragment_p->Block.Position.x = FloatBox.Position.x + FloatBox.Size.width;
                }
            }
        }
    }

    nh_core_appendToList(&Canvas_p->Floats, Fragment_p);

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}