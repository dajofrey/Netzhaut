#ifndef NH_TTY_SIDEBAR_H
#define NH_TTY_SIDEBAR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    NH_TTY_RESULT nh_tty_handleSideBarHit(
        NH_WSI_MOUSE_E mouse, int row
    );

    NH_TTY_RESULT nh_tty_drawSideBarRow(
        nh_tty_Glyph *Glyphs_p, int row
    );

/** @} */

#endif 
