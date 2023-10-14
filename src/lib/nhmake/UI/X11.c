// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "X11.h"
#include "GUI.h"

#include "../Main/Util.h"
#include "../Common/Macro.h"

#include NH_MAKE_FLOW
#include NH_MAKE_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __unix__
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xutil.h>
#endif

// DATA ============================================================================================

static nh_make_X11 Data;

// FOR SOFTWARE RENDERING :
//
// #include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <unistd.h>
// #include <malloc.h>
// #include <stdio.h>
// 
// int main(int argc,char **argv)
// {
// 	Display *display = XOpenDisplay(NULL);
// 	int screen_num = DefaultScreen(display);
// 	Window root = RootWindow(display,screen_num);
// 	Visual *visual = DefaultVisual(display,screen_num);
// 
// 	char *data = (char*)malloc(256*256*4);
//         for (int i = 0; i < 256*256*4; ++i) {data[i] = 0;}
//         for (int i = 0; i < 10000; ++i) {if (i % 4 == 1) {data[i] = 255;}}
// 
// 
// 	XImage *img = XCreateImage(display,visual,DefaultDepth(display,screen_num),ZPixmap,
// 		0,data,256,256,32,0);
// 	fprintf(stdout,"end\n");
// 
// 	Window win = XCreateSimpleWindow(display,root,50,50,256,256,1,0,0);
// 	XSelectInput(display,win,ExposureMask);
// 	XMapWindow(display,win);
// 
// 	XEvent event;
// 	while(1)
// 	{
// 		XNextEvent(display,&event);
// 		if(event.type == Expose)
// 		{
// 			XPutImage(display,win,DefaultGC(display,screen_num),img,0,0,0,0,256,256);
// 		}
// 	}
// 	sleep(10);
// 	XCloseDisplay(display);
// 	return 0;
// }

// CREATE WINDOW ===================================================================================

static NH_MAKE_RESULT nh_make_connectToDisplay()
{
NH_MAKE_BEGIN()

    Data.Display_p = XOpenDisplay(NULL);
    NH_MAKE_CHECK_NULL(Data.Display_p)
    Data.screen = DefaultScreen(Data.Display_p);
    Data.Root = RootWindow(Data.Display_p, Data.screen);
    Data.white = WhitePixel(Data.Display_p, Data.screen);
    Data.black = BlackPixel(Data.Display_p, Data.screen);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_createSimpleWindow(
    int width, int height)
{
NH_MAKE_BEGIN()

    Data.Window = XCreateSimpleWindow (
        Data.Display_p, Data.Root, 1, 1, width, height, 0, Data.black, Data.white 
    );
    XSelectInput(
        Data.Display_p, Data.Window, StructureNotifyMask | KeyPressMask | KeyReleaseMask | 
        ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | 
        LeaveWindowMask | FocusChangeMask | ExposureMask
    );

    XMapWindow(Data.Display_p, Data.Window);

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static void nh_make_createGraphicsContext()
{
    Data.Gfx = XCreateGC(Data.Display_p, Data.Window, 0, 0);
    XSetBackground(Data.Display_p, Data.Gfx, Data.white); 
    XSetForeground(Data.Display_p, Data.Gfx, Data.black); 
}

static void nh_make_createFont()
{
    const char *fontname_p = "-*-helvetica-*-r-*-*-14-*-*-*-*-*-*-*";
    Data.Font_p = XLoadQueryFont(Data.Display_p, fontname_p);
    /* If the font could not be loaded, revert to the "fixed" font. */
    if (!Data.Font_p) {
        fprintf (stderr, "unable to load font %s: using fixed\n", fontname_p);
        Data.Font_p = XLoadQueryFont(Data.Display_p, "fixed");
    }
    XSetFont(Data.Display_p, Data.Gfx, Data.Font_p->fid);
}

NH_MAKE_RESULT nh_make_X11_createWindow(
    char *title_p, int width, int height)
{
NH_MAKE_BEGIN()

#ifdef __unix__

    NH_MAKE_CHECK(nh_make_connectToDisplay())

// keyboard
    Data.Connection_p = XGetXCBConnection(Data.Display_p);
    Data.Context_p    = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    xkb_x11_setup_xkb_extension(
        Data.Connection_p, XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, 
        0, NULL, NULL, NULL, NULL
    ); 
    Data.keyboardDeviceID = xkb_x11_get_core_keyboard_device_id(Data.Connection_p);
    Data.Keymap_p = xkb_x11_keymap_new_from_device(
        Data.Context_p, 
        Data.Connection_p, 
        Data.keyboardDeviceID, 
        XKB_KEYMAP_COMPILE_NO_FLAGS
    );
    Data.State_p = xkb_x11_state_new_from_device(
        Data.Keymap_p, Data.Connection_p, Data.keyboardDeviceID
    );

// create window
    nh_make_createSimpleWindow(width, height);
    nh_make_createGraphicsContext();
    nh_make_createFont();

// window delete protocol
    Data.WM_DELETE_WINDOW = XInternAtom(Data.Display_p, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(Data.Display_p, Data.Window, &Data.WM_DELETE_WINDOW, 1);

// xdnd (file-)drop
    Atom xdndAware = XInternAtom(Data.Display_p, "XdndAware", 0); 
    Atom xdndVersion = 5;
    XChangeProperty(
        Data.Display_p, Data.Window, xdndAware, XA_ATOM, 32,
        PropModeReplace, (unsigned char*)&xdndVersion, 1
    );

// window name   
    XStoreName(Data.Display_p, Data.Window, title_p);

// center window
    Screen *Screen_p = ScreenOfDisplay(Data.Display_p, Data.screen);
    XMoveWindow(Data.Display_p, Data.Window, (int)(Screen_p->width/2) - (int)(width/2), (int)(Screen_p->height/2) - (int)(height/2));
    XSync(Data.Display_p, 0);

    NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)

#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)
}

// DESTROY WINDOW ==================================================================================

NH_MAKE_RESULT nh_make_X11_destroyWindow()
{
NH_MAKE_BEGIN()

#ifdef __unix__

    XDestroyWindow(Data.Display_p, Data.Window);
    XCloseDisplay(Data.Display_p);
    xkb_state_unref(Data.State_p);
    xkb_keymap_unref(Data.Keymap_p);
    xkb_context_unref(Data.Context_p);

    NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)

#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)
}

// INPUT ===========================================================================================

NH_MAKE_RESULT nh_make_X11_getInput() 
{
NH_MAKE_BEGIN()

#ifdef __unix__
    
    XEvent Event;

    Atom XdndEnter      = XInternAtom(Data.Display_p, "XdndEnter", False);
    Atom XdndPosition   = XInternAtom(Data.Display_p, "XdndPosition", False);
    Atom XdndStatus     = XInternAtom(Data.Display_p, "XdndStatus", False);
    Atom XdndTypeList   = XInternAtom(Data.Display_p, "XdndTypeList", False);
    Atom XdndActionCopy = XInternAtom(Data.Display_p, "XdndActionCopy", False);
    Atom XdndDrop       = XInternAtom(Data.Display_p, "XdndDrop", False);
    Atom XdndLeave      = XInternAtom(Data.Display_p, "XdndLeave", False);
    Atom XdndFinished   = XInternAtom(Data.Display_p, "XdndFinished", False);
    Atom XdndSelection  = XInternAtom(Data.Display_p, "XdndSelection", False);
    Atom XdndProxy      = XInternAtom(Data.Display_p, "XdndProxy", False);
    Atom sel            = XInternAtom(Data.Display_p, "PRIMARY", 0);

    int xdndVersion = 0;
    Window xdndSourceWindow = None;

    while (XPending(Data.Display_p) > 0) 
    {
        XNextEvent(Data.Display_p, &Event);
        
        if (Event.type == ClientMessage)
        {   
                 if (Event.xclient.data.l[0] == Data.WM_DELETE_WINDOW) {nh_make_handleGUIExit();}
            else if (Event.type == Expose) {}
            else if (Event.xclient.message_type == XdndEnter) 
            {
                xdndVersion = (Event.xclient.data.l[1] >> 24);
            }
            else if (Event.xclient.message_type == XdndDrop)
            {
		xdndSourceWindow = Event.xclient.data.l[0];
                Atom string = XInternAtom(Data.Display_p, "STRING", 0);
		
                if (xdndVersion >= 1) {
		    XConvertSelection(Data.Display_p, XdndSelection, string, sel, Data.Window, Event.xclient.data.l[2]);
                } else {
                    XConvertSelection(Data.Display_p, XdndSelection, string, sel, Data.Window, CurrentTime);
                }
            }
            else if (Event.xclient.message_type == XdndPosition)
            {
		Atom action = XdndActionCopy;
		if(xdndVersion >= 2) {action = Event.xclient.data.l[4];}
		
		XClientMessageEvent m;
		memset(&m, sizeof(m), 0);
		m.type         = ClientMessage;
		m.display      = Event.xclient.display;
		m.window       = Event.xclient.data.l[0];
		m.message_type = XdndStatus;
		m.format       = 32;
		
                m.data.l[0] = Data.Window;
		m.data.l[1] = 1;
		m.data.l[2] = 0; //Specify an empty rectangle
		m.data.l[3] = 0;
		m.data.l[4] = XdndActionCopy; //We only accept copying anyway.

		XSendEvent(Data.Display_p, Event.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                XFlush(Data.Display_p);
            }
            else if (Event.xclient.message_type == XdndLeave) {}
        }

        else if (Event.type == ConfigureNotify) 
        {
            XConfigureEvent Configure = Event.xconfigure;
            Data.width  = Configure.width;
            Data.height = Configure.height;
        }

        else if (Event.type == MotionNotify)
        {
        }
        
        else if (Event.type == ButtonPress || Event.type == ButtonRelease)
        {
        }
        
        else if (Event.type == KeyPress || Event.type == KeyRelease)
        {
        }
        
        else if (Event.type == FocusIn || Event.type == FocusOut)
        {
        }
        
        else if (Event.type == SelectionNotify)
        {
        }

        else if (Event.type == Expose) {
            NH_MAKE_CHECK(nh_make_handleGUIExpose())
        }
    }

    NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)

#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)
}

// DRAW ============================================================================================

void nh_make_X11_drawLine(
    int x1, int y1, int x2, int y2)
{
NH_MAKE_BEGIN()

    XDrawLine(Data.Display_p, Data.Window, Data.Gfx, x1, y1, x2, y2);

NH_MAKE_SILENT_END()
}

void nh_make_X11_fillRectangle(
   int x, int y, int width, int height)
{
NH_MAKE_BEGIN()

    XFillRectangle(Data.Display_p, Data.Window, Data.Gfx, x, y, width, height);

NH_MAKE_SILENT_END()
}

void nh_make_X11_drawTextLine(
    int x, int y, char *text_p)
{
NH_MAKE_BEGIN()

    int direction;
    int ascent;
    int descent;
    XCharStruct overall;

    XTextExtents(Data.Font_p, text_p, strlen(text_p), &direction, &ascent, &descent, &overall);

    int count = 0;
    if (overall.width + x > Data.width) 
    {
        char *reduced_p = malloc(sizeof(char) * (strlen(text_p) + 1));
        strcpy(reduced_p, text_p);

        for (int i = 0; i < strlen(reduced_p); ++i) 
        {
            char reverse = reduced_p[i]; 
            reduced_p[i] = '\0';
            XTextExtents(Data.Font_p, reduced_p, strlen(reduced_p), &direction, &ascent, &descent, &overall);
            reduced_p[i] = reverse;
            if (overall.width + x > Data.width) {
                count = i - 1;
                break;
            }
        }
        free(reduced_p);
        if (count == -1) {return;}
    }

    y += ascent;

    XDrawString(Data.Display_p, Data.Window, Data.Gfx, x, y, text_p, strlen(text_p));
    if (count != 0) {nh_make_X11_drawTextLine(x, y + descent, text_p + count);}

NH_MAKE_SILENT_END()
}

void nh_make_X11_clearWindow()
{
NH_MAKE_BEGIN()

    XClearWindow(Data.Display_p, Data.Window);

NH_MAKE_SILENT_END()
}

