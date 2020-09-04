/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_IO_X11_WINDOW_H
#define NH_IO_X11_WINDOW_H

#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/Thread.h"

#ifdef __unix__
    #include <wayland-server.h>
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <linux/input.h>
    #include <X11/Xlib-xcb.h>
    #include <xkbcommon/xkbcommon-x11.h>
    #include <sys/mman.h>
#endif

/** @addtogroup Structs
 *  @{
 */

    typedef struct NH_IO_X11_Window 
    {
        int ignore;

    #ifdef __unix__
        Display *Display_p;             
        Window XWindow;                 
        Atom DeleteAtom;                
        xcb_connection_t *Connection_p;
        int32_t keyboardDeviceID;      
        struct xkb_keymap *Keymap_p;    
        struct xkb_state *State_p;      
        struct xkb_context *Context_p;  
    #endif

    } NH_IO_X11_Window;

/** @} */

/** @addtogroup Functions
 *  @{
 */

    NH_RESULT NH_IO_X11_createWindow(
        char *title, int width, int height, NH_IO_X11_Window *Window_p
    );
    
    NH_RESULT NH_IO_X11_destroyWindow(
        NH_IO_X11_Window *Window_p
    );
    
    NH_RESULT NH_IO_X11_setWindowBackgroundColor(
        NH_IO_X11_Window *Window_p, float color_p[4]
    );
    
    NH_RESULT NH_IO_X11_setWindowTitle(
        NH_IO_X11_Window *Window_p, char *title_p
    );
    
    NH_RESULT NH_IO_X11_setMouseCursor(
        NH_IO_X11_Window *Window_p, char *title_p
    );

/** @} */

#endif 
