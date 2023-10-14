#ifndef NH_CSS_BLOCK_H
#define NH_CSS_BLOCK_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Main/Canvas.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_functions
 *  @{
 */

//    NH_CSS_RESULT nh_css_collapse(
//        nh_css_BoxFragment *Fragment_p
//    );

    NH_CSS_RESULT nh_css_arrangeBlockFormattingContext(
        nh_css_Canvas *Canvas_p, nh_css_Fragment *Fragment_p, NH_PIXEL *advance_p 
    );

/** @} */

#endif
