// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "BoxModel.h"

#include "../Common/Macros.h"

#include "../../nh-gfx/Base/Viewport.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

// CLIP BOX ========================================================================================

#define CLIP_LENGTH(VALUE) (VALUE + 1.0f)
#define PIXEL_TO_CLIP(PIXEL, VIEWPORT, basedOnWidth)                                            \
(                                                                                             \
    basedOnWidth ? ((((float)PIXEL) / ((float)VIEWPORT->Settings.Size.width))) * 2.0f - 1.0f  \
                 : ((((float)PIXEL) / ((float)VIEWPORT->Settings.Size.height))) * 2.0f - 1.0f \
)

nh_css_ClipBox nh_css_convertToClipBox(
    nh_gfx_Viewport *Viewport_p, nh_css_PixelBox PixelBox)
{
NH_CSS_BEGIN()

    nh_css_ClipBox ClipBox = {0};

    ClipBox.Position.x = PIXEL_TO_CLIP(PixelBox.Position.x, Viewport_p, NH_TRUE);
    ClipBox.Position.y = PIXEL_TO_CLIP(PixelBox.Position.y, Viewport_p, NH_FALSE);

    ClipBox.Size.width  = CLIP_LENGTH(PIXEL_TO_CLIP(PixelBox.Size.width, Viewport_p, NH_TRUE));
    ClipBox.Size.height = CLIP_LENGTH(PIXEL_TO_CLIP(PixelBox.Size.height, Viewport_p, NH_FALSE));

    ClipBox.depth = PixelBox.depth;

NH_CSS_END(ClipBox)
}

// MAX =============================================================================================

NH_PIXEL nh_css_getMaxX(
    nh_css_PixelBox Box)
{
NH_CSS_BEGIN()
NH_CSS_END(Box.Position.x + Box.Size.width)
}

NH_PIXEL nh_css_getMaxY(
    nh_css_PixelBox Box)
{
NH_CSS_BEGIN()
NH_CSS_END(Box.Position.y + Box.Size.height)
}

// PIXEL BOX =======================================================================================

nh_css_PixelBox nh_css_getContentBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p)
{
NH_CSS_BEGIN()

    nh_css_PixelBox Box = nh_css_getPaddingBox(ContainingBlock_p, Values_p); 

    Box.Position.x  += Values_p->paddingLeft;
    Box.Position.y  += Values_p->paddingTop;
    Box.Size.width  -= Values_p->paddingRight + Values_p->paddingLeft;
    Box.Size.height -= Values_p->paddingBottom + Values_p->paddingTop;

NH_CSS_END(Box)
}

nh_css_PixelBox nh_css_getPaddingBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p)
{
NH_CSS_BEGIN()

    nh_css_PixelBox Box = nh_css_getBorderBox(ContainingBlock_p, Values_p);

    Box.Position.x += Values_p->borderLeft;
    Box.Position.y += Values_p->borderTop;
    Box.Size.width -= Values_p->borderRight + Values_p->borderLeft;
    Box.Size.height -= Values_p->borderBottom + Values_p->borderTop;

NH_CSS_END(Box)
}

nh_css_PixelBox nh_css_getBorderBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p)
{
NH_CSS_BEGIN()

    nh_css_PixelBox Box;

    Box.Position.x  = ContainingBlock_p->Position.x + Values_p->marginLeft;
    Box.Position.y  = ContainingBlock_p->Position.y + Values_p->marginTop;
    Box.Size.width  = ContainingBlock_p->Size.width - (Values_p->marginRight + Values_p->marginLeft);
    Box.Size.height = ContainingBlock_p->Size.height - (Values_p->marginBottom + Values_p->marginTop);

    Box.depth = ContainingBlock_p->depth;

NH_CSS_END(Box)
}

nh_css_PixelBox nh_css_getMarginBox(
    nh_css_PixelBox *ContentBox_p, nh_css_BoxValues *Values_p)
{
NH_CSS_BEGIN()

    nh_css_PixelBox Box;

    Box.Position.x  = ContentBox_p->Position.x - Values_p->marginLeft - Values_p->borderLeft - Values_p->paddingLeft;
    Box.Position.y  = ContentBox_p->Position.y - Values_p->marginTop - Values_p->borderTop - Values_p->paddingTop;
    Box.Size.width  = ContentBox_p->Size.width + Values_p->marginRight + Values_p->marginLeft + Values_p->paddingRight + Values_p->paddingLeft + Values_p->borderRight + Values_p->borderLeft;
    Box.Size.height = ContentBox_p->Size.height + Values_p->marginBottom + Values_p->marginTop + Values_p->paddingBottom + Values_p->paddingTop + Values_p->borderBottom + Values_p->borderTop;

    Box.depth = ContentBox_p->depth;

NH_CSS_END(Box)
}
