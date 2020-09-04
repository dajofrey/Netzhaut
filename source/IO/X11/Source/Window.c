// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Window.h"
#include "../Header/Macros.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __unix__
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xutil.h>
#endif

// CREATE ==========================================================================================

NH_RESULT NH_IO_X11_createWindow(
    char *title, int width, int height, NH_IO_X11_Window *Window_p)
{
NH_BEGIN()

#ifdef __unix__

    Window_p->Display_p  = XOpenDisplay(NULL);
    Display *Display_p = Window_p->Display_p;
    NH_CHECK_NULL(Display_p)

// keyboard
    Window_p->Connection_p = XGetXCBConnection(Display_p);
    Window_p->Context_p    = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    xkb_x11_setup_xkb_extension(
        Window_p->Connection_p, XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, 
        0, NULL, NULL, NULL, NULL
    ); 
    Window_p->keyboardDeviceID = xkb_x11_get_core_keyboard_device_id(Window_p->Connection_p);
    Window_p->Keymap_p = xkb_x11_keymap_new_from_device(
        Window_p->Context_p, 
        Window_p->Connection_p, 
        Window_p->keyboardDeviceID, 
        XKB_KEYMAP_COMPILE_NO_FLAGS
    );
    Window_p->State_p = xkb_x11_state_new_from_device(
        Window_p->Keymap_p, Window_p->Connection_p, Window_p->keyboardDeviceID
    );

// RGBA color for transparency
    XVisualInfo vinfo;
    XMatchVisualInfo(Display_p, DefaultScreen(Display_p), 32, TrueColor, &vinfo);

// transparent window
    XSetWindowAttributes Attributes;
    Attributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask | ExposureMask;
    Attributes.border_pixel = 0;
    Attributes.background_pixel = 0xff000000; // ARGB
    Attributes.colormap = XCreateColormap(Display_p, DefaultRootWindow(Display_p), vinfo.visual, AllocNone);

    Window_p->XWindow = XCreateWindow(
        Display_p, DefaultRootWindow(Display_p), 0, 0, width, height, 0,
        vinfo.depth, InputOutput, vinfo.visual, CWEventMask | CWBackPixel | CWBorderPixel | CWColormap, 
        &Attributes
    );
    
    XMapWindow(Display_p, Window_p->XWindow);
    
// window delete protocol
    Window_p->DeleteAtom = XInternAtom(Display_p, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(Display_p, Window_p->XWindow, &Window_p->DeleteAtom, 1);

// xdnd (file-)drop
    Atom xdndAware = XInternAtom(Display_p, "XdndAware", 0); 
    Atom xdndVersion = 5;
    XChangeProperty(
        Display_p, Window_p->XWindow, xdndAware, XA_ATOM, 32,
        PropModeReplace, (unsigned char*)&xdndVersion, 1
    );

// window name   
    XStoreName(Display_p, Window_p->XWindow, title);

// center window
    Screen *Screen_p = ScreenOfDisplay(Display_p, DefaultScreen(Display_p));
    XMoveWindow(Display_p, Window_p->XWindow, (int)(Screen_p->width/2) - (int)(width/2), (int)(Screen_p->height/2) - (int)(height/2));
    XSync(Display_p, false);

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

// DESTROY =========================================================================================

NH_RESULT NH_IO_X11_destroyWindow(
    NH_IO_X11_Window *Window_p)
{
NH_BEGIN()

#ifdef __unix__

    XDestroyWindow(Window_p->Display_p, Window_p->XWindow);
    XCloseDisplay(Window_p->Display_p);
    xkb_state_unref(Window_p->State_p);
    xkb_keymap_unref(Window_p->Keymap_p);
    xkb_context_unref(Window_p->Context_p);

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

// SET =============================================================================================

NH_RESULT NH_IO_X11_setWindowBackgroundColor(
    NH_IO_X11_Window *Window_p, float color_p[4])
{
NH_BEGIN()

#ifdef __unix__

    uint16_t rgba_p[4];
    for (int i = 0; i < 4; ++i) {rgba_p[i] = (uint16_t)(color_p[i]*255);}
    long double color = 0x00000000; // == black
    color = rgba_p[0] << 16 | rgba_p[1] << 8 | rgba_p[2] << 0 | rgba_p[3] << 24; 
    XSetWindowAttributes Attributes;
    Attributes.background_pixel = color;
    XChangeWindowAttributes(Window_p->Display_p, Window_p->XWindow, CWBackPixel, &Attributes);

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

NH_RESULT NH_IO_X11_setWindowTitle(
    NH_IO_X11_Window *Window_p, char *title_p)
{
NH_BEGIN()

#ifdef __unix__

    XStoreName(Window_p->Display_p, Window_p->XWindow, title_p);

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

NH_RESULT NH_IO_X11_setMouseCursor(
    NH_IO_X11_Window *Window_p, char *title_p)
{
NH_BEGIN()

#ifdef __unix__

    Cursor C = XcursorLibraryLoadCursor(Window_p->Display_p, "sb_v_double_arrow");
    XDefineCursor (Window_p->Display_p, Window_p->XWindow, C);

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

