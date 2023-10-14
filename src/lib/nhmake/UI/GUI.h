#ifndef NH_MAKE_UI_GUI_H
#define NH_MAKE_UI_GUI_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhmake_vars
 *  @{
 */

    extern NH_MAKE_BOOL NH_MAKE_GUI;

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    NH_MAKE_RESULT nh_make_runGUI(
    );

    NH_MAKE_RESULT nh_make_handleGUIExpose(
    );

    void nh_make_handleGUIExit(
    );

/** @} */

#endif // NH_MAKE_UI_GUI_H
