/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_IO_WIN_WINDOW_H
#define NH_IO_WIN_WINDOW_H

#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/Thread.h"

#if defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

/** @addtogroup Structs
 *  @{
 */

    typedef struct NH_IO_Win_Window 
    {
        int ignore;

    #if defined(_WIN32) || defined (WIN32)
        NHND hwnd;
        WNDCLASS wc;
        HINSTANCE hinstance;
    #endif

    } NH_IO_Win_Window;

/** @} */


/** @addtogroup Functions
 *  @{
 */

    NH_RESULT NH_IO_Win_createWindow(
        char *title, int width, int height, NH_IO_Win_Window *Window_p
    );
    
    NH_RESULT NH_IO_Win_destroyWindow(
        NH_IO_Win_Window *Window_p
    );
    
    NH_RESULT NH_IO_Win_setWindowBackgroundColor(
        NH_IO_Win_Window *Window_p, float color_p[4]
    );
    
    NH_RESULT NH_IO_Win_setWindowTitle(
        NH_IO_Win_Window *Window_p, char *title_p
    );
    
    NH_RESULT NH_IO_Win_setMouseCursor(
        NH_IO_Win_Window *Window_p, char *title_p
    );

/** @} */

#endif 
