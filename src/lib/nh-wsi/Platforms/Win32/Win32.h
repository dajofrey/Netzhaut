#ifndef NH_WSI_WIN32_H
#define NH_WSI_WIN32_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../../Common/Includes.h"

#if defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

#endif

typedef struct nh_wsi_WindowConfiguration nh_wsi_WindowConfiguration;

/** @addtogroup Structs
 *  @{
 */

    typedef struct nh_Win32_Window {

        int ignore;

    #if defined(_WIN32) || defined (WIN32)
        NHND hwnd;
        WNDCLASS wc;
        HINSTANCE hinstance;
    #endif

    } nh_Win32_Window;

/** @} */


/** @addtogroup Functions
 *  @{
 */

    NH_API_RESULT nh_Win32_getInput(
        nh_Win32_Window *Window_p
    ); 

    NH_API_RESULT nh_Win32_createWindow(
        nh_Win32_Window *Window_p, nh_wsi_WindowConfiguration *Configuration_p, 
        nh_gfx_SurfaceRequirements *Requirements_p
    );
    
    NH_API_RESULT nh_Win32_destroyWindow(
        nh_Win32_Window *Window_p
    );
    
    NH_API_RESULT nh_Win32_setWindowBackgroundColor(
        nh_Win32_Window *Window_p, nh_css_Color Color
    );
    
    NH_API_RESULT nh_Win32_setWindowTitle(
        nh_Win32_Window *Window_p, char *title_p
    );
    
    NH_API_RESULT nh_Win32_setMouseCursor(
        nh_Win32_Window *Window_p, int type
    );

/** @} */

#endif 
