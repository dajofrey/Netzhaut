#ifndef NH_CSS_LAYOUT_BOX_MODEL_H
#define NH_CSS_LAYOUT_BOX_MODEL_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "LayoutValues.h"
#include "../Common/Includes.h"

// STRUCTS =========================================================================================

typedef struct nh_css_ClipPosition { 
    float x; 
    float y; 
} nh_css_ClipPosition; 

typedef struct nh_css_ClipSize { 
    float width; 
    float height; 
} nh_css_ClipSize; 

typedef struct nh_css_PixelBox {
    nh_api_PixelPosition Position;
    nh_api_PixelSize Size;
    float depth;
} nh_css_PixelBox;

typedef struct nh_css_ClipBox {
    nh_css_ClipPosition Position;
    nh_css_ClipSize Size;
    float depth;
} nh_css_ClipBox;

// FUNCTIONS =======================================================================================

nh_css_ClipBox nh_css_convertToClipBox(
    void *Viewport_p, nh_css_PixelBox PixelBox
);

int nh_css_getMaxX(
    nh_css_PixelBox Box
);

int nh_css_getMaxY(
    nh_css_PixelBox Box
);

void nh_css_updateContentBoxPosition(
    nh_css_Fragment *Fragment_p
);

nh_css_PixelBox nh_css_getContentBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p
);

nh_css_PixelBox nh_css_getPaddingBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p
);

nh_css_PixelBox nh_css_getBorderBox(
    nh_css_PixelBox *ContainingBlock_p, nh_css_BoxValues *Values_p
);

nh_css_PixelBox nh_css_getMarginBox(
    nh_css_Fragment *Fragment_p
);

#endif // NH_CSS_LAYOUT_BOX_MODEL_H
