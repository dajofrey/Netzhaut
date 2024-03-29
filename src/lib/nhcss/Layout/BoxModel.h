#ifndef NH_CSS_BOX_MODEL_H
#define NH_CSS_BOX_MODEL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "LayoutValues.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_structs
 *  @{
 */

    typedef struct nh_css_PixelBox {
        nh_PixelPosition Position;
        nh_PixelSize Size;
        float depth;
    } nh_css_PixelBox;

    typedef struct nh_css_ClipBox {
        nh_ClipPosition Position;
        nh_ClipSize Size;
        float depth;
    } nh_css_ClipBox;

/** @} */

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    nh_css_ClipBox nh_css_convertToClipBox(
        nh_gfx_Viewport *Viewport_p, nh_css_PixelBox PixelBox
    );

    NH_PIXEL nh_css_getMaxX(
        nh_css_PixelBox Box
    );
    
    NH_PIXEL nh_css_getMaxY(
        nh_css_PixelBox Box
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
        nh_css_PixelBox *ContentBox_p, nh_css_BoxValues *Values_p
    );

/** @} */

#endif