// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Init.h"

#include "../../Common/Log.h"
#include "../../Common/Macros.h"

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#ifdef __unix__
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xutil.h>
#endif

// DECLARE =========================================================================================

nh_wsi_X11 NH_WSI_X11;

// EWMH ============================================================================================

// Retrieve a single window property of the specified type
// Inspired by fghGetWindowProperty from freeglut
//
static unsigned long nh_x11_getWindowProperty(
    Window window, Atom property, Atom type, unsigned char** value)
{
    Atom actualType;
    int actualFormat;
    unsigned long itemCount, bytesAfter;

    XGetWindowProperty(NH_WSI_X11.Display_p, window, property, 0, LONG_MAX, False, type, &actualType, 
        &actualFormat, &itemCount, &bytesAfter, value);

    return itemCount;
}

// Return the atom ID only if it is listed in the specified array
//
static Atom nh_x11_getAtomIfSupported(
    Atom* supportedAtoms, unsigned long atomCount, const char *atomName_p)
{
    const Atom atom = XInternAtom(NH_WSI_X11.Display_p, atomName_p, False);

    for (unsigned long i = 0;  i < atomCount;  i++) {
        if (supportedAtoms[i] == atom)
            return atom;
    }

    return None;
}

// Check whether the running window manager is EWMH-compliant
//
static void nh_x11_detectEWMH()
{
NH_WSI_BEGIN()

    // First we read the _NET_SUPPORTING_WM_CHECK property on the root window

    Window* windowFromRoot = NULL;
    if (!nh_x11_getWindowProperty(NH_WSI_X11.root, NH_WSI_X11.Atoms.NET_SUPPORTING_WM_CHECK,
        XA_WINDOW, (unsigned char**) &windowFromRoot)) {
        NH_WSI_SILENT_END()
    }

    // If it exists, it should be the XID of a top-level window
    // Then we look for the same property on that window

    Window* windowFromChild = NULL;
    if (!nh_x11_getWindowProperty(*windowFromRoot, NH_WSI_X11.Atoms.NET_SUPPORTING_WM_CHECK,
        XA_WINDOW, (unsigned char**) &windowFromChild))
    {
        XFree(windowFromRoot);
        NH_WSI_SILENT_END() 
    }

    // If the property exists, it should contain the XID of the window

    if (*windowFromRoot != *windowFromChild)
    {
        XFree(windowFromRoot);
        XFree(windowFromChild);
        NH_WSI_SILENT_END() 
    }

    XFree(windowFromRoot);
    XFree(windowFromChild);

    // We are now fairly sure that an EWMH-compliant WM is currently running
    // We can now start querying the WM about what features it supports by
    // looking in the _NET_SUPPORTED property on the root window
    // It should contain a list of supported EWMH protocol and state atoms

    Atom* supportedAtoms = NULL;
    const unsigned long atomCount = nh_x11_getWindowProperty(NH_WSI_X11.root,
        NH_WSI_X11.Atoms.NET_SUPPORTED, XA_ATOM, (unsigned char**) &supportedAtoms);

    // See which of the atoms we support that are supported by the WM

    NH_WSI_X11.Atoms.NET_WM_STATE                   = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE");
    NH_WSI_X11.Atoms.NET_WM_STATE_REMOVE            = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_REMOVE");
    NH_WSI_X11.Atoms.NET_WM_STATE_ADD               = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_ADD");
    NH_WSI_X11.Atoms.NET_WM_STATE_TOGGLE            = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_TOGGLE");
    NH_WSI_X11.Atoms.NET_WM_STATE_ABOVE             = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_ABOVE");
    NH_WSI_X11.Atoms.NET_WM_STATE_FULLSCREEN        = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_FULLSCREEN");
    NH_WSI_X11.Atoms.NET_WM_STATE_MAXIMIZED_VERT    = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_MAXIMIZED_VERT");
    NH_WSI_X11.Atoms.NET_WM_STATE_MAXIMIZED_HORZ    = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_MAXIMIZED_HORZ");
    NH_WSI_X11.Atoms.NET_WM_STATE_DEMANDS_ATTENTION = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_DEMANDS_ATTENTION");
    NH_WSI_X11.Atoms.NET_WM_FULLSCREEN_MONITORS     = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_FULLSCREEN_MONITORS");
    NH_WSI_X11.Atoms.NET_WM_WINDOW_TYPE             = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_WINDOW_TYPE");
    NH_WSI_X11.Atoms.NET_WM_WINDOW_TYPE_NORMAL      = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_WINDOW_TYPE_NORMAL");
    NH_WSI_X11.Atoms.NET_WM_MOVERESIZE              = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_MOVERESIZE");
    NH_WSI_X11.Atoms.NET_WORKAREA                   = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_WORKAREA");
    NH_WSI_X11.Atoms.NET_CURRENT_DESKTOP            = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_CURRENT_DESKTOP");
    NH_WSI_X11.Atoms.NET_ACTIVE_WINDOW              = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_ACTIVE_WINDOW");
    NH_WSI_X11.Atoms.NET_FRAME_EXTENTS              = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_FRAME_EXTENTS");
    NH_WSI_X11.Atoms.NET_REQUEST_FRAME_EXTENTS      = nh_x11_getAtomIfSupported(supportedAtoms, atomCount, "_NET_REQUEST_FRAME_EXTENTS");

    if (supportedAtoms)
        XFree(supportedAtoms);

NH_WSI_SILENT_END()
}

// Look for and initialize supported X11 extensions
//
static NH_WSI_RESULT_E nh_x11_initAtoms()
{
NH_WSI_BEGIN()

    // String format atoms
    NH_WSI_X11.Atoms.NULL_       = XInternAtom(NH_WSI_X11.Display_p, "NULL", False);
    NH_WSI_X11.Atoms.UTF8_STRING = XInternAtom(NH_WSI_X11.Display_p, "UTF8_STRING", False);
    NH_WSI_X11.Atoms.STRING      = XInternAtom(NH_WSI_X11.Display_p, "STRING", False);
    NH_WSI_X11.Atoms.TEXT        = XInternAtom(NH_WSI_X11.Display_p, "TEXT", False);
    NH_WSI_X11.Atoms.ATOM_PAIR   = XInternAtom(NH_WSI_X11.Display_p, "ATOM_PAIR", False);

    // Custom selection property atom
    NH_WSI_X11.Atoms.GLFW_SELECTION = XInternAtom(NH_WSI_X11.Display_p, "GLFW_SELECTION", False);

    // ICCCM standard clipboard atoms
    NH_WSI_X11.Atoms.PENGUIN   = XInternAtom(NH_WSI_X11.Display_p, "PENGUIN", False);
    NH_WSI_X11.Atoms.TARGETS   = XInternAtom(NH_WSI_X11.Display_p, "TARGETS", False);
    NH_WSI_X11.Atoms.MULTIPLE  = XInternAtom(NH_WSI_X11.Display_p, "MULTIPLE", False);
    NH_WSI_X11.Atoms.PRIMARY   = XInternAtom(NH_WSI_X11.Display_p, "PRIMARY", False);
    NH_WSI_X11.Atoms.INCR      = XInternAtom(NH_WSI_X11.Display_p, "INCR", False);
    NH_WSI_X11.Atoms.CLIPBOARD = XInternAtom(NH_WSI_X11.Display_p, "CLIPBOARD", False);

    // Clipboard manager atoms
    NH_WSI_X11.Atoms.CLIPBOARD_MANAGER = XInternAtom(NH_WSI_X11.Display_p, "CLIPBOARD_MANAGER", False);
    NH_WSI_X11.Atoms.SAVE_TARGETS      = XInternAtom(NH_WSI_X11.Display_p, "SAVE_TARGETS", False);

    // Xdnd (drag and drop) atoms
    NH_WSI_X11.Atoms.XdndAware      = XInternAtom(NH_WSI_X11.Display_p, "XdndAware", False);
    NH_WSI_X11.Atoms.XdndEnter      = XInternAtom(NH_WSI_X11.Display_p, "XdndEnter", False);
    NH_WSI_X11.Atoms.XdndLeave      = XInternAtom(NH_WSI_X11.Display_p, "XdndLeave", False);
    NH_WSI_X11.Atoms.XdndPosition   = XInternAtom(NH_WSI_X11.Display_p, "XdndPosition", False);
    NH_WSI_X11.Atoms.XdndStatus     = XInternAtom(NH_WSI_X11.Display_p, "XdndStatus", False);
    NH_WSI_X11.Atoms.XdndActionCopy = XInternAtom(NH_WSI_X11.Display_p, "XdndActionCopy", False);
    NH_WSI_X11.Atoms.XdndDrop       = XInternAtom(NH_WSI_X11.Display_p, "XdndDrop", False);
    NH_WSI_X11.Atoms.XdndFinished   = XInternAtom(NH_WSI_X11.Display_p, "XdndFinished", False);
    NH_WSI_X11.Atoms.XdndSelection  = XInternAtom(NH_WSI_X11.Display_p, "XdndSelection", False);
    NH_WSI_X11.Atoms.XdndTypeList   = XInternAtom(NH_WSI_X11.Display_p, "XdndTypeList", False);
    NH_WSI_X11.Atoms.text_uri_list  = XInternAtom(NH_WSI_X11.Display_p, "text/uri-list", False);

    // ICCCM, EWMH and Motif window property atoms
    // These can be set safely even without WM support
    // The EWMH atoms that require WM support are handled in detectEWMH
    NH_WSI_X11.Atoms.WM_PROTOCOLS             = XInternAtom(NH_WSI_X11.Display_p, "WM_PROTOCOLS", False);
    NH_WSI_X11.Atoms.WM_STATE                 = XInternAtom(NH_WSI_X11.Display_p, "WM_STATE", False);
    NH_WSI_X11.Atoms.WM_DELETE_WINDOW         = XInternAtom(NH_WSI_X11.Display_p, "WM_DELETE_WINDOW", False);
    NH_WSI_X11.Atoms.NET_SUPPORTED            = XInternAtom(NH_WSI_X11.Display_p, "_NET_SUPPORTED", False);
    NH_WSI_X11.Atoms.NET_SUPPORTING_WM_CHECK  = XInternAtom(NH_WSI_X11.Display_p, "_NET_SUPPORTING_WM_CHECK", False);
    NH_WSI_X11.Atoms.NET_WM_ICON              = XInternAtom(NH_WSI_X11.Display_p, "_NET_WM_ICON", False);
    NH_WSI_X11.Atoms.NET_WM_PING              = XInternAtom(NH_WSI_X11.Display_p, "_NET_WM_PING", False);
    NH_WSI_X11.Atoms.NET_WM_PID               = XInternAtom(NH_WSI_X11.Display_p, "_NET_WM_PID", False);
    NH_WSI_X11.Atoms.NET_WM_NAME              = XInternAtom(NH_WSI_X11.Display_p, "_NET_WM_NAME", False);
    NH_WSI_X11.Atoms.NET_WM_ICON_NAME         = XInternAtom(NH_WSI_X11.Display_p, "_NET_WM_ICON_NAME", False);
    NH_WSI_X11.Atoms.NET_WM_BYPASS_COMPOSITOR = XInternAtom(NH_WSI_X11.Display_p, "_NET_WM_BYPASS_COMPOSITOR", False);
    NH_WSI_X11.Atoms.NET_WM_WINDOW_OPACITY    = XInternAtom(NH_WSI_X11.Display_p, "_NET_WM_WINDOW_OPACITY", False);
    NH_WSI_X11.Atoms.MOTIF_WM_HINTS           = XInternAtom(NH_WSI_X11.Display_p, "_MOTIF_WM_HINTS", False);

    // The compositing manager selection name contains the screen number
    {
        char name_p[32];
        snprintf(name_p, sizeof(name_p), "_NET_WM_CM_S%u", NH_WSI_X11.screen);
        NH_WSI_X11.Atoms.NET_WM_CM_Sx = XInternAtom(NH_WSI_X11.Display_p, name_p, False);
    }

    // Detect whether an EWMH-conformant window manager is running
    nh_x11_detectEWMH();

NH_WSI_END(NH_WSI_SUCCESS)
}

static NH_WSI_RESULT_E nh_x11_initExtensions()
{
NH_WSI_BEGIN()

    // XKB (X keyboard extension)

    NH_WSI_X11.XKB.Connection_p = XGetXCBConnection(NH_WSI_X11.Display_p);
    NH_WSI_X11.XKB.Context_p    = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    xkb_x11_setup_xkb_extension(
        NH_WSI_X11.XKB.Connection_p, XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, 
        0, NULL, NULL, NULL, NULL
    ); 

    NH_WSI_X11.XKB.keyboardDeviceID = xkb_x11_get_core_keyboard_device_id(NH_WSI_X11.XKB.Connection_p);
    NH_WSI_X11.XKB.Keymap_p = xkb_x11_keymap_new_from_device(
        NH_WSI_X11.XKB.Context_p, 
        NH_WSI_X11.XKB.Connection_p, 
        NH_WSI_X11.XKB.keyboardDeviceID, 
        XKB_KEYMAP_COMPILE_NO_FLAGS
    );

    NH_WSI_X11.XKB.State_p = xkb_x11_state_new_from_device(
        NH_WSI_X11.XKB.Keymap_p, NH_WSI_X11.XKB.Connection_p, NH_WSI_X11.XKB.keyboardDeviceID
    );

NH_WSI_END(NH_WSI_SUCCESS)
}

NH_WSI_RESULT_E nh_x11_initialize()
{
NH_WSI_BEGIN()

    NH_WSI_X11.Display_p = XOpenDisplay(NULL);
    if (!NH_WSI_X11.Display_p) {NH_WSI_END(NH_WSI_ERROR_BAD_STATE)}

    NH_WSI_X11.screen = DefaultScreen(NH_WSI_X11.Display_p);
    NH_WSI_X11.root   = RootWindow(NH_WSI_X11.Display_p, NH_WSI_X11.screen);

    NH_WSI_CHECK(nh_x11_initExtensions())
    NH_WSI_CHECK(nh_x11_initAtoms())

NH_WSI_END(NH_WSI_SUCCESS)
}

NH_WSI_RESULT_E nh_x11_close()
{
NH_WSI_BEGIN()

    xkb_state_unref(NH_WSI_X11.XKB.State_p);
    xkb_keymap_unref(NH_WSI_X11.XKB.Keymap_p);
    xkb_context_unref(NH_WSI_X11.XKB.Context_p);

    XCloseDisplay(NH_WSI_X11.Display_p);
 
NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

