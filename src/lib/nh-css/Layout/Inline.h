#ifndef NH_CSS_INLINE_H
#define NH_CSS_INLINE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Main/Canvas.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    typedef struct nh_css_LineBox {
        bool shortened;
        nh_api_PixelSize Size;
        nh_api_PixelPosition Position;
        int currentX;
        nh_css_Fragment *RootInline_p;
    } nh_css_LineBox;

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    NH_API_RESULT nh_css_arrangeInlineFormattingContext(
        nh_css_Canvas *Canvas_p, nh_css_Fragment *Fragment_p, int yOffset, int *newFragments_p
    );

/** @} */

#endif
