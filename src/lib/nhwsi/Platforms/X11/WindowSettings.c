// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "WindowSettings.h"
#include "Window.h"

#include "../../Window/Window.h"
#include "../../Window/Event.h"

#include "../../Common/Log.h"
#include "../../Common/Macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#ifdef __unix__
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xutil.h>
#endif

// WINDOW SETTINGS =================================================================================
// Functions for setting X11 window properties.

NH_WSI_RESULT_E nh_x11_setWindowBackgroundColor(
    nh_x11_Window *Window_p, nh_Color Color)
{
NH_WSI_BEGIN()

#ifdef __unix__

    uint16_t rgba_p[4] = {
        (uint16_t)Color.r*255,
        (uint16_t)Color.g*255,
        (uint16_t)Color.b*255,
        (uint16_t)Color.a*255,
    };

    long double color = 0x00000000; // == black
    color = rgba_p[0] << 16 | rgba_p[1] << 8 | rgba_p[2] << 0 | rgba_p[3] << 24; 

    XSetWindowAttributes Attributes;
    Attributes.background_pixel = color;
    XChangeWindowAttributes(NH_WSI_X11.Display_p, Window_p->Handle, CWBackPixel, &Attributes);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

NH_WSI_RESULT_E nh_x11_setWindowTitle(
    nh_x11_Window *Window_p, NH_BYTE *title_p)
{
NH_WSI_BEGIN()

#ifdef __unix__

    XStoreName(NH_WSI_X11.Display_p, Window_p->Handle, title_p);
    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

/**
 * Credit goes to https://github.com/glfw/glfw 
 */
NH_WSI_RESULT_E nh_x11_setWindowDecorated(
    nh_x11_Window *Window_p, NH_BOOL decorated)
{
NH_WSI_BEGIN()

#ifdef __unix__

    XUnmapWindow(NH_WSI_X11.Display_p, Window_p->Handle);

    struct
    {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long input_mode;
        unsigned long status;
    } hints = {0};
    
    hints.flags = 2;
    hints.decorations = decorated;
 
    XChangeProperty(NH_WSI_X11.Display_p, Window_p->Handle,
                    NH_WSI_X11.Atoms.MOTIF_WM_HINTS,
                    NH_WSI_X11.Atoms.MOTIF_WM_HINTS, 32,
                    PropModeReplace,
                    (unsigned char*) &hints,
                    sizeof(hints) / sizeof(long));

    XMapWindow(NH_WSI_X11.Display_p, Window_p->Handle);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

/**
 * Related resources:
 * - https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html
 */
NH_WSI_RESULT_E nh_x11_setWindowState(
    nh_x11_Window *Window_p, NH_BOOL *state_p)
{
NH_WSI_BEGIN()

#ifdef __unix__

    XUnmapWindow(NH_WSI_X11.Display_p, Window_p->Handle);

    if (NH_WSI_X11.Atoms.NET_WM_STATE == 0) {
        NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

    Atom states_p[3];
    int count = 0;

    if (state_p[NH_WSI_WINDOW_STATE_ABOVE]) {
        if (NH_WSI_X11.Atoms.NET_WM_STATE_ABOVE)
            states_p[count++] = NH_WSI_X11.Atoms.NET_WM_STATE_ABOVE;
    }

    if (state_p[NH_WSI_WINDOW_STATE_MAXIMIZED]) {
        // Changing wm state is not working on my machine. 
        // So lets do it the stupid way.

        Atom net_workarea = XInternAtom(NH_WSI_X11.Display_p, "_NET_WORKAREA", False);
        Atom actual_type;
        int actual_format;
        unsigned long nitems, bytes_after;
        unsigned char *prop_value;
    
        if (XGetWindowProperty(NH_WSI_X11.Display_p, NH_WSI_X11.root, net_workarea, 0, 4, False, XA_CARDINAL,
                               &actual_type, &actual_format, &nitems, &bytes_after, &prop_value) != Success) {
            NH_WSI_END(NH_WSI_ERROR_BAD_STATE)
        }

        if (actual_format == 32 && nitems == 4) {
            long *workarea = (long *)prop_value;
            int max_width = workarea[2];
            int max_height = workarea[3];
            nh_x11_getWindowSize(Window_p, &Window_p->oldX, &Window_p->oldY);
            XResizeWindow(NH_WSI_X11.Display_p, Window_p->Handle, max_width, max_height);
        } else {
            NH_WSI_END(NH_WSI_ERROR_BAD_STATE)
        }
    }
    else {
        XResizeWindow(NH_WSI_X11.Display_p, Window_p->Handle, Window_p->oldX, Window_p->oldY);
    }

    if (count) {
        XChangeProperty(NH_WSI_X11.Display_p, Window_p->Handle, NH_WSI_X11.Atoms.NET_WM_STATE,
                        XA_ATOM, 32, PropModeReplace, (unsigned char*)states_p, count);
    }

    XMapWindow(NH_WSI_X11.Display_p, Window_p->Handle);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

/**
 * Related resources:
 * - https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html
 */
NH_WSI_RESULT_E nh_x11_setWindowType(
    nh_x11_Window *Window_p, NH_WSI_WINDOW_TYPE_E type)
{
NH_WSI_BEGIN()

#ifdef __unix__

    XUnmapWindow(NH_WSI_X11.Display_p, Window_p->Handle);

    switch (type) {
        case NH_WSI_WINDOW_TYPE_NORMAL :
            if (NH_WSI_X11.Atoms.NET_WM_WINDOW_TYPE && NH_WSI_X11.Atoms.NET_WM_WINDOW_TYPE_NORMAL) {
                Atom type = NH_WSI_X11.Atoms.NET_WM_WINDOW_TYPE_NORMAL;
                XChangeProperty(NH_WSI_X11.Display_p, Window_p->Handle, NH_WSI_X11.Atoms.NET_WM_WINDOW_TYPE,
                                XA_ATOM, 32, PropModeReplace, (unsigned char*) &type, 1);
            }
            break;
    }

    XMapWindow(NH_WSI_X11.Display_p, Window_p->Handle);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

/**
 * Related resources: 
 * - https://github.com/freedesktop/xorg-lib-libX11/blob/master/include/X11/cursorfont.h
 */ 
NH_WSI_RESULT_E nh_x11_setMouseCursor(
    nh_x11_Window *Window_p, NH_WSI_CURSOR_E type)
{
NH_WSI_BEGIN()

#ifdef __unix__

    Cursor C;

    switch (type)
    {
        case NH_WSI_CURSOR_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "arrow"); break;
        case NH_WSI_CURSOR_ARROW_DOWN : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "based_arrow_down"); break;
        case NH_WSI_CURSOR_ARROW_UP : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "based_arrow_up"); break;
        case NH_WSI_CURSOR_BOAT : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "boat"); break;
        case NH_WSI_CURSOR_BOGOSITY : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "bogosity"); break;
        case NH_WSI_CURSOR_BOTTOM_LEFT_CORNER : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "bottom_left_corner"); break;
        case NH_WSI_CURSOR_BOTTOM_RIGHT_CORNER : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "bottom_right_corner"); break;
        case NH_WSI_CURSOR_BOTTOM_SIDE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "bottom_side"); break;
        case NH_WSI_CURSOR_BOTTOM_TEE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "bottom_tee"); break;
        case NH_WSI_CURSOR_BOX_SPIRAL : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "box_spiral"); break;
        case NH_WSI_CURSOR_CENTER_PTR : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "center_ptr"); break;
        case NH_WSI_CURSOR_CIRCLE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "circle"); break;
        case NH_WSI_CURSOR_CLOCK : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "clock"); break;
        case NH_WSI_CURSOR_COFFEE_MUG : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "coffee_mug"); break;
        case NH_WSI_CURSOR_CROSS : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "cross"); break;
        case NH_WSI_CURSOR_CROSS_REVERSE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "cross_reverse"); break;
        case NH_WSI_CURSOR_CROSSHAIR : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "crosshair"); break;
        case NH_WSI_CURSOR_DIAMOND_CROSS : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "diamond_cross"); break;
        case NH_WSI_CURSOR_DOT : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "dot"); break;
        case NH_WSI_CURSOR_DOTBOX : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "dotbox"); break;
        case NH_WSI_CURSOR_DOUBLE_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "double_arrow"); break;
        case NH_WSI_CURSOR_DRAFT_LARGE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "draft_large"); break;
        case NH_WSI_CURSOR_DRAFT_SMALL : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "draft_small"); break;
        case NH_WSI_CURSOR_DRAPED_BOX : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "draped_box"); break;
        case NH_WSI_CURSOR_EXCHANGE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "exchange"); break;
        case NH_WSI_CURSOR_FLEUR : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "fleur"); break;
        case NH_WSI_CURSOR_GOBBLER : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "gobbler"); break;
        case NH_WSI_CURSOR_GUMBY : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "gumby"); break;
        case NH_WSI_CURSOR_HAND1 : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "hand1"); break;
        case NH_WSI_CURSOR_HAND2 : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "hand2"); break;
        case NH_WSI_CURSOR_HEART : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "heart"); break;
        case NH_WSI_CURSOR_ICON : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "icon"); break;
        case NH_WSI_CURSOR_IRON_CROSS : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "iron_cross"); break;
        case NH_WSI_CURSOR_LEFT_PTR : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "left_ptr"); break;
        case NH_WSI_CURSOR_LEFT_SIDE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "left_side"); break;
        case NH_WSI_CURSOR_LEFT_TEE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "left_tee"); break;
        case NH_WSI_CURSOR_LEFT_BUTTON : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "leftbutton"); break;
        case NH_WSI_CURSOR_LL_ANGLE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "ll_angle"); break;
        case NH_WSI_CURSOR_LR_ANGLE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "lr_angle"); break;
        case NH_WSI_CURSOR_MAN : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "man"); break;
        case NH_WSI_CURSOR_MIDDLEBUTTON : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "middlebutton"); break;
        case NH_WSI_CURSOR_MOUSE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "mouse"); break;
        case NH_WSI_CURSOR_PENCIL : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "pencil"); break;
        case NH_WSI_CURSOR_PIRATE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "pirate"); break;
        case NH_WSI_CURSOR_PLUS : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "plus"); break;
        case NH_WSI_CURSOR_QUESTION_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "question_arrow"); break;
        case NH_WSI_CURSOR_RIGHT_PTR : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "right_ptr"); break;
        case NH_WSI_CURSOR_RIGHT_SIDE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "right_side"); break;
        case NH_WSI_CURSOR_RIGHT_TEE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "right_tee"); break;
        case NH_WSI_CURSOR_RIGHT_BUTTON : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "rightbutton"); break;
        case NH_WSI_CURSOR_RTL_LOGO : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "rtl_logo"); break;
        case NH_WSI_CURSOR_SAILBOAT : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "sailboat"); break;
        case NH_WSI_CURSOR_SB_DOWN_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "sb_down_arrow"); break;
        case NH_WSI_CURSOR_SB_H_DOUBLE_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "sb_h_double_arrow"); break;
        case NH_WSI_CURSOR_SB_LEFT_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "sb_left_arrow"); break;
        case NH_WSI_CURSOR_SB_RIGHT_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "sb_right_arrow"); break;
        case NH_WSI_CURSOR_SB_UP_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "sb_up_arrow"); break;
        case NH_WSI_CURSOR_SB_V_DOUBLE_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "sb_v_double_arrow"); break;
        case NH_WSI_CURSOR_SHUTTLE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "shuttle"); break;
        case NH_WSI_CURSOR_SIZING : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "sizing"); break;
        case NH_WSI_CURSOR_SPIDER : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "spider"); break;
        case NH_WSI_CURSOR_SPRAYCAN : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "spraycan"); break;
        case NH_WSI_CURSOR_STAR : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "star"); break;
        case NH_WSI_CURSOR_TARGET : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "target"); break;
        case NH_WSI_CURSOR_TCROSS : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "tcross"); break;
        case NH_WSI_CURSOR_TOP_LEFT_ARROW : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "top_left_arrow"); break;
        case NH_WSI_CURSOR_TOP_LEFT_CORNER : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "top_left_corner"); break;
        case NH_WSI_CURSOR_TOP_RIGHT_CORNER : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "top_right_corner"); break;
        case NH_WSI_CURSOR_TOP_SIDE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "top_side"); break;
        case NH_WSI_CURSOR_TOP_TEE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "top_tee"); break;
        case NH_WSI_CURSOR_TREK : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "trek"); break;
        case NH_WSI_CURSOR_UL_ANGLE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "ul_angle"); break;
        case NH_WSI_CURSOR_UMBRELLA : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "umbrella"); break;
        case NH_WSI_CURSOR_UR_ANGLE : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "ur_angle"); break;
        case NH_WSI_CURSOR_WATCH : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "watch"); break;
        case NH_WSI_CURSOR_XTERM : C = XcursorLibraryLoadCursor(NH_WSI_X11.Display_p, "xterm"); break;
    }

    XDefineCursor(NH_WSI_X11.Display_p, Window_p->Handle, C);

    NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)

#endif

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

