#ifndef NH_CSS_FLOAT_H
#define NH_CSS_FLOAT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Inline.h"

#include "../Main/Canvas.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    NH_API_RESULT nh_css_shiftLineBoxDownward(
        nh_css_Canvas *Canvas_p, nh_css_LineBox *LineBox_p
    );

    NH_API_RESULT nh_css_offsetLineBoxFromFloats(
        nh_css_Canvas *Canvas_p, nh_css_LineBox *LineBox_p
    );

    NH_API_RESULT nh_css_arrangeFloat(
        nh_css_Canvas *Canvas_p, nh_css_Fragment *Fragment_p
    );

/** @} */

#endif
