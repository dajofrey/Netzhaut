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
#include "../../nh-core/Util/List.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_css_shiftLineBoxDownward(
    nh_css_Canvas *Canvas_p, nh_css_LineBox *LineBox_p)
{
    int shift = 10000;

    // get minimal vertical shift value;
    for (int i = 0; i < Canvas_p->Floats.size; ++i) {
        nh_css_Fragment *Float_p = Canvas_p->Floats.pp[i];
        nh_css_PixelBox FloatBox = nh_css_getMarginBox(Float_p);
        int upperY = FloatBox.Position.y;
        int lowerY = upperY + FloatBox.Size.height;
        // check for vertical overlap;
        if (upperY < LineBox_p->Position.y && lowerY > LineBox_p->Position.y
        ||  upperY < LineBox_p->Position.y + LineBox_p->Size.height && lowerY > LineBox_p->Position.y + LineBox_p->Size.height
        ||  upperY == LineBox_p->Position.y || lowerY == LineBox_p->Position.y + LineBox_p->Size.height) {
            if (shift > FloatBox.Size.height) {shift = FloatBox.Size.height;}
        }
    }

    LineBox_p->RootInline_p->ContentBox.Position.y += shift;
    LineBox_p->Position.y = LineBox_p->RootInline_p->ContentBox.Position.y;
    LineBox_p->RootInline_p->ContentBox.Position.x = LineBox_p->RootInline_p->Parent_p->ContentBox.Position.x;
    LineBox_p->currentX = LineBox_p->RootInline_p->ContentBox.Position.x;
    LineBox_p->Position.x = LineBox_p->currentX;
    LineBox_p->Size.width = LineBox_p->RootInline_p->Parent_p->ContentBox.Size.width;
    LineBox_p->RootInline_p->ContentBox.Size.width = 0;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_offsetLineBoxFromFloats(
    nh_css_Canvas *Canvas_p, nh_css_LineBox *LineBox_p)
{
    for (int i = 0; i < Canvas_p->Floats.size; ++i) {
        nh_css_Fragment *Float_p = Canvas_p->Floats.pp[i];
        nh_css_PixelBox FloatBox = nh_css_getMarginBox(Float_p);
        int lowerY = FloatBox.Position.y + FloatBox.Size.height;
        // check for vertical overlap
        if (FloatBox.Position.y < LineBox_p->Position.y && lowerY > LineBox_p->Position.y
        ||  FloatBox.Position.y < LineBox_p->Position.y + LineBox_p->Size.height && lowerY > LineBox_p->Position.y + LineBox_p->Size.height
        ||  FloatBox.Position.y == LineBox_p->Position.y || lowerY == LineBox_p->Position.y + LineBox_p->Size.height) {
            // check for horizontal overlap
            if (FloatBox.Position.x + FloatBox.Size.width >= LineBox_p->Position.x) {
                LineBox_p->shortened = true;
                LineBox_p->Size.width -= (FloatBox.Position.x + FloatBox.Size.width) - LineBox_p->Position.x;
                LineBox_p->Position.x = FloatBox.Position.x + FloatBox.Size.width;
                LineBox_p->currentX = LineBox_p->Position.x;
                LineBox_p->RootInline_p->ContentBox.Size.width = 0;
                LineBox_p->RootInline_p->ContentBox.Position.x = LineBox_p->Position.x;
            }
        }
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_arrangeFloat(
    nh_css_Canvas *Canvas_p, nh_css_Fragment *Fragment_p)
{
    nh_css_PixelBox CurrentBox = nh_css_getMarginBox(Fragment_p);

    for (int i = 0; i < Canvas_p->Floats.size; ++i) {
        nh_css_Fragment *Float_p = Canvas_p->Floats.pp[i];
        nh_css_PixelBox FloatBox = nh_css_getMarginBox(Float_p);
        // check for vertical overlap;
        if (FloatBox.Position.y < CurrentBox.Position.y && FloatBox.Position.y + FloatBox.Size.height > CurrentBox.Position.y
        ||  FloatBox.Position.y < CurrentBox.Position.y + CurrentBox.Size.height && FloatBox.Position.y + FloatBox.Size.height > CurrentBox.Position.y + CurrentBox.Size.height
        ||  FloatBox.Position.y == CurrentBox.Position.y) {
            // check for horizontal overlap;
            if (FloatBox.Position.x + FloatBox.Size.width > CurrentBox.Position.x) {
                // check if there is sufficient space next to float;
                if (FloatBox.Position.x + FloatBox.Size.width + CurrentBox.Size.width > Fragment_p->Parent_p->ContentBox.Position.x + Fragment_p->Parent_p->ContentBox.Size.width) {
                    // move below float and rerun;
                    Fragment_p->ContentBox.Position.y = FloatBox.Position.y + FloatBox.Size.height;
                    Fragment_p->ContentBox.Position.x = Fragment_p->Parent_p->ContentBox.Position.x;
                    CurrentBox = nh_css_getMarginBox(Fragment_p);
                    i = -1;
                }
                else {
                    // move to right of float;
                    Fragment_p->ContentBox.Position.x = FloatBox.Position.x + FloatBox.Size.width;
                }
            }
        }
    }

    nh_core_appendToList(&Canvas_p->Floats, Fragment_p);

    return NH_API_SUCCESS;
}
