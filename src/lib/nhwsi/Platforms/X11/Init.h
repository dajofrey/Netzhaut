#ifndef NH_WSI_PLATFORMS_X11_INIT_H
#define NH_WSI_PLATFORMS_X11_INIT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../../Common/Includes.h"

#ifdef __unix__
    #include <wayland-server.h>
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <X11/extensions/Xrender.h>
    #include <linux/input.h>
    #include <X11/Xlib-xcb.h>
    #include <xkbcommon/xkbcommon-x11.h>
    #include <sys/mman.h>
#endif

#endif

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct nh_x11_Atoms {
        // String format atoms
        Atom NULL_;
        Atom UTF8_STRING;
        Atom STRING;
        Atom TEXT;
        Atom ATOM_PAIR;
        // Custom selection property atom
        Atom GLFW_SELECTION;
        // ICCCM standard clipboard atoms
        Atom PENGUIN;
        Atom TARGETS;
        Atom MULTIPLE;
        Atom PRIMARY;
        Atom INCR;
        Atom CLIPBOARD;
        // Clipboard manager atoms
        Atom CLIPBOARD_MANAGER;
        Atom SAVE_TARGETS;
        // Xdnd (drag and drop) atoms
        Atom XdndAware;
        Atom XdndEnter;
        Atom XdndLeave;
        Atom XdndPosition;
        Atom XdndStatus;
        Atom XdndActionCopy;
        Atom XdndDrop;
        Atom XdndFinished;
        Atom XdndSelection;
        Atom XdndTypeList;
        Atom text_uri_list;
        // 
        Atom WM_PROTOCOLS;
        Atom WM_STATE;
        Atom WM_DELETE_WINDOW;
        Atom NET_SUPPORTED;
        Atom NET_SUPPORTING_WM_CHECK;
        Atom NET_WM_ICON;
        Atom NET_WM_PING;
        Atom NET_WM_PID;
        Atom NET_WM_NAME;
        Atom NET_WM_ICON_NAME;
        Atom NET_WM_BYPASS_COMPOSITOR;
        Atom NET_WM_WINDOW_OPACITY;
        Atom MOTIF_WM_HINTS;
        Atom NET_WM_CM_Sx;
        // Following atoms need WM support check.
        Atom NET_WM_STATE;
        Atom NET_WM_STATE_REMOVE;
        Atom NET_WM_STATE_ADD;
        Atom NET_WM_STATE_TOGGLE;
        Atom NET_WM_STATE_ABOVE;
        Atom NET_WM_STATE_FULLSCREEN;
        Atom NET_WM_STATE_MAXIMIZED_VERT;
        Atom NET_WM_STATE_MAXIMIZED_HORZ;
        Atom NET_WM_STATE_DEMANDS_ATTENTION;
        Atom NET_WM_FULLSCREEN_MONITORS;
        Atom NET_WM_WINDOW_TYPE;
        Atom NET_WM_WINDOW_TYPE_NORMAL;
        Atom NET_WM_MOVERESIZE;
        Atom NET_WORKAREA;
        Atom NET_CURRENT_DESKTOP;
        Atom NET_ACTIVE_WINDOW;
        Atom NET_FRAME_EXTENTS;
        Atom NET_REQUEST_FRAME_EXTENTS;
    } nh_x11_Atoms;

    typedef struct nh_x11_XKB {
        xcb_connection_t *Connection_p;
        int32_t keyboardDeviceID;      
        struct xkb_keymap *Keymap_p;    
        struct xkb_state *State_p;      
        struct xkb_context *Context_p;  
    } nh_x11_XKB;

    typedef struct nh_wsi_X11 {
        nh_x11_Atoms Atoms; /**<Property identifiers.*/
        nh_x11_XKB XKB;        /**<XKB.*/
        Display *Display_p;    /**<X server connection.*/
        int screen;            /**<Screen.*/
        Window root;           /**<Root.*/
    } nh_wsi_X11;

/** @} */

/** @addtogroup lib_nhwsi_vars
 *  @{
 */

    extern nh_wsi_X11 NH_WSI_X11;

/** @} */

/** @addtogroup IOFunctions Function
 *  \ingroup IO
 *  @{
 */

    NH_WSI_RESULT_E nh_x11_initialize(
    );

    NH_WSI_RESULT_E nh_x11_close(
    );

/** @} */

#endif 
