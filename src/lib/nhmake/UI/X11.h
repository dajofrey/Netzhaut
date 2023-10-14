#ifndef NH_MAKE_X11_H
#define NH_MAKE_X11_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Result.h"

#ifdef __unix__
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <linux/input.h>
    #include <X11/Xlib-xcb.h>
    #include <xkbcommon/xkbcommon-x11.h>
    #include <sys/mman.h>
#endif

#endif

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef struct nh_make_X11 {

    #ifdef __unix__
        int screen;
        XFontStruct *Font_p;
        GC Gfx;
        Display *Display_p;
        Window Root;
        Window Window;
        Atom WM_DELETE_WINDOW;
        xcb_connection_t *Connection_p;
        int32_t keyboardDeviceID;
        struct xkb_keymap *Keymap_p;
        struct xkb_state *State_p;
        struct xkb_context *Context_p;
        unsigned long black, white;
        int width, height;
    #endif

    } nh_make_X11;

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    NH_MAKE_RESULT nh_make_X11_createWindow(
        char *title, int width, int heigh
    );
    
    NH_MAKE_RESULT nh_make_X11_destroyWindow(
    );
 
    NH_MAKE_RESULT nh_make_X11_getInput(
    );

    void nh_make_X11_drawTextLine(
        int x, int y, char *text_p
    );

    void nh_make_X11_drawLine(
       int x1, int y1, int x2, int y2
    );
 
    void nh_make_X11_fillRectangle(
       int x1, int y1, int x2, int y2
    );

    void nh_make_X11_clearWindow(
    );

/** @} */

#endif 
