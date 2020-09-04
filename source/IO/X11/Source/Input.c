// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Input.h"
#include "../Header/Macros.h"

#include "../../Header/Log.h"
#include "../../Header/Mouse.h"
#include "../../Header/Keyboard.h"

#include NH_DEBUG

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// DECLARE =========================================================================================

#ifdef __unix__

    typedef struct Property {
        unsigned char *data;
        int format, nitems;
        Atom type;
    } Property;
    
    static Property readProperty(
        Display* Display_p, Window Window, Atom property
    );

#endif

// IMPLEMENT =======================================================================================

NH_RESULT NH_IO_X11_getInput(
    Nh_Window *Window_p) 
{
NH_BEGIN()

    NH_IO_Input *Input_p    = &Window_p->Input;
    NH_IO_X11_Window *X11_p = &Window_p->WSI.X11;

#ifdef __unix__
    
    XEvent event;
    Display* Display_p      = X11_p->Display_p;
    struct xkb_state* state = X11_p->State_p;

    Atom XdndEnter      = XInternAtom(Display_p, "XdndEnter", False);
    Atom XdndPosition   = XInternAtom(Display_p, "XdndPosition", False);
    Atom XdndStatus     = XInternAtom(Display_p, "XdndStatus", False);
    Atom XdndTypeList   = XInternAtom(Display_p, "XdndTypeList", False);
    Atom XdndActionCopy = XInternAtom(Display_p, "XdndActionCopy", False);
    Atom XdndDrop       = XInternAtom(Display_p, "XdndDrop", False);
    Atom XdndLeave      = XInternAtom(Display_p, "XdndLeave", False);
    Atom XdndFinished   = XInternAtom(Display_p, "XdndFinished", False);
    Atom XdndSelection  = XInternAtom(Display_p, "XdndSelection", False);
    Atom XdndProxy      = XInternAtom(Display_p, "XdndProxy", False);
    Atom sel            = XInternAtom(Display_p, "PRIMARY", 0);

    int xdndVersion = 0;
    Window xdndSourceWindow = None;

    while (XPending(Display_p) > 0) 
    {
        XNextEvent(Display_p, &event);
        
        if (event.type == ClientMessage)
        {   
                 if (event.xclient.data.l[0] == X11_p->DeleteAtom) {Window_p->Flags.close = true; NH_END(NH_SUCCESS)}
            else if (event.type == Expose) {}
            else if (event.xclient.message_type == XdndEnter) 
            {
                xdndVersion = (event.xclient.data.l[1] >> 24);
            }
            else if (event.xclient.message_type == XdndDrop)
            {
		xdndSourceWindow = event.xclient.data.l[0];
                Atom string = XInternAtom(Display_p, "STRING", 0);
		
                if (xdndVersion >= 1) {
		    XConvertSelection(Display_p, XdndSelection, string, sel, X11_p->XWindow, event.xclient.data.l[2]);
                } else {
                    XConvertSelection(Display_p, XdndSelection, string, sel, X11_p->XWindow, CurrentTime);
                }
            }
            else if (event.xclient.message_type == XdndPosition)
            {
		Atom action = XdndActionCopy;
		if(xdndVersion >= 2) {action = event.xclient.data.l[4];}
		
		XClientMessageEvent m;
		memset(&m, sizeof(m), 0);
		m.type         = ClientMessage;
		m.display      = event.xclient.display;
		m.window       = event.xclient.data.l[0];
		m.message_type = XdndStatus;
		m.format       = 32;
		
                m.data.l[0] = X11_p->XWindow;
		m.data.l[1] = 1;
		m.data.l[2] = 0; //Specify an empty rectangle
		m.data.l[3] = 0;
		m.data.l[4] = XdndActionCopy; //We only accept copying anyway.

		XSendEvent(Display_p, event.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                XFlush(Display_p);
            }
            else if (event.xclient.message_type == XdndLeave) {}
        }

        else if (event.type == ConfigureNotify) {}
        else if (event.type == MotionNotify)
        {
            NH_IO_MouseEvent *Event_p = Nh_advanceRingIterator(&Input_p->Mouse.Events);
           
            Event_p->position_p[0] = event.xbutton.x;
            Event_p->position_p[1] = event.xbutton.y;
            Event_p->trigger       = NH_IO_TRIGGER_MOVE;
            Event_p->type          = NH_IO_MOUSE_FLAG_MOVE;

            Nh_IO_logMouse(Input_p);
        }
        
        else if (event.type == ButtonPress || event.type == ButtonRelease)
        {
            NH_IO_MouseEvent *Event_p = Nh_advanceRingIterator(&Input_p->Mouse.Events);

            NH_IO_TRIGGER trigger = event.type == ButtonPress ? NH_IO_TRIGGER_PRESS : NH_IO_TRIGGER_RELEASE;
            NH_IO_MOUSE button = 0;

            switch (event.xbutton.button)
            {
                case Button1 : button = NH_IO_MOUSE_FLAG_LEFT; break;
                case Button2 : button = NH_IO_MOUSE_FLAG_MIDDLE; break;
                case Button3 : button = NH_IO_MOUSE_FLAG_RIGHT; break;
                case Button4 : button = NH_IO_MOUSE_FLAG_SCROLL; trigger = NH_IO_TRIGGER_UP; break;
                case Button5 : button = NH_IO_MOUSE_FLAG_SCROLL; trigger = NH_IO_TRIGGER_DOWN; break;
            }

            Event_p->position_p[0] = event.xbutton.x;
            Event_p->position_p[1] = event.xbutton.y;
            Event_p->trigger       = trigger;
            Event_p->type          = button;

            Nh_IO_logMouse(Input_p);
        }
        
        else if (event.type == KeyPress || event.type == KeyRelease)
        {
            NH_IO_KeyboardEvent *Event_p = Nh_advanceRingIterator(&Input_p->Keyboard.Events);

            NH_IO_TRIGGER event_ = event.type == KeyPress ? NH_IO_TRIGGER_PRESS : NH_IO_TRIGGER_RELEASE;
            NH_IO_KEY key = 0;

            // get key
            switch (XLookupKeysym(&event.xkey, 0))
            {
                // TTY function keys
                case XKB_KEY_BackSpace    : break;
                case XKB_KEY_Tab          : break;
                case XKB_KEY_Linefeed     : break;
                case XKB_KEY_Clear        : break;
                case XKB_KEY_Return       : break;
                case XKB_KEY_Pause        : break; 
                case XKB_KEY_Scroll_Lock  : break;
                case XKB_KEY_Sys_Req      : break;
                case XKB_KEY_Escape       : break;
                case XKB_KEY_Delete       : break;

                // Cursor control & motion    
                case XKB_KEY_Home         : break;
                case XKB_KEY_Left         : break;      
                case XKB_KEY_Up           : break;               
                case XKB_KEY_Right        : break;
                case XKB_KEY_Down         : break;
                case XKB_KEY_Prior        : break; // same as XKB_KEY_Page_Up
                case XKB_KEY_Next         : break; // same as XKB_KEY_Page_Down
                case XKB_KEY_End          : break;
                case XKB_KEY_Begin        : break;

                // Modifiers
                case XKB_KEY_Shift_L      : key = NH_IO_KEY_SHIFT_L; break; 
                case XKB_KEY_Shift_R      : key = NH_IO_KEY_SHIFT_R; break;   
                case XKB_KEY_Control_L    : key = NH_IO_KEY_CONTROL_L; break; 
                case XKB_KEY_Control_R    : break;
                case XKB_KEY_Caps_Lock    : break; 
                case XKB_KEY_Shift_Lock   : break;
                case XKB_KEY_Meta_L       : break;    
                case XKB_KEY_Meta_R       : break;    
                case XKB_KEY_Alt_L        : break;     
                case XKB_KEY_Alt_R        : break;     
                case XKB_KEY_Super_L      : break;   
                case XKB_KEY_Super_R      : break;   
                case XKB_KEY_Hyper_L      : break;   
                case XKB_KEY_Hyper_R      : break;   

                // Latin 1 (ISO/*IEC 8859-1 = Unicode U+0020..U+00FF)
                case XKB_KEY_space        : break;        
                case XKB_KEY_exclam       : break;       
                case XKB_KEY_quotedbl     : break;     
                case XKB_KEY_numbersign   : break;   
                case XKB_KEY_dollar       : break;       
                case XKB_KEY_percent      : break;      
                case XKB_KEY_ampersand    : break;    
                case XKB_KEY_apostrophe   : break;   
                case XKB_KEY_parenleft    : break;    
                case XKB_KEY_parenright   : break;   
                case XKB_KEY_asterisk     : break;     
                case XKB_KEY_plus         : break;         
                case XKB_KEY_comma        : break;        
                case XKB_KEY_minus        : break;        
                case XKB_KEY_period       : break;       
                case XKB_KEY_slash        : break;        
                case XKB_KEY_0            : break;            
                case XKB_KEY_1            : break;            
                case XKB_KEY_2            : break;            
                case XKB_KEY_3            : break;            
                case XKB_KEY_4            : break;            
                case XKB_KEY_5            : break;            
                case XKB_KEY_6            : break;            
                case XKB_KEY_7            : break;            
                case XKB_KEY_8            : break;            
                case XKB_KEY_9            : break;            
                case XKB_KEY_colon        : break;        
                case XKB_KEY_semicolon    : break;    
                case XKB_KEY_less         : break;         
                case XKB_KEY_equal        : break;        
                case XKB_KEY_greater      : break;      
                case XKB_KEY_question     : break;     
                case XKB_KEY_at           : break;           
                case XKB_KEY_A            : break;
                case XKB_KEY_B            : break;
                case XKB_KEY_C            : break;
                case XKB_KEY_D            : break;
                case XKB_KEY_E            : break;
                case XKB_KEY_F            : break;
                case XKB_KEY_G            : break;
                case XKB_KEY_H            : break;
                case XKB_KEY_I            : break;
                case XKB_KEY_J            : break;
                case XKB_KEY_K            : break;
                case XKB_KEY_L            : break;
                case XKB_KEY_M            : break;
                case XKB_KEY_N            : break;
                case XKB_KEY_O            : break;
                case XKB_KEY_P            : break;
                case XKB_KEY_Q            : break;
                case XKB_KEY_R            : break;
                case XKB_KEY_S            : break;
                case XKB_KEY_T            : break;
                case XKB_KEY_U            : break;
                case XKB_KEY_V            : break;
                case XKB_KEY_W            : break;
                case XKB_KEY_X            : break;
                case XKB_KEY_Y            : break;
                case XKB_KEY_Z            : break;
                case XKB_KEY_bracketleft  : break;  
                case XKB_KEY_backslash    : break;    
                case XKB_KEY_bracketright : break; 
                case XKB_KEY_asciicircum  : break;  
                case XKB_KEY_underscore   : break;   
                case XKB_KEY_grave        : break;        
                case XKB_KEY_a            : break;            
                case XKB_KEY_b            : break;            
                case XKB_KEY_c            : break;            
                case XKB_KEY_d            : break;            
                case XKB_KEY_e            : break;            
                case XKB_KEY_f            : break;            
                case XKB_KEY_g            : break;            
                case XKB_KEY_h            : break;            
                case XKB_KEY_i            : break;            
                case XKB_KEY_j            : break;            
                case XKB_KEY_k            : break;            
                case XKB_KEY_l            : break;            
                case XKB_KEY_m            : break;            
                case XKB_KEY_n            : break;            
                case XKB_KEY_o            : key = NH_IO_KEY_SMALL_O; break;
                case XKB_KEY_p            : break;
                case XKB_KEY_q            : break;
                case XKB_KEY_r            : key = NH_IO_KEY_SMALL_R; break;
                case XKB_KEY_s            : break;
                case XKB_KEY_t            : break;
                case XKB_KEY_u            : break;
                case XKB_KEY_v            : break;
                case XKB_KEY_w            : break;
                case XKB_KEY_x            : break;
                case XKB_KEY_y            : break;
                case XKB_KEY_z            : break;
                case XKB_KEY_braceleft    : break; 
                case XKB_KEY_bar          : break;       
                case XKB_KEY_braceright   : break;
                case XKB_KEY_asciitilde   : break; 
            }

            Event_p->key     = key;
            Event_p->trigger = event_;

            Input_p->Keyboard.state_p[key] = event_ == NH_IO_TRIGGER_PRESS;
        }
        
        else if (event.type == FocusIn || event.type == FocusOut)
        {
        }
        
        else if (event.type == SelectionNotify)
        {
            Atom target = event.xselection.target;
	    
            if(event.xselection.property == None) {continue;}
            Property prop = readProperty(Display_p, X11_p->XWindow, sel); 
            
//            NH_drop(prop.data, Window_p);

            XClientMessageEvent m;
	    memset(&m, sizeof(m), 0);
	    m.type         = ClientMessage;
	    m.display      = Display_p;
	    m.window       = event.xclient.data.l[0];
	    m.message_type = XdndFinished;
	    m.format    = 32;
	    m.data.l[0] = X11_p->XWindow;
	    m.data.l[1] = 1;
	    m.data.l[2] = XdndActionCopy;

	    XSendEvent(Display_p, event.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
            XSync(Display_p, False);
        }
    }

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

// HELPER ==========================================================================================

#ifdef __unix__

    static Property readProperty(
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
        
        Property p = {ret, actual_format, nitems, actual_type};
        
        return p;
    }

#endif // __unix__

