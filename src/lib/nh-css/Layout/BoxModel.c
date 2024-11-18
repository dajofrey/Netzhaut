// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "BoxModel.h"

#include "../Main/FragmentTree.h"
#include "../../nh-gfx/Base/Viewport.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

// DEFINES =========================================================================================

#define CLIP_LENGTH(VALUE) (VALUE + 1.0f)
#define PIXEL_TO_CLIP(PIXEL, VIEWPORT, basedOnWidth)                                            \
(                                                                                             \
    basedOnWidth ? ((((float)PIXEL) / ((float)VIEWPORT->Settings.Size.width))) * 2.0f - 1.0f  \
                 : ((((float)PIXEL) / ((float)VIEWPORT->Settings.Size.height))) * 2.0f - 1.0f \
)

// FUNCTIONS =========================================================================================

nh_css_ClipBox nh_css_convertToClipBox(
    void *Viewport_p, nh_css_PixelBox PixelBox)
{
    nh_css_ClipBox ClipBox = {0};

    ClipBox.Position.x = PIXEL_TO_CLIP(PixelBox.Position.x, ((nh_gfx_Viewport*)Viewport_p), true);
    ClipBox.Position.y = PIXEL_TO_CLIP(PixelBox.Position.y, ((nh_gfx_Viewport*)Viewport_p), false);

    ClipBox.Size.width  = CLIP_LENGTH(PIXEL_TO_CLIP(PixelBox.Size.width, ((nh_gfx_Viewport*)Viewport_p), true));
    ClipBox.Size.height = CLIP_LENGTH(PIXEL_TO_CLIP(PixelBox.Size.height, ((nh_gfx_Viewport*)Viewport_p), false));

    ClipBox.depth = PixelBox.depth;

    return ClipBox;
}

int nh_css_getMaxX(
    nh_css_PixelBox Box)
{
    return Box.Position.x + Box.Size.width;
}

int nh_css_getMaxY(
    nh_css_PixelBox Box)
{
    return Box.Position.y + Box.Size.height;
}

void nh_css_updateContentBoxPosition(
    nh_css_Fragment *Fragment_p)
{
    nh_css_PixelBox ContentBox = nh_css_getContentBox(&Fragment_p->Parent_p->ContentBox, &Fragment_p->Box.Values); 

    Fragment_p->ContentBox.Position.x = ContentBox.Position.x;
    Fragment_p->ContentBox.Position.y = ContentBox.Position.y;
}

nh_css_PixelBox nh_css_getContentBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p)
{
    nh_css_PixelBox Box = nh_css_getPaddingBox(ContainingBlock_p, Values_p); 

    Box.Position.x  += Values_p->paddingLeft;
    Box.Position.y  += Values_p->paddingTop;
    Box.Size.width  -= Values_p->paddingRight + Values_p->paddingLeft;
    Box.Size.height -= Values_p->paddingBottom + Values_p->paddingTop;

    return Box;
}

nh_css_PixelBox nh_css_getPaddingBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p)
{
    nh_css_PixelBox Box = nh_css_getBorderBox(ContainingBlock_p, Values_p);

    Box.Position.x += Values_p->borderLeft;
    Box.Position.y += Values_p->borderTop;
    Box.Size.width -= Values_p->borderRight + Values_p->borderLeft;
    Box.Size.height -= Values_p->borderBottom + Values_p->borderTop;

    return Box;
}

nh_css_PixelBox nh_css_getBorderBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p)
{
    nh_css_PixelBox Box;

    Box.Position.x  = ContainingBlock_p->Position.x + Values_p->MarginLeft.value;
    Box.Position.y  = ContainingBlock_p->Position.y + Values_p->marginTop;
    Box.Size.width  = ContainingBlock_p->Size.width - (Values_p->MarginRight.value + Values_p->MarginLeft.value);
    Box.Size.height = ContainingBlock_p->Size.height - (Values_p->marginBottom + Values_p->marginTop);

    Box.depth = ContainingBlock_p->depth;

    return Box;
}

nh_css_PixelBox nh_css_getMarginBox(
    nh_css_Fragment *Fragment_p)
{
    nh_css_PixelBox Box;

    Box.Position.x  = Fragment_p->ContentBox.Position.x - Fragment_p->Box.Values.MarginLeft.value - Fragment_p->Box.Values.borderLeft - Fragment_p->Box.Values.paddingLeft;
    Box.Position.y  = Fragment_p->ContentBox.Position.y - Fragment_p->Box.Values.marginTop - Fragment_p->Box.Values.borderTop - Fragment_p->Box.Values.paddingTop;
    Box.Size.width  = Fragment_p->ContentBox.Size.width + Fragment_p->Box.Values.MarginRight.value + Fragment_p->Box.Values.MarginLeft.value + Fragment_p->Box.Values.paddingRight + Fragment_p->Box.Values.paddingLeft + Fragment_p->Box.Values.borderRight + Fragment_p->Box.Values.borderLeft;
    Box.Size.height = Fragment_p->ContentBox.Size.height + Fragment_p->Box.Values.marginBottom + Fragment_p->Box.Values.marginTop + Fragment_p->Box.Values.paddingBottom + Fragment_p->Box.Values.paddingTop + Fragment_p->Box.Values.borderBottom + Fragment_p->Box.Values.borderTop;

    Box.depth = Fragment_p->ContentBox.depth;

    return Box;
}
