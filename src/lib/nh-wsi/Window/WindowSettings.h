#ifndef NH_WSI_WINDOW_WINDOW_SETTINGS_H
#define NH_WSI_WINDOW_WINDOW_SETTINGS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-wsi_typedefs
 *  @{
 */

    typedef NH_WSI_RESULT_E (*nh_wsi_toggleWindowSize_f)(
        nh_wsi_Window *Window_p
    );

/** @} */

/** @addtogroup lib_nh-wsi_functions
 *  @{
 */
    
    NH_WSI_RESULT_E nh_wsi_configureWindow(
        nh_wsi_Window *Window_p
    );

    NH_WSI_RESULT_E nh_wsi_toggleWindowSize(
        nh_wsi_Window *Window_p
    );

    NH_WSI_RESULT_E nh_wsi_setMouseCursor(
        nh_wsi_Window *Window_p, NH_WSI_CURSOR_E cursor
    );

/** @} */

#endif // NH_WSI_WINDOW_WINDOW_SETTINGS_H 
