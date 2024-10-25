#ifndef NH_CSS_LAYOUT_BLOCK_H
#define NH_CSS_LAYOUT_BLOCK_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Main/Canvas.h"
#include "../Common/Includes.h"

// FUNCTIONS =======================================================================================

//    NH_API_RESULT nh_css_collapse(
//        nh_css_BoxFragment *Fragment_p
//    );

    NH_API_RESULT nh_css_arrangeBlockFormattingContext(
        nh_css_Canvas *Canvas_p, nh_css_Fragment *Fragment_p, int *advance_p 
    );

#endif // NH_CSS_LAYOUT_BLOCK_H
