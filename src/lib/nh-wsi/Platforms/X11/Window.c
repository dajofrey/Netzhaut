// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Window.h"
#include "WindowSettings.h"
#include "Init.h"

#include "../../Window/Window.h"
#include "../../Window/WindowSettings.h"
#include "../../Window/Listener.h"
#include "../../Window/Event.h"

#include "../../Common/Log.h"
#include "../../Common/Config.h"

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#ifdef __unix__
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xutil.h>
#endif

// HELPER ==========================================================================================

typedef struct nh_x11_Property {
    unsigned char *data;
    int format, nitems;
    Atom type;
} nh_x11_Property;

static nh_x11_Property nh_wsi_readX11Property(
    Display* Display_p, Window Window, Atom property)
{
    Atom actual_type;
    int actual_format;
    int read_bytes = 1024;	
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *ret = 0;	

    do {
        if(ret != 0) {
            XFree(ret);
        }
        XGetWindowProperty(
            Display_p, Window, property, 0, read_bytes, False, AnyPropertyType,
	    &actual_type, &actual_format, &nitems, &bytes_after, &ret
        );
	read_bytes *= 2;
    }
    while(bytes_after != 0);
    
    nh_x11_Property P = {ret, actual_format, nitems, actual_type};
    return P;
}

/**
 * https://stackoverflow.com/questions/3909713/xlib-xgetwindowattributes-always-returns-1x1
 */
static Window nh_wsi_getX11TopLevelParent(
    Display * display, Window window)
{
     Window parent;
     Window root;
     Window * children;
     unsigned int num_children;

     while (1) {
         if (0 == XQueryTree(display, window, &root,
                   &parent, &children, &num_children)) {
             fprintf(stderr, "XQueryTree error\n");
             abort(); //change to whatever error handling you prefer
         }
         if (children) { //must test for null
             XFree(children);
         }
         if (window == root || parent == root) {
             return window;
         }
         else {
             window = parent;
         }
     }
}

static void nh_wsi_getX11CursorPosition(
    nh_wsi_X11Window *Window_p, double* xpos, double* ypos)
{ 
    Window root, child;  
    int rootX, rootY, childX, childY;
    unsigned int mask;
   
    XQueryPointer(
        NH_WSI_X11.Display_p, Window_p->Handle, &root, &child, &rootX, &rootY, &childX, &childY, &mask);

    if (xpos)
        *xpos = childX;
    if (ypos)
        *ypos = childY;

    return;
}

static void nh_wsi_getX11WindowPosition(
    nh_wsi_X11Window *Window_p, int* xpos, int* ypos)
{   
    Window dummy;
    int x, y;

    XTranslateCoordinates(
        NH_WSI_X11.Display_p, Window_p->Handle, NH_WSI_X11.root, 0, 0, &x, &y, &dummy);

    if (xpos)
        *xpos = x;
    if (ypos)
        *ypos = y;

    return;
}

void nh_wsi_getX11WindowSize(
    nh_wsi_X11Window *Window_p, int* width_p, int* height_p)
{   
    XWindowAttributes Attributes;
    XGetWindowAttributes(NH_WSI_X11.Display_p, Window_p->Handle, &Attributes);
    
    *width_p = Attributes.width;
    *height_p = Attributes.height; 

    return;
}

// https://specifications.freedesktop.org/wm-spec/1.4/ar01s04.html
typedef enum NH_X11_MOVERESIZE_E {
    NH_X11_MOVERESIZE_SIZE_TOPLEFT = 0,
    NH_X11_MOVERESIZE_SIZE_TOP,
    NH_X11_MOVERESIZE_SIZE_TOPRIGHT,
    NH_X11_MOVERESIZE_SIZE_RIGHT,
    NH_X11_MOVERESIZE_SIZE_BOTTOMRIGHT,
    NH_X11_MOVERESIZE_SIZE_BOTTOM,
    NH_X11_MOVERESIZE_SIZE_BOTTOMLEFT,
    NH_X11_MOVERESIZE_SIZE_LEFT,
    NH_X11_MOVERESIZE_MOVE,
} NH_X11_MOVERESIZE_E;

/**
 * Credit goes to felselva https://github.com/glfw/glfw/pull/991/files
 *
 * @param Window_p Valid nh_x11_Window handle.
 * @param border Value from NH_X11_MOVERESIZE_E.
 */
static NH_API_RESULT nh_wsi_resizeX11WindowDrag(
    nh_wsi_X11Window *Window_p, int border)
{
    int winXpos, winYpos;
    double curXpos, curYpos;
    XClientMessageEvent xclient;
    memset(&xclient, 0, sizeof(XClientMessageEvent));

    XUngrabPointer(NH_WSI_X11.Display_p, 0);
    XFlush(NH_WSI_X11.Display_p);

    nh_wsi_getX11CursorPosition(Window_p, &curXpos, &curYpos);
    nh_wsi_getX11WindowPosition(Window_p, &winXpos, &winYpos);

    xclient.type = ClientMessage;
    xclient.window = Window_p->Handle;
    xclient.message_type = NH_WSI_X11.Atoms.NET_WM_MOVERESIZE;
    xclient.format = 32;
    xclient.data.l[0] = winXpos + curXpos;
    xclient.data.l[1] = winYpos + curYpos;
    xclient.data.l[2] = border;
    xclient.data.l[3] = 0;
    xclient.data.l[4] = 0;

    XSendEvent(NH_WSI_X11.Display_p, NH_WSI_X11.root, False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *)&xclient);
    XFlush(NH_WSI_X11.Display_p);

    return NH_API_SUCCESS;
}

/**
 * Credit goes to felselva https://github.com/glfw/glfw/pull/991/files
 *
 * @param Window_p Valid nh_x11_Window handle.
 */
NH_API_RESULT nh_wsi_moveX11Window(
    nh_wsi_X11Window *Window_p)
{
    int winXpos, winYpos;
    double curXpos, curYpos;
 
    nh_wsi_getX11CursorPosition(Window_p, &curXpos, &curYpos);
    nh_wsi_getX11WindowPosition(Window_p, &winXpos, &winYpos);

    XUngrabPointer(NH_WSI_X11.Display_p, 0);
    XFlush(NH_WSI_X11.Display_p);

    XClientMessageEvent xclient;
    memset(&xclient, 0, sizeof(XClientMessageEvent));

    xclient.type = ClientMessage;
    xclient.window = Window_p->Handle;
    xclient.message_type = XInternAtom(NH_WSI_X11.Display_p, "_NET_WM_MOVERESIZE", False);
    xclient.format = 32;
    xclient.data.l[0] = winXpos + curXpos;
    xclient.data.l[1] = winYpos + curYpos;
    xclient.data.l[2] = NH_X11_MOVERESIZE_MOVE;
    xclient.data.l[3] = 0;
    xclient.data.l[4] = 0;

    XSendEvent(NH_WSI_X11.Display_p, NH_WSI_X11.root, False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *)&xclient);
    XFlush(NH_WSI_X11.Display_p);

    return NH_API_SUCCESS;
}

// GET INPUT =======================================================================================

// This is needed because we don't want to block execution of other stuff in single threaded mode.
#define NH_X11_INPUT_LIMIT 30

NH_API_RESULT nh_wsi_getX11Input(
    nh_wsi_Window *Window_p, bool *idle_p) 
{
    nh_wsi_WindowConfig Config = nh_wsi_getWindowConfig(Window_p);

    XEvent Event;
    int xdnd = 0;
    int limit = 0;

    while (XPending(NH_WSI_X11.Display_p) > 0) 
    {
        *idle_p = false;
        XNextEvent(NH_WSI_X11.Display_p, &Event);
        
        if (Event.type == ClientMessage)
        {   
                 if (Event.xclient.data.l[0] == NH_WSI_X11.Atoms.WM_DELETE_WINDOW) {exit(0);} // TODO event
            else if (Event.xclient.data.l[0] == NH_WSI_X11.Atoms.NET_WM_PING)
            {
                // The window manager is pinging the application to ensure
                // it's still responding to events

                XEvent Reply = Event;
                Reply.xclient.window = NH_WSI_X11.root;

                XSendEvent(NH_WSI_X11.Display_p, NH_WSI_X11.root,
                           False,
                           SubstructureNotifyMask | SubstructureRedirectMask,
                           &Reply);
            }

            else if (Event.type == Expose) {}
            else if (Event.xclient.message_type == NH_WSI_X11.Atoms.XdndEnter) 
            {
                xdnd = (Event.xclient.data.l[1] >> 24);
            }
            else if (Event.xclient.message_type == NH_WSI_X11.Atoms.XdndDrop)
            {
                Atom string = XInternAtom(NH_WSI_X11.Display_p, "STRING", 0);
		
                if (xdnd >= 1) {
		    XConvertSelection(NH_WSI_X11.Display_p, NH_WSI_X11.Atoms.XdndSelection, 
                        string, NH_WSI_X11.Atoms.PRIMARY, Window_p->X11.Handle, Event.xclient.data.l[2]);
                } else {
                    XConvertSelection(NH_WSI_X11.Display_p, NH_WSI_X11.Atoms.XdndSelection, 
                        string, NH_WSI_X11.Atoms.PRIMARY, Window_p->X11.Handle, CurrentTime);
                }
            }
            else if (Event.xclient.message_type == NH_WSI_X11.Atoms.XdndPosition)
            {
                Atom action = NH_WSI_X11.Atoms.XdndActionCopy;
                if(xdnd >= 2) {action = Event.xclient.data.l[4];}
                
                XClientMessageEvent m;
                memset(&m, sizeof(m), 0);
                m.type         = ClientMessage;
                m.display      = Event.xclient.display;
                m.window       = Event.xclient.data.l[0];
                m.message_type = NH_WSI_X11.Atoms.XdndStatus;
                m.format       = 32;
                
                m.data.l[0] = Window_p->X11.Handle;
                m.data.l[1] = 1;
                m.data.l[2] = 0; //Specify an empty rectangle
                m.data.l[3] = 0;
                m.data.l[4] = NH_WSI_X11.Atoms.XdndActionCopy; //We only accept copying anyway.

                XSendEvent(NH_WSI_X11.Display_p, Event.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                XFlush(NH_WSI_X11.Display_p);
            }
            else if (Event.xclient.message_type == NH_WSI_X11.Atoms.XdndLeave) {}
        }
        else if (Event.type == ConfigureNotify) 
        {
            XConfigureEvent Configure = Event.xconfigure;

            XWindowAttributes XWA; 
            XGetWindowAttributes(NH_WSI_X11.Display_p, 
                nh_wsi_getX11TopLevelParent(NH_WSI_X11.Display_p, Window_p->X11.Handle), &XWA);

            nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_CONFIGURE, XWA.x, XWA.y, Configure.width, Configure.height);
        }
        else if (Event.type == MotionNotify)
        {
            nh_wsi_sendMouseEvent(Window_p, Event.xbutton.x, Event.xbutton.y, NH_API_TRIGGER_MOVE, 
                NH_API_MOUSE_MOVE);

            if (Config.resizable && !Config.decorated) {
                int width, height;
                nh_wsi_getX11WindowSize(&Window_p->X11, &width, &height);

                if (Event.xbutton.x < 5 && Event.xbutton.y <= height-5 && Event.xbutton.y >= 5) {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_LEFT_SIDE);
                } else if (Event.xbutton.x < 5 && Event.xbutton.y < 5) {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_TOP_LEFT_CORNER);
                } else if (Event.xbutton.x > width-5 && Event.xbutton.y <= height-5 && Event.xbutton.y >= 5) {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_RIGHT_SIDE);
                } else if (Event.xbutton.x > width-5 && Event.xbutton.y < 5) {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_TOP_RIGHT_CORNER);
                } else if (Event.xbutton.x < 5 && Event.xbutton.y > height-5) {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_BOTTOM_LEFT_CORNER);
                } else if (Event.xbutton.x > width-5 && Event.xbutton.y > height-5) {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_BOTTOM_RIGHT_CORNER);
                } else if (Event.xbutton.y < 5) {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_TOP_SIDE);
                } else if (Event.xbutton.y > height-5) {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_BOTTOM_SIDE);
                } else {
                    nh_wsi_setMouseCursor(Window_p, NH_WSI_CURSOR_ARROW);
                }
            } 
        }
        else if (Event.type == ButtonPress || Event.type == ButtonRelease)
        {
            NH_API_TRIGGER_E trigger = Event.type == ButtonPress ? NH_API_TRIGGER_PRESS : NH_API_TRIGGER_RELEASE;
            NH_API_MOUSE_E button = 0;

            switch (Event.xbutton.button)
            {
                case Button1 : button = NH_API_MOUSE_LEFT; break;
                case Button2 : button = NH_API_MOUSE_MIDDLE; break;
                case Button3 : button = NH_API_MOUSE_RIGHT; break;
                case Button4 : button = NH_API_MOUSE_SCROLL; trigger = NH_API_TRIGGER_UP; break;
                case Button5 : button = NH_API_MOUSE_SCROLL; trigger = NH_API_TRIGGER_DOWN; break;
            }

            if (Config.resizable && !Config.decorated) {
                int width, height;
                nh_wsi_getX11WindowSize(&Window_p->X11, &width, &height);

                if (Event.xbutton.x < 5 && Event.xbutton.y <= height-5 && Event.xbutton.y >= 5) {
                    nh_wsi_resizeX11WindowDrag(&Window_p->X11, NH_X11_MOVERESIZE_SIZE_LEFT);
                } else if (Event.xbutton.x < 5 && Event.xbutton.y < 5) {
                    nh_wsi_resizeX11WindowDrag(&Window_p->X11, NH_X11_MOVERESIZE_SIZE_TOPLEFT);
                } else if (Event.xbutton.x > width-5 && Event.xbutton.y <= height-5 && Event.xbutton.y >= 5) {
                    nh_wsi_resizeX11WindowDrag(&Window_p->X11, NH_X11_MOVERESIZE_SIZE_RIGHT);
                } else if (Event.xbutton.x > width-5 && Event.xbutton.y < 5) {
                    nh_wsi_resizeX11WindowDrag(&Window_p->X11, NH_X11_MOVERESIZE_SIZE_TOPRIGHT);
                } else if (Event.xbutton.x < 5 && Event.xbutton.y > height-5) {
                    nh_wsi_resizeX11WindowDrag(&Window_p->X11, NH_X11_MOVERESIZE_SIZE_BOTTOMLEFT);
                } else if (Event.xbutton.x > width-5 && Event.xbutton.y > height-5) {
                    nh_wsi_resizeX11WindowDrag(&Window_p->X11, NH_X11_MOVERESIZE_SIZE_BOTTOMRIGHT);
                } else if (Event.xbutton.y < 5) {
                    nh_wsi_resizeX11WindowDrag(&Window_p->X11, NH_X11_MOVERESIZE_SIZE_TOP);
                } else if (Event.xbutton.y > height-5) {
                    nh_wsi_resizeX11WindowDrag(&Window_p->X11, NH_X11_MOVERESIZE_SIZE_BOTTOM);
                } else {
                    nh_wsi_sendMouseEvent(Window_p, Event.xbutton.x, Event.xbutton.y, trigger, button);
                }
            } else {
                nh_wsi_sendMouseEvent(Window_p, Event.xbutton.x, Event.xbutton.y, trigger, button);
            }
        }
        else if (Event.type == KeyPress || Event.type == KeyRelease)
        {
            NH_API_TRIGGER_E trigger = Event.type == KeyPress ? NH_API_TRIGGER_PRESS : NH_API_TRIGGER_RELEASE;
            NH_API_KEY_E special = 0;

            // https://tronche.com/gui/x/xlib/input/keyboard-encoding.html
            // XLookupString seems to be the most straight forward way to get modified e.g. shifted keysym.
            // It does NOT manipulate keys to historical ASCII C0 control characters when pressing CTRL.
            KeySym keysym;
            XLookupString(&Event.xkey, NULL, 0, &keysym, NULL);
 
            // xkbcommon-keysyms.h
            switch (keysym)
            {
                // TTY function keys
                case XK_BackSpace   : special = NH_API_KEY_BACKSPACE; break;
                case XK_Tab         : special = NH_API_KEY_TAB; break;  
                case XK_Linefeed    : special = NH_API_KEY_LINEFEED; break;  
                case XK_Clear       : special = NH_API_KEY_CLEAR; break;  
                case XK_Return      : special = NH_API_KEY_RETURN; break;  
                case XK_Pause       : special = NH_API_KEY_PAUSE; break;  
                case XK_Scroll_Lock : special = NH_API_KEY_SCROLL_LOCK; break;
                case XK_Sys_Req     : special = NH_API_KEY_SYS_REQ; break;  
                case XK_Escape      : special = NH_API_KEY_ESCAPE; break;  
                case XK_Delete      : special = NH_API_KEY_DELETE; break;    

                // Modifiers
                case XK_Shift_L    : special = NH_API_KEY_SHIFT_L; break; 
                case XK_Shift_R    : special = NH_API_KEY_SHIFT_R; break;   
                case XK_Control_L  : special = NH_API_KEY_CONTROL_L; break; 
                case XK_Control_R  : special = NH_API_KEY_CONTROL_R; break;
                case XK_Caps_Lock  : special = NH_API_KEY_CAPS_LOCK; break; 
                case XK_Shift_Lock : special = NH_API_KEY_SHIFT_LOCK; break;
                case XK_Meta_L     : special = NH_API_KEY_META_L; break;    
                case XK_Meta_R     : special = NH_API_KEY_META_R; break;    
                case XK_Alt_L      : special = NH_API_KEY_ALT_L; break;     
                case XK_Alt_R      : special = NH_API_KEY_ALT_R; break;     
                case XK_Super_L    : special = NH_API_KEY_SUPER_L; break;   
                case XK_Super_R    : special = NH_API_KEY_SUPER_R; break;   
                case XK_Hyper_L    : special = NH_API_KEY_HYPER_L; break;   
                case XK_Hyper_R    : special = NH_API_KEY_HYPER_R; break;  

                // Cursor control & motion
                case XK_Home      : special = NH_API_KEY_HOME; break;
                case XK_Left      : special = NH_API_KEY_LEFT; break;
                case XK_Up        : special = NH_API_KEY_UP; break;
                case XK_Right     : special = NH_API_KEY_RIGHT; break;
                case XK_Down      : special = NH_API_KEY_DOWN; break; 
                case XK_Page_Up   : special = NH_API_KEY_PAGE_UP; break;
                case XK_Page_Down : special = NH_API_KEY_PAGE_DOWN; break;
                case XK_End       : special = NH_API_KEY_END; break;
                case XK_Begin     : special = NH_API_KEY_BEGIN; break;

                // Misc functions
                case XK_Select        : special = NH_API_KEY_SELECT; break;
                case XK_Print         : special = NH_API_KEY_PRINT; break;
                case XK_Execute       : special = NH_API_KEY_EXECUTE; break;
                case XK_Insert        : special = NH_API_KEY_INSERT; break;
                case XK_Undo          : special = NH_API_KEY_UNDO; break;
                case XK_Redo          : special = NH_API_KEY_REDO; break;
                case XK_Menu          : special = NH_API_KEY_MENU; break;
                case XK_Find          : special = NH_API_KEY_FIND; break;
                case XK_Cancel        : special = NH_API_KEY_CANCEL; break;
                case XK_Help          : special = NH_API_KEY_HELP; break;
                case XK_Break         : special = NH_API_KEY_BREAK; break;
                case XK_Mode_switch   : special = NH_API_KEY_MODE_SWITCH; break;
                case XK_Num_Lock      : special = NH_API_KEY_NUM_LOCK; break;

                // Keypad functions
                case XK_KP_Space     : special = NH_API_KEY_KP_SPACE; break;
                case XK_KP_Tab       : special = NH_API_KEY_KP_TAB; break;
                case XK_KP_Enter     : special = NH_API_KEY_KP_ENTER; break;
                case XK_KP_F1        : special = NH_API_KEY_KP_F1; break;
                case XK_KP_F2        : special = NH_API_KEY_KP_F2; break;
                case XK_KP_F3        : special = NH_API_KEY_KP_F3; break;
                case XK_KP_F4        : special = NH_API_KEY_KP_F4; break;
                case XK_KP_Home      : special = NH_API_KEY_KP_HOME; break;
                case XK_KP_Left      : special = NH_API_KEY_KP_LEFT; break;
                case XK_KP_Up        : special = NH_API_KEY_KP_UP; break;
                case XK_KP_Right     : special = NH_API_KEY_KP_RIGHT; break;
                case XK_KP_Down      : special = NH_API_KEY_KP_DOWN; break;
                case XK_KP_Prior     : special = NH_API_KEY_KP_PRIOR; break;
                case XK_KP_Next      : special = NH_API_KEY_KP_NEXT; break;
                case XK_KP_End       : special = NH_API_KEY_KP_END; break;
                case XK_KP_Begin     : special = NH_API_KEY_KP_BEGIN; break;
                case XK_KP_Insert    : special = NH_API_KEY_KP_INSERT; break;
                case XK_KP_Delete    : special = NH_API_KEY_KP_DELETE; break;
                case XK_KP_Equal     : special = NH_API_KEY_KP_EQUAL; break;
                case XK_KP_Multiply  : special = NH_API_KEY_KP_MULTIPLY; break;
                case XK_KP_Add       : special = NH_API_KEY_KP_ADD; break;
                case XK_KP_Separator : special = NH_API_KEY_KP_SEPARATOR; break;
                case XK_KP_Subtract  : special = NH_API_KEY_KP_SUBTRACT; break;
                case XK_KP_Decimal   : special = NH_API_KEY_KP_DECIMAL; break;
                case XK_KP_Divide    : special = NH_API_KEY_KP_DIVIDE; break;

                case XK_KP_0 : special = NH_API_KEY_KP_0; break;
                case XK_KP_1 : special = NH_API_KEY_KP_1; break;
                case XK_KP_2 : special = NH_API_KEY_KP_2; break;
                case XK_KP_3 : special = NH_API_KEY_KP_3; break;
                case XK_KP_4 : special = NH_API_KEY_KP_4; break;
                case XK_KP_5 : special = NH_API_KEY_KP_5; break;
                case XK_KP_6 : special = NH_API_KEY_KP_6; break;
                case XK_KP_7 : special = NH_API_KEY_KP_7; break;
                case XK_KP_8 : special = NH_API_KEY_KP_8; break;
                case XK_KP_9 : special = NH_API_KEY_KP_9; break;

                case XK_F1  : special = NH_API_KEY_F1; break;
                case XK_F2  : special = NH_API_KEY_F2; break;
                case XK_F3  : special = NH_API_KEY_F3; break;
                case XK_F4  : special = NH_API_KEY_F4; break;
                case XK_F5  : special = NH_API_KEY_F5; break;
                case XK_F6  : special = NH_API_KEY_F6; break;
                case XK_F7  : special = NH_API_KEY_F7; break;
                case XK_F8  : special = NH_API_KEY_F8; break;
                case XK_F9  : special = NH_API_KEY_F9; break;
                case XK_F10 : special = NH_API_KEY_F10; break;
                case XK_F11 : special = NH_API_KEY_F11; break;
                case XK_F12 : special = NH_API_KEY_F12; break;
                case XK_F13 : special = NH_API_KEY_F13; break;
                case XK_F14 : special = NH_API_KEY_F14; break;
                case XK_F15 : special = NH_API_KEY_F15; break;
                case XK_F16 : special = NH_API_KEY_F16; break;
                case XK_F17 : special = NH_API_KEY_F17; break;
                case XK_F18 : special = NH_API_KEY_F18; break;
                case XK_F19 : special = NH_API_KEY_F19; break;
                case XK_F20 : special = NH_API_KEY_F20; break;
                case XK_F21 : special = NH_API_KEY_F21; break;
                case XK_F22 : special = NH_API_KEY_F22; break;
                case XK_F23 : special = NH_API_KEY_F23; break;
                case XK_F24 : special = NH_API_KEY_F24; break;
                case XK_F25 : special = NH_API_KEY_F25; break;
                case XK_F26 : special = NH_API_KEY_F26; break;
                case XK_F27 : special = NH_API_KEY_F27; break;
                case XK_F28 : special = NH_API_KEY_F28; break;
                case XK_F29 : special = NH_API_KEY_F29; break;
                case XK_F30 : special = NH_API_KEY_F30; break;
                case XK_F31 : special = NH_API_KEY_F31; break;
                case XK_F32 : special = NH_API_KEY_F32; break;
                case XK_F33 : special = NH_API_KEY_F33; break;
                case XK_F34 : special = NH_API_KEY_F34; break;
                case XK_F35 : special = NH_API_KEY_F35; break;
            }

            // Get UTF32 representation.
            NH_ENCODING_UTF32 codepoint = xkb_keysym_to_utf32(keysym);

            // Apply historical ASCII control char shift if necessary. 
            if (codepoint >= 97 && codepoint <= 122 && (Event.xkey.state & ControlMask)) {
                codepoint = codepoint & 0x1f; 
            }
            nh_wsi_sendKeyboardEvent(Window_p, codepoint, special, trigger, Event.xkey.state);
        }
        else if (Event.type == FocusIn)
        {
            nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_FOCUS_IN, 0, 0, 0, 0);
        }
        else if (Event.type == FocusOut)
        {
            nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_FOCUS_OUT, 0, 0, 0, 0);
        }
        else if (Event.type == PropertyNotify)
        {
        }
        else if (Event.type == SelectionNotify)
        {
            XSelectionEvent *Event_p = (XSelectionEvent*)&Event.xselection;
            if (Event_p->property == None) {continue;}

            Atom data, type;
            int di;
            unsigned long size, dul;
            unsigned char *data_p = NULL;

            /* Dummy call to get type and size. */
            XGetWindowProperty(NH_WSI_X11.Display_p, Window_p->X11.Handle, NH_WSI_X11.Atoms.PENGUIN, 0, 0, False, AnyPropertyType,
                               &type, &di, &dul, &size, &data_p);
            XFree(data_p);

            Atom incr = XInternAtom(NH_WSI_X11.Display_p, "INCR", False);
            if (type == incr) {
                printf("Data too large and INCR mechanism not implemented\n");
                continue;
            }

            XGetWindowProperty(NH_WSI_X11.Display_p, Window_p->X11.Handle, NH_WSI_X11.Atoms.PENGUIN, 0, size, False, AnyPropertyType,
                               &data, &di, &dul, &dul, &data_p);

            NH_CORE_CHECK(nh_wsi_setClipboard(data_p, size, false))
            XFree(data_p);
        }
        else if (Event.type == SelectionClear)
        {
        }
        else if (Event.type == SelectionRequest)
        {
            XSelectionRequestEvent *Request_p = (XSelectionRequestEvent*)&Event.xselectionrequest;

            XSelectionEvent Response;
            Response.type = SelectionNotify;
            Response.requestor = Request_p->requestor;
            Response.selection = Request_p->selection;
            Response.target = Request_p->target;
            Response.time = Request_p->time;
            if (Request_p->property == None) {
                Request_p->property = Request_p->target;
            }
 
            // Default behavior rejects requests.
            Response.property = None;

            if (Request_p->target == NH_WSI_X11.Atoms.TARGETS) {
                // The list of supported targets.
                const Atom targets[] = {
                    NH_WSI_X11.Atoms.TARGETS, 
                    NH_WSI_X11.Atoms.UTF8_STRING,
                    NH_WSI_X11.Atoms.TEXT,
                    NH_WSI_X11.Atoms.STRING,
                };

                XChangeProperty(
                    NH_WSI_X11.Display_p,
                    Request_p->requestor,
                    Request_p->property,
                    XA_ATOM,
                    32,
                    PropModeReplace,
                    (unsigned char*) targets,
                    sizeof(targets) / sizeof(targets[0]));

                Response.property = Request_p->property;
            }
            else if (Request_p->target == NH_WSI_X11.Atoms.UTF8_STRING || Request_p->target == NH_WSI_X11.Atoms.STRING || Request_p->target == NH_WSI_X11.Atoms.TEXT) {
                char *clipboard_p = nh_wsi_getClipboard();
                if (clipboard_p == NULL) {continue;}
            
                XChangeProperty(
                    NH_WSI_X11.Display_p, 
                    Request_p->requestor, 
                    Request_p->property, 
                    NH_WSI_X11.Atoms.UTF8_STRING, 
                    8, 
                    PropModeReplace,
                    (unsigned char *)clipboard_p, strlen(clipboard_p));
            
                Response.property = Request_p->property;
            }

            // Send response.
            XSendEvent(NH_WSI_X11.Display_p, Request_p->requestor, 1, 0, (XEvent *)&Response);
        }
        else if (Event.type == ReparentNotify)
        {
        }

        if (limit++ == NH_X11_INPUT_LIMIT) {break;}
    }

    return NH_API_SUCCESS;
}

// WINDOW ==========================================================================================

// Updates the full screen status of the window
//
static void nh_wsi_updateX11WindowMode(
    nh_wsi_X11Window *Window_p)
{
//    if (window->monitor)
//    {
//        if (_glfw.x11.xinerama.available &&
//            _glfw.x11.NET_WM_FULLSCREEN_MONITORS)
//        {
//            sendEventToWM(window,
//                          _glfw.x11.NET_WM_FULLSCREEN_MONITORS,
//                          window->monitor->x11.index,
//                          window->monitor->x11.index,
//                          window->monitor->x11.index,
//                          window->monitor->x11.index,
//                          0);
//        }
//
//        if (_glfw.x11.NET_WM_STATE && _glfw.x11.NET_WM_STATE_FULLSCREEN)
//        {
//            sendEventToWM(window,
//                          _glfw.x11.NET_WM_STATE,
//                          _NET_WM_STATE_ADD,
//                          _glfw.x11.NET_WM_STATE_FULLSCREEN,
//                          0, 1, 0);
//        }
//        else
//        {
//            // This is the butcher's way of removing window decorations
//            // Setting the override-redirect attribute on a window makes the
//            // window manager ignore the window completely (ICCCM, section 4)
//            // The good thing is that this makes undecorated full screen windows
//            // easy to do; the bad thing is that we have to do everything
//            // manually and some things (like iconify/restore) won't work at
//            // all, as those are tasks usually performed by the window manager
//
//            XSetWindowAttributes attributes;
//            attributes.override_redirect = True;
//            XChangeWindowAttributes(_glfw.x11.display,
//                                    window->x11.handle,
//                                    CWOverrideRedirect,
//                                    &attributes);
//
//            window->x11.overrideRedirect = GLFW_TRUE;
//        }
//
//        // Enable compositor bypass
//        if (!window->x11.transparent)
//        {
//            const unsigned long value = 1;
//
//            XChangeProperty(_glfw.x11.display,  window->x11.handle,
//                            _glfw.x11.NET_WM_BYPASS_COMPOSITOR, XA_CARDINAL, 32,
//                            PropModeReplace, (unsigned char*) &value, 1);
//        }
//    }
//    else
//    {
//        if (_glfw.x11.xinerama.available &&
//            _glfw.x11.NET_WM_FULLSCREEN_MONITORS)
//        {
//            XDeleteProperty(_glfw.x11.display, window->x11.handle,
//                            _glfw.x11.NET_WM_FULLSCREEN_MONITORS);
//        }
//
//        if (_glfw.x11.NET_WM_STATE && _glfw.x11.NET_WM_STATE_FULLSCREEN)
//        {
//            sendEventToWM(window,
//                          _glfw.x11.NET_WM_STATE,
//                          _NET_WM_STATE_REMOVE,
//                          _glfw.x11.NET_WM_STATE_FULLSCREEN,
//                          0, 1, 0);
//        }
//        else
//        {
//            XSetWindowAttributes attributes;
//            attributes.override_redirect = False;
//            XChangeWindowAttributes(_glfw.x11.display,
//                                    window->x11.handle,
//                                    CWOverrideRedirect,
//                                    &attributes);
//
//            window->x11.overrideRedirect = GLFW_FALSE;
//        }
//
//        // Disable compositor bypass
//        if (!window->x11.transparent)
//        {
//            XDeleteProperty(_glfw.x11.display, window->x11.handle,
//                            _glfw.x11.NET_WM_BYPASS_COMPOSITOR);
//        }
//    }

    return;
}

// Make the specified window and its video mode active on its monitor
//
static void nh_wsi_acquireX11Monitor(
    nh_wsi_X11Window *Window_p)
{
//    if (_glfw.x11.saver.count == 0)
//    {
//        // Remember old screen saver settings
//        XGetScreenSaver(_glfw.x11.display,
//                        &_glfw.x11.saver.timeout,
//                        &_glfw.x11.saver.interval,
//                        &_glfw.x11.saver.blanking,
//                        &_glfw.x11.saver.exposure);
//
//        // Disable screen saver
//        XSetScreenSaver(_glfw.x11.display, 0, 0, DontPreferBlanking,
//                        DefaultExposures);
//    }
//
//    if (!window->monitor->window)
//        _glfw.x11.saver.count++;
//
//    _glfwSetVideoModeX11(window->monitor, &window->videoMode);
//
//    if (window->x11.overrideRedirect)
//    {
//        int xpos, ypos;
//        GLFWvidmode mode;
//
//        // Manually position the window over its monitor
//        _glfwGetMonitorPosX11(window->monitor, &xpos, &ypos);
//        _glfwGetVideoModeX11(window->monitor, &mode);
//
//        XMoveResizeWindow(_glfw.x11.display, window->x11.handle,
//                          xpos, ypos, mode.width, mode.height);
//    }
//
//    _glfwInputMonitorWindow(window->monitor, window);

    return;
}

// Updates the normal hints according to the window settings
//
static void nh_wsi_updateX11NormalHints(
    nh_wsi_X11Window *Window_p, nh_wsi_WindowConfig *Config_p)
{
    XSizeHints* hints = XAllocSizeHints();

//    if (!window->monitor)
//    {
//        if (window->resizable)
//        {
//            if (window->minwidth != GLFW_DONT_CARE &&
//                window->minheight != GLFW_DONT_CARE)
//            {
//                hints->flags |= PMinSize;
//                hints->min_width = window->minwidth;
//                hints->min_height = window->minheight;
//            }
//
//            if (window->maxwidth != GLFW_DONT_CARE &&
//                window->maxheight != GLFW_DONT_CARE)
//            {
//                hints->flags |= PMaxSize;
//                hints->max_width = window->maxwidth;
//                hints->max_height = window->maxheight;
//            }
//
//            if (window->numer != GLFW_DONT_CARE &&
//                window->denom != GLFW_DONT_CARE)
//            {
//                hints->flags |= PAspect;
//                hints->min_aspect.x = hints->max_aspect.x = window->numer;
//                hints->min_aspect.y = hints->max_aspect.y = window->denom;
//            }
//        }
//        else
//        {
//            hints->flags |= (PMinSize | PMaxSize);
//            hints->min_width  = hints->max_width  = width;
//            hints->min_height = hints->max_height = height;
//        }
//    }

    hints->flags |= PWinGravity;
    hints->win_gravity = StaticGravity;

    XSetWMNormalHints(NH_WSI_X11.Display_p, Window_p->Handle, hints);
    XFree(hints);

    return;
}

NH_API_RESULT nh_wsi_createX11Window(
    nh_wsi_X11Window *Window_p, nh_wsi_WindowConfig Config, nh_gfx_SurfaceRequirements *Requirements_p)
{
    XVisualInfo Template = {0};

    Template.screen        = 0;
    Template.depth         = 32;
    Template.class         = TrueColor;
    Template.red_mask      = 0xff0000;
    Template.green_mask    = 0xff00;
    Template.blue_mask     = 0xff;
    Template.colormap_size = 256;
    Template.bits_per_rgb  = 8;

    long mask = 
      VisualScreenMask
    | VisualClassMask
    | VisualRedMaskMask
    | VisualGreenMaskMask
    | VisualBlueMaskMask
    | VisualColormapSizeMask
    | VisualBitsPerRGBMask;

    int infos = 0;
    XVisualInfo *Infos_p = XGetVisualInfo(NH_WSI_X11.Display_p, mask, &Template, &infos);
    XVisualInfo *Info_p = NULL;

    for (int i = 0; i < infos; ++i) {
        if (!Requirements_p) {
            Info_p = Infos_p;
            Window_p->Info = *Info_p;
            break;
        }
        for (int j = 0; j < Requirements_p->OpenGL.Ids.length; ++j) {
            if (Infos_p[i].visualid == ((VisualID*)Requirements_p->OpenGL.Ids.p)[j]) {
                Info_p = Infos_p+i;
                Window_p->Info = *Info_p;
                break;
            }
        }
        if (Info_p) {break;}
    }

    if (!Info_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    XSetWindowAttributes Attributes = {0,};

    Attributes.colormap = 
        XCreateColormap(NH_WSI_X11.Display_p, NH_WSI_X11.root, Info_p->visual, AllocNone);
    Attributes.background_pixmap = None;
    Attributes.border_pixmap = None;
    Attributes.border_pixel = 0;
    Attributes.event_mask =
        StructureNotifyMask
      | EnterWindowMask
      | LeaveWindowMask
      | ExposureMask
      | ButtonPressMask
      | ButtonReleaseMask
      | OwnerGrabButtonMask
      | PointerMotionMask
      | PropertyChangeMask
      | KeyPressMask
      | FocusChangeMask
      | KeyReleaseMask;

    int attributesMask = 
        CWBackPixmap
      | CWColormap
      | CWBorderPixel
      | CWEventMask;

    Window_p->Handle = 
        XCreateWindow(NH_WSI_X11.Display_p, NH_WSI_X11.root, Config.Position.x, Config.Position.y, Config.Size.width, 
            Config.Size.height, 0, Info_p->depth, InputOutput, Info_p->visual, attributesMask, &Attributes);

    XFree(Infos_p);

    // Declare the WM protocols supported by us.
    Atom protocols[] = {
        NH_WSI_X11.Atoms.WM_DELETE_WINDOW,
        NH_WSI_X11.Atoms.NET_WM_PING
    };

    XSetWMProtocols(NH_WSI_X11.Display_p, Window_p->Handle, protocols, sizeof(protocols)/sizeof(Atom));

    // Declare our PID.
    const long pid = getpid();
    XChangeProperty(NH_WSI_X11.Display_p, Window_p->Handle, NH_WSI_X11.Atoms.NET_WM_PID, 
                    XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &pid, 1);

    // Set ICCCM WM_HINTS property
    XWMHints* Hints_p = XAllocWMHints();
    NH_CORE_CHECK_MEM(Hints_p)

    Hints_p->flags = StateHint;
    Hints_p->initial_state = NormalState;

    XSetWMHints(NH_WSI_X11.Display_p, Window_p->Handle, Hints_p);
    XFree(Hints_p);

    nh_wsi_updateX11NormalHints(Window_p, &Config);

    // Set ICCCM WM_CLASS property, WM's seem to need this. 
    XClassHint* Hint_p = XAllocClassHint();

    Hint_p->res_name = Config.title_p;
    Hint_p->res_class = Config.title_p;

//    if (strlen(wndconfig->x11.instanceName) &&
//        strlen(wndconfig->x11.className))
//    {
//        Hint_p->res_name = (char*) wndconfig->x11.instanceName;
//        Hint_p->res_class = (char*) wndconfig->x11.className;
//    }
//    else
//    {
//        const char* resourceName = getenv("RESOURCE_NAME");
//        if (resourceName && strlen(resourceName))
//            hint->res_name = (char*) resourceName;
//        else if (strlen(wndconfig->title))
//            hint->res_name = (char*) wndconfig->title;
//        else
//            hint->res_name = (char*) "glfw-application";
//
//        if (strlen(wndconfig->title))
//            hint->res_class = (char*) wndconfig->title;
//        else
//            hint->res_class = (char*) "GLFW-Application";
//    }

    XSetClassHint(NH_WSI_X11.Display_p, Window_p->Handle, Hint_p);
    XFree(Hint_p);

    // Announce support for Xdnd (drag and drop).
    const Atom version = 5;
    XChangeProperty(NH_WSI_X11.Display_p, Window_p->Handle,
                    NH_WSI_X11.Atoms.XdndAware, XA_ATOM, 32,
                    PropModeReplace, (unsigned char*) &version, 1);

    XMapWindow(NH_WSI_X11.Display_p, Window_p->Handle);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_destroyX11Window(
    nh_wsi_X11Window *Window_p)
{
    XUnmapWindow(NH_WSI_X11.Display_p, Window_p->Handle);
    XDestroyWindow(NH_WSI_X11.Display_p, Window_p->Handle);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setX11ClipboardOwner(
    nh_wsi_X11Window *Window_p)
{
    XSetSelectionOwner(NH_WSI_X11.Display_p, NH_WSI_X11.Atoms.CLIPBOARD, Window_p->Handle, CurrentTime);

    return NH_API_SUCCESS;
}

bool nh_wsi_isX11ClipboardOwner(
    nh_wsi_X11Window *Window_p)
{
    Window window = XGetSelectionOwner(NH_WSI_X11.Display_p, NH_WSI_X11.Atoms.CLIPBOARD);
    return Window_p->Handle == window;
}

NH_API_RESULT nh_wsi_requestX11ClipboardConversion(
    nh_wsi_X11Window *Window_p)
{
    XConvertSelection(NH_WSI_X11.Display_p, NH_WSI_X11.Atoms.CLIPBOARD, NH_WSI_X11.Atoms.UTF8_STRING, 
        NH_WSI_X11.Atoms.PENGUIN, Window_p->Handle, CurrentTime);

    return NH_API_SUCCESS;
}
