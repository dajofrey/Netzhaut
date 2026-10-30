#ifndef NH_API_NH_WSI_H
#define NH_API_NH_WSI_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include <stdalign.h>

#include "nh-core.h"
#include "nh-encoding.h"

// ENUMS ===========================================================================================

typedef enum NH_API_WSI_EVENT_E {
    NH_API_WSI_EVENT_MOUSE,
    NH_API_WSI_EVENT_KEYBOARD,
    NH_API_WSI_EVENT_WINDOW,
    NH_API_WSI_EVENT_STYLUS,
    NH_API_WSI_EVENT_GESTURE,
} NH_API_WSI_EVENT_E;

typedef enum NH_API_WINDOW_E {
    NH_API_WINDOW_CONFIGURE,
    NH_API_WINDOW_FOCUS_IN,
    NH_API_WINDOW_FOCUS_OUT,
} NH_API_WINDOW_E;

typedef enum NH_API_TRIGGER_E {
    NH_API_TRIGGER_UNDEFINED,
    NH_API_TRIGGER_PRESS,
    NH_API_TRIGGER_RELEASE,
    NH_API_TRIGGER_UP,
    NH_API_TRIGGER_DOWN,
    NH_API_TRIGGER_LEFT,
    NH_API_TRIGGER_RIGHT,
    NH_API_TRIGGER_MOVE,
    NH_API_TRIGGER_GET_FOCUS,
    NH_API_TRIGGER_LOOSE_FOCUS,
    NH_API_TRIGGER_CANCEL,
} NH_API_TRIGGER_E;

typedef enum NH_API_KEY_E {
    NH_API_KEY_NONE,
    // TTY function keys
    NH_API_KEY_BACKSPACE,
    NH_API_KEY_TAB,  
    NH_API_KEY_LINEFEED,  
    NH_API_KEY_CLEAR,  
    NH_API_KEY_RETURN,  
    NH_API_KEY_PAUSE,  
    NH_API_KEY_SCROLL_LOCK,
    NH_API_KEY_SYS_REQ,  
    NH_API_KEY_ESCAPE,  
    NH_API_KEY_DELETE,    
    // Modifiers
    NH_API_KEY_SHIFT_L, 
    NH_API_KEY_SHIFT_R,   
    NH_API_KEY_CONTROL_L, 
    NH_API_KEY_CONTROL_R,
    NH_API_KEY_CAPS_LOCK, 
    NH_API_KEY_SHIFT_LOCK,
    NH_API_KEY_META_L,    
    NH_API_KEY_META_R,    
    NH_API_KEY_ALT_L,     
    NH_API_KEY_ALT_R,     
    NH_API_KEY_SUPER_L,   
    NH_API_KEY_SUPER_R,   
    NH_API_KEY_HYPER_L,   
    NH_API_KEY_HYPER_R,  
    // Cursor control & motion
    NH_API_KEY_HOME,
    NH_API_KEY_LEFT,
    NH_API_KEY_UP,
    NH_API_KEY_RIGHT,
    NH_API_KEY_DOWN, 
    NH_API_KEY_PRIOR,
    NH_API_KEY_PAGE_UP,
    NH_API_KEY_NEXT,
    NH_API_KEY_PAGE_DOWN,
    NH_API_KEY_END,
    NH_API_KEY_BEGIN,
    // Misc functions
    NH_API_KEY_SELECT,
    NH_API_KEY_PRINT,
    NH_API_KEY_EXECUTE,
    NH_API_KEY_INSERT,
    NH_API_KEY_UNDO,
    NH_API_KEY_REDO,
    NH_API_KEY_MENU,
    NH_API_KEY_FIND,
    NH_API_KEY_CANCEL,
    NH_API_KEY_HELP,
    NH_API_KEY_BREAK,
    NH_API_KEY_MODE_SWITCH,
    NH_API_KEY_SCRIPT_SWITCH,
    NH_API_KEY_NUM_LOCK,
    // Keypad functions
    NH_API_KEY_KP_SPACE,
    NH_API_KEY_KP_TAB,
    NH_API_KEY_KP_ENTER,
    NH_API_KEY_KP_F1,
    NH_API_KEY_KP_F2,
    NH_API_KEY_KP_F3,
    NH_API_KEY_KP_F4,
    NH_API_KEY_KP_HOME,
    NH_API_KEY_KP_LEFT,
    NH_API_KEY_KP_UP,
    NH_API_KEY_KP_RIGHT,
    NH_API_KEY_KP_DOWN,
    NH_API_KEY_KP_PRIOR,
    NH_API_KEY_KP_PAGE_UP,
    NH_API_KEY_KP_NEXT,
    NH_API_KEY_KP_PAGE_DOWN,
    NH_API_KEY_KP_END,
    NH_API_KEY_KP_BEGIN,
    NH_API_KEY_KP_INSERT,
    NH_API_KEY_KP_DELETE,
    NH_API_KEY_KP_EQUAL,
    NH_API_KEY_KP_MULTIPLY,
    NH_API_KEY_KP_ADD,
    NH_API_KEY_KP_SEPARATOR,
    NH_API_KEY_KP_SUBTRACT,
    NH_API_KEY_KP_DECIMAL,
    NH_API_KEY_KP_DIVIDE,
    NH_API_KEY_KP_0,
    NH_API_KEY_KP_1,
    NH_API_KEY_KP_2,
    NH_API_KEY_KP_3,
    NH_API_KEY_KP_4,
    NH_API_KEY_KP_5,
    NH_API_KEY_KP_6,
    NH_API_KEY_KP_7,
    NH_API_KEY_KP_8,
    NH_API_KEY_KP_9,
    NH_API_KEY_F1,
    NH_API_KEY_F2,
    NH_API_KEY_F3,
    NH_API_KEY_F4,
    NH_API_KEY_F5,
    NH_API_KEY_F6,
    NH_API_KEY_F7,
    NH_API_KEY_F8,
    NH_API_KEY_F9,
    NH_API_KEY_F10,
    NH_API_KEY_F11,
    NH_API_KEY_F12,
    NH_API_KEY_F13,
    NH_API_KEY_F14,
    NH_API_KEY_F15,
    NH_API_KEY_F16,
    NH_API_KEY_F17,
    NH_API_KEY_F18,
    NH_API_KEY_F19,
    NH_API_KEY_F20,
    NH_API_KEY_F21,
    NH_API_KEY_F22,
    NH_API_KEY_F23,
    NH_API_KEY_F24,
    NH_API_KEY_F25,
    NH_API_KEY_F26,
    NH_API_KEY_F27,
    NH_API_KEY_F28,
    NH_API_KEY_F29,
    NH_API_KEY_F30,
    NH_API_KEY_F31,
    NH_API_KEY_F32,
    NH_API_KEY_F33,
    NH_API_KEY_F34,
    NH_API_KEY_F35,
    // Apple
    NH_API_KEY_COMMAND_L,
    NH_API_KEY_COMMAND_R,
    NH_API_KEY_OPTION_L,
    NH_API_KEY_OPTION_R,
} NH_API_KEY_E;

typedef enum NH_API_MOUSE_E {
    NH_API_MOUSE_UNDEFINED,
    NH_API_MOUSE_LEFT,
    NH_API_MOUSE_RIGHT,
    NH_API_MOUSE_MIDDLE,
    NH_API_MOUSE_SCROLL,
    NH_API_MOUSE_MOVE,
} NH_API_MOUSE_E;

typedef enum NH_API_MOUSE_FLAG {
    NH_API_MOUSE_FLAG_UNDEFINED = 1 << 0,
    NH_API_MOUSE_FLAG_LEFT      = 1 << 1,
    NH_API_MOUSE_FLAG_RIGHT     = 1 << 2,
    NH_API_MOUSE_FLAG_MIDDLE    = 1 << 3,
    NH_API_MOUSE_FLAG_SCROLL    = 1 << 4,
    NH_API_MOUSE_FLAG_MOVE      = 1 << 5,
} NH_API_MOUSE_FLAG;

typedef enum NH_API_MODIFIER_FLAG {
    NH_API_MODIFIER_SHIFT   = 1 << 0, 
    NH_API_MODIFIER_LOCK    = 1 << 1,
    NH_API_MODIFIER_CONTROL = 1 << 2,
    NH_API_MODIFIER_MOD1    = 1 << 3,
    NH_API_MODIFIER_MOD2    = 1 << 4,
    NH_API_MODIFIER_MOD3    = 1 << 5,
    NH_API_MODIFIER_MOD4    = 1 << 6,
    NH_API_MODIFIER_MOD5    = 1 << 7,
} NH_API_MODIFIER_FLAG;

/**
 * E-paper waveforms, from fastest to cleanest.
 */
typedef enum NH_API_EPAPER_UPDATE_MODE_E {
    NH_API_EPAPER_UPDATE_MODE_DEFAULT = 0, // Platform default.
    NH_API_EPAPER_UPDATE_MODE_A2,          // Black/white only, fastest, ghosts. For panning, zooming, scrolling.
    NH_API_EPAPER_UPDATE_MODE_DU,          // Black/white only, fast, less ghosting than A2.
    NH_API_EPAPER_UPDATE_MODE_GU,          // Grayscale, no flash.
    NH_API_EPAPER_UPDATE_MODE_GC,          // Grayscale with flash, removes ghosting.
} NH_API_EPAPER_UPDATE_MODE_E;

// STRUCTS =========================================================================================

typedef struct nh_api_Rect {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} nh_api_Rect;

typedef struct NH_ALIGN_16 nh_api_PixelPosition { 
    int x; 
    int y; 
} nh_api_PixelPosition; 

typedef struct NH_ALIGN_16 nh_api_PixelSize { 
    int width; 
    int height; 
} nh_api_PixelSize; 

typedef struct NH_ALIGN_16 nh_api_WindowEvent {
    NH_API_WINDOW_E type;
    nh_api_PixelPosition Position;
    nh_api_PixelSize PixelSize;
} nh_api_WindowEvent;

typedef struct NH_ALIGN_16 nh_api_MouseEvent {
    NH_API_MOUSE_E type;
    NH_API_TRIGGER_E trigger;
    nh_api_PixelPosition Position;
} nh_api_MouseEvent;

typedef struct NH_ALIGN_16 nh_api_KeyboardEvent {
    NH_API_KEY_E special;
    NH_API_UTF32 codepoint;
    NH_API_TRIGGER_E trigger;
    NH_API_MODIFIER_FLAG state;
} nh_api_KeyboardEvent;

typedef struct NH_ALIGN_16 nh_api_StylusEvent {
    NH_API_TRIGGER_E trigger;
    float x;
    float y;
    float pressure;
} nh_api_StylusEvent;

/**
 * Combined pan/pinch touch gesture.
 * trigger: PRESS (begin), MOVE (update), RELEASE (end) or CANCEL (e.g. palm or stylus contact,
 * receivers should revert changes made since PRESS).
 * x/y is the current focus point (centroid of all fingers) in window pixels, deltaX/deltaY the
 * focus movement and scale the relative span change, both since the previous gesture event.
 */
typedef struct NH_ALIGN_16 nh_api_GestureEvent {
    NH_API_TRIGGER_E trigger;
    int pointers;
    float x;
    float y;
    float deltaX;
    float deltaY;
    float scale;
} nh_api_GestureEvent;

typedef struct NH_ALIGN_16 nh_api_WSIEvent {
    NH_API_WSI_EVENT_E type;
    nh_api_WindowEvent Window;
    nh_api_MouseEvent Mouse;
    nh_api_KeyboardEvent Keyboard;
    nh_api_StylusEvent Stylus;
    nh_api_GestureEvent Gesture;
} nh_api_WSIEvent;

typedef struct nh_api_SurfaceRequirements nh_api_SurfaceRequirements;
typedef struct nh_api_Window nh_api_Window;

typedef void (*nh_api_windowCallback_f)(nh_api_Window *Window_p, nh_api_WSIEvent Event) __attribute__((aligned(16)));

// FUNCTIONS =======================================================================================

nh_api_Window *nh_api_createWindow(
    char *namespace_p, nh_api_SurfaceRequirements *Requirements_p
);

#if defined(__ANDROID__)
struct android_app;
/**
 * Register the android_native_app_glue application after nh_api_initialize()
 * and before creating a Netzhaut window. Call after assigning the host's
 * app callbacks; Netzhaut chains those callbacks and receives lifecycle/input
 * events from the host-owned NativeActivity event loop. Create the Netzhaut
 * window from the host's APP_CMD_INIT_WINDOW callback. Android applications
 * using the EGL backend must configure nh-gfx.api as "opengl".
 */
NH_API_RESULT nh_api_setAndroidApp(
    struct android_app *app_p
);
#endif

NH_API_RESULT nh_api_setWindowEventListener(
    nh_api_Window *Window_p, nh_api_windowCallback_f callback_f
);

NH_API_RESULT nh_api_moveWindow(
    nh_api_Window *Window_p
);

NH_API_RESULT nh_api_showKeyboard(
    nh_api_Window *Window_p
);

NH_API_RESULT nh_api_hideKeyboard(
    nh_api_Window *Window_p
);

// E-PAPER FUNCTIONS ===============================================================================

NH_API_RESULT nh_api_setEPaperRawDrawingEnabled(
    nh_api_Window *Window_p, bool enabled
);

NH_API_RESULT nh_api_setEPaperRawDrawingRenderEnabled(
    nh_api_Window *Window_p, bool enabled
);

NH_API_RESULT nh_api_setEPaperRawDrawingLimitRect(
    nh_api_Window *Window_p, int left, int top, int right, int bottom,
    const nh_api_Rect *excludeRects_p, int excludeCount
);

NH_API_RESULT nh_api_setEPaperRawDrawingStrokeWidth(
    nh_api_Window *Window_p, float width
);

NH_API_RESULT nh_api_setEPaperRawDrawingStrokeStyle(
    nh_api_Window *Window_p, int style
);

NH_API_RESULT nh_api_setEPaperRawDrawingEraserEnabled(
    nh_api_Window *Window_p, bool enabled
);

/**
 * Selects the e-paper waveform used for all following screen updates of the window, until
 * NH_API_EPAPER_UPDATE_MODE_DEFAULT restores the platform default. Use a fast mode only for
 * the duration of an interaction like panning or zooming and request a refresh afterwards to
 * remove ghosting. mode is a NH_API_EPAPER_UPDATE_MODE_E value.
 */
NH_API_RESULT nh_api_setEPaperUpdateMode(
    nh_api_Window *Window_p, int mode
);

/**
 * Redraws the given window rectangle (window pixels, right/bottom exclusive) with the given
 * NH_API_EPAPER_UPDATE_MODE_E waveform.
 */
NH_API_RESULT nh_api_requestEPaperRefreshRect(
    nh_api_Window *Window_p, int left, int top, int right, int bottom, int mode
);

#endif // NH_API_NH_WSI_H
