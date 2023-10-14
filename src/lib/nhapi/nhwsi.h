#ifndef NH_WSI_H
#define NH_WSI_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhcore.h"
#include "nhencoding.h"

#endif

/** @defgroup api_nhwsi nhwsi
 *  @brief Create windows for rendering.
 */

/** @addtogroup api_nhwsi
 *  @{
 */

// DEFINES =========================================================================================

#define NH_WSI_DEFAULT_WINDOW_WIDTH  1280
#define NH_WSI_DEFAULT_WINDOW_HEIGHT 720

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_WSI_RESULT_E {
        NH_WSI_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_WSI_ERROR_NULL_POINTER,
        NH_WSI_ERROR_BAD_STATE,
        NH_WSI_ERROR_MEMORY_ALLOCATION,
    } NH_WSI_RESULT_E;

    typedef enum NH_WSI_TYPE_E {
        NH_WSI_TYPE_X11,
        NH_WSI_TYPE_WAYLAND,
        NH_WSI_TYPE_WIN32,
    } NH_WSI_TYPE_E;

    typedef enum NH_WSI_EVENT_E {
        NH_WSI_EVENT_MOUSE,
        NH_WSI_EVENT_KEYBOARD,
        NH_WSI_EVENT_WINDOW,
    } NH_WSI_EVENT_E;

    typedef enum NH_WSI_WINDOW_E {
        NH_WSI_WINDOW_CONFIGURE,
        NH_WSI_WINDOW_FOCUS_IN,
        NH_WSI_WINDOW_FOCUS_OUT,
    } NH_WSI_WINDOW_E;

    typedef enum NH_WSI_TRIGGER_E {
        NH_WSI_TRIGGER_UNDEFINED,
        NH_WSI_TRIGGER_PRESS,
        NH_WSI_TRIGGER_RELEASE,
        NH_WSI_TRIGGER_UP,
        NH_WSI_TRIGGER_DOWN,
        NH_WSI_TRIGGER_MOVE,
        NH_WSI_TRIGGER_GET_FOCUS,
        NH_WSI_TRIGGER_LOOSE_FOCUS,
    } NH_WSI_TRIGGER_E;

    typedef enum NH_WSI_KEY_E {
        NH_WSI_KEY_NONE,
        // TTY function keys
        NH_WSI_KEY_BACKSPACE,
        NH_WSI_KEY_TAB,  
        NH_WSI_KEY_LINEFEED,  
        NH_WSI_KEY_CLEAR,  
        NH_WSI_KEY_RETURN,  
        NH_WSI_KEY_PAUSE,  
        NH_WSI_KEY_SCROLL_LOCK,
        NH_WSI_KEY_SYS_REQ,  
        NH_WSI_KEY_ESCAPE,  
        NH_WSI_KEY_DELETE,    
        // Modifiers
        NH_WSI_KEY_SHIFT_L, 
        NH_WSI_KEY_SHIFT_R,   
        NH_WSI_KEY_CONTROL_L, 
        NH_WSI_KEY_CONTROL_R,
        NH_WSI_KEY_CAPS_LOCK, 
        NH_WSI_KEY_SHIFT_LOCK,
        NH_WSI_KEY_META_L,    
        NH_WSI_KEY_META_R,    
        NH_WSI_KEY_ALT_L,     
        NH_WSI_KEY_ALT_R,     
        NH_WSI_KEY_SUPER_L,   
        NH_WSI_KEY_SUPER_R,   
        NH_WSI_KEY_HYPER_L,   
        NH_WSI_KEY_HYPER_R,  
        // Cursor control & motion
        NH_WSI_KEY_HOME,
        NH_WSI_KEY_LEFT,
        NH_WSI_KEY_UP,
        NH_WSI_KEY_RIGHT,
        NH_WSI_KEY_DOWN, 
        NH_WSI_KEY_PRIOR,
        NH_WSI_KEY_PAGE_UP,
        NH_WSI_KEY_NEXT,
        NH_WSI_KEY_PAGE_DOWN,
        NH_WSI_KEY_END,
        NH_WSI_KEY_BEGIN,
        // Misc functions
        NH_WSI_KEY_SELECT,
        NH_WSI_KEY_PRINT,
        NH_WSI_KEY_EXECUTE,
        NH_WSI_KEY_INSERT,
        NH_WSI_KEY_UNDO,
        NH_WSI_KEY_REDO,
        NH_WSI_KEY_MENU,
        NH_WSI_KEY_FIND,
        NH_WSI_KEY_CANCEL,
        NH_WSI_KEY_HELP,
        NH_WSI_KEY_BREAK,
        NH_WSI_KEY_MODE_SWITCH,
        NH_WSI_KEY_SCRIPT_SWITCH,
        NH_WSI_KEY_NUM_LOCK,
        // Keypad functions
        NH_WSI_KEY_KP_SPACE,
        NH_WSI_KEY_KP_TAB,
        NH_WSI_KEY_KP_ENTER,
        NH_WSI_KEY_KP_F1,
        NH_WSI_KEY_KP_F2,
        NH_WSI_KEY_KP_F3,
        NH_WSI_KEY_KP_F4,
        NH_WSI_KEY_KP_HOME,
        NH_WSI_KEY_KP_LEFT,
        NH_WSI_KEY_KP_UP,
        NH_WSI_KEY_KP_RIGHT,
        NH_WSI_KEY_KP_DOWN,
        NH_WSI_KEY_KP_PRIOR,
        NH_WSI_KEY_KP_PAGE_UP,
        NH_WSI_KEY_KP_NEXT,
        NH_WSI_KEY_KP_PAGE_DOWN,
        NH_WSI_KEY_KP_END,
        NH_WSI_KEY_KP_BEGIN,
        NH_WSI_KEY_KP_INSERT,
        NH_WSI_KEY_KP_DELETE,
        NH_WSI_KEY_KP_EQUAL,
        NH_WSI_KEY_KP_MULTIPLY,
        NH_WSI_KEY_KP_ADD,
        NH_WSI_KEY_KP_SEPARATOR,
        NH_WSI_KEY_KP_SUBTRACT,
        NH_WSI_KEY_KP_DECIMAL,
        NH_WSI_KEY_KP_DIVIDE,
        NH_WSI_KEY_KP_0,
        NH_WSI_KEY_KP_1,
        NH_WSI_KEY_KP_2,
        NH_WSI_KEY_KP_3,
        NH_WSI_KEY_KP_4,
        NH_WSI_KEY_KP_5,
        NH_WSI_KEY_KP_6,
        NH_WSI_KEY_KP_7,
        NH_WSI_KEY_KP_8,
        NH_WSI_KEY_KP_9,
        NH_WSI_KEY_F1,
        NH_WSI_KEY_F2,
        NH_WSI_KEY_F3,
        NH_WSI_KEY_F4,
        NH_WSI_KEY_F5,
        NH_WSI_KEY_F6,
        NH_WSI_KEY_F7,
        NH_WSI_KEY_F8,
        NH_WSI_KEY_F9,
        NH_WSI_KEY_F10,
        NH_WSI_KEY_F11,
        NH_WSI_KEY_F12,
        NH_WSI_KEY_F13,
        NH_WSI_KEY_F14,
        NH_WSI_KEY_F15,
        NH_WSI_KEY_F16,
        NH_WSI_KEY_F17,
        NH_WSI_KEY_F18,
        NH_WSI_KEY_F19,
        NH_WSI_KEY_F20,
        NH_WSI_KEY_F21,
        NH_WSI_KEY_F22,
        NH_WSI_KEY_F23,
        NH_WSI_KEY_F24,
        NH_WSI_KEY_F25,
        NH_WSI_KEY_F26,
        NH_WSI_KEY_F27,
        NH_WSI_KEY_F28,
        NH_WSI_KEY_F29,
        NH_WSI_KEY_F30,
        NH_WSI_KEY_F31,
        NH_WSI_KEY_F32,
        NH_WSI_KEY_F33,
        NH_WSI_KEY_F34,
        NH_WSI_KEY_F35,
    } NH_WSI_KEY_E;

    typedef enum NH_WSI_MOUSE_E {
        NH_WSI_MOUSE_UNDEFINED,
        NH_WSI_MOUSE_LEFT,
        NH_WSI_MOUSE_RIGHT,
        NH_WSI_MOUSE_MIDDLE,
        NH_WSI_MOUSE_SCROLL,
        NH_WSI_MOUSE_MOVE,
    } NH_WSI_MOUSE_E;

    typedef enum NH_WSI_CURSOR_E {
        NH_WSI_CURSOR_ARROW,
        NH_WSI_CURSOR_ARROW_DOWN,
        NH_WSI_CURSOR_ARROW_UP,
        NH_WSI_CURSOR_BOAT,
        NH_WSI_CURSOR_BOGOSITY,
        NH_WSI_CURSOR_BOTTOM_LEFT_CORNER,
        NH_WSI_CURSOR_BOTTOM_RIGHT_CORNER,
        NH_WSI_CURSOR_BOTTOM_SIDE,
        NH_WSI_CURSOR_BOTTOM_TEE,
        NH_WSI_CURSOR_BOX_SPIRAL,
        NH_WSI_CURSOR_CENTER_PTR,
        NH_WSI_CURSOR_CIRCLE,
        NH_WSI_CURSOR_CLOCK,
        NH_WSI_CURSOR_COFFEE_MUG,
        NH_WSI_CURSOR_CROSS,
        NH_WSI_CURSOR_CROSS_REVERSE,
        NH_WSI_CURSOR_CROSSHAIR,
        NH_WSI_CURSOR_DIAMOND_CROSS,
        NH_WSI_CURSOR_DOT,
        NH_WSI_CURSOR_DOTBOX,
        NH_WSI_CURSOR_DOUBLE_ARROW,
        NH_WSI_CURSOR_DRAFT_LARGE,
        NH_WSI_CURSOR_DRAFT_SMALL,
        NH_WSI_CURSOR_DRAPED_BOX,
        NH_WSI_CURSOR_EXCHANGE,
        NH_WSI_CURSOR_FLEUR,
        NH_WSI_CURSOR_GOBBLER,
        NH_WSI_CURSOR_GUMBY,
        NH_WSI_CURSOR_HAND1,
        NH_WSI_CURSOR_HAND2,
        NH_WSI_CURSOR_HEART,
        NH_WSI_CURSOR_ICON,
        NH_WSI_CURSOR_IRON_CROSS,
        NH_WSI_CURSOR_LEFT_PTR,
        NH_WSI_CURSOR_LEFT_SIDE,
        NH_WSI_CURSOR_LEFT_TEE,
        NH_WSI_CURSOR_LEFT_BUTTON,
        NH_WSI_CURSOR_LL_ANGLE,
        NH_WSI_CURSOR_LR_ANGLE,
        NH_WSI_CURSOR_MAN,
        NH_WSI_CURSOR_MIDDLEBUTTON,
        NH_WSI_CURSOR_MOUSE,
        NH_WSI_CURSOR_PENCIL,
        NH_WSI_CURSOR_PIRATE,
        NH_WSI_CURSOR_PLUS,
        NH_WSI_CURSOR_QUESTION_ARROW,
        NH_WSI_CURSOR_RIGHT_PTR,
        NH_WSI_CURSOR_RIGHT_SIDE,
        NH_WSI_CURSOR_RIGHT_TEE,
        NH_WSI_CURSOR_RIGHT_BUTTON,
        NH_WSI_CURSOR_RTL_LOGO,
        NH_WSI_CURSOR_SAILBOAT,
        NH_WSI_CURSOR_SB_DOWN_ARROW,
        NH_WSI_CURSOR_SB_H_DOUBLE_ARROW,
        NH_WSI_CURSOR_SB_LEFT_ARROW,
        NH_WSI_CURSOR_SB_RIGHT_ARROW,
        NH_WSI_CURSOR_SB_UP_ARROW,
        NH_WSI_CURSOR_SB_V_DOUBLE_ARROW,
        NH_WSI_CURSOR_SHUTTLE,
        NH_WSI_CURSOR_SIZING,
        NH_WSI_CURSOR_SPIDER,
        NH_WSI_CURSOR_SPRAYCAN,
        NH_WSI_CURSOR_STAR,
        NH_WSI_CURSOR_TARGET,
        NH_WSI_CURSOR_TCROSS,
        NH_WSI_CURSOR_TOP_LEFT_ARROW,
        NH_WSI_CURSOR_TOP_LEFT_CORNER,
        NH_WSI_CURSOR_TOP_RIGHT_CORNER,
        NH_WSI_CURSOR_TOP_SIDE,
        NH_WSI_CURSOR_TOP_TEE,
        NH_WSI_CURSOR_TREK,
        NH_WSI_CURSOR_UL_ANGLE,
        NH_WSI_CURSOR_UMBRELLA,
        NH_WSI_CURSOR_UR_ANGLE,
        NH_WSI_CURSOR_WATCH,
        NH_WSI_CURSOR_XTERM,
    } NH_WSI_CURSOR_E;

    typedef enum NH_WSI_MOUSE_FLAG {
        NH_WSI_MOUSE_FLAG_UNDEFINED = 1 << 0,
        NH_WSI_MOUSE_FLAG_LEFT      = 1 << 1,
        NH_WSI_MOUSE_FLAG_RIGHT     = 1 << 2,
        NH_WSI_MOUSE_FLAG_MIDDLE    = 1 << 3,
        NH_WSI_MOUSE_FLAG_SCROLL    = 1 << 4,
        NH_WSI_MOUSE_FLAG_MOVE      = 1 << 5,
    } NH_WSI_MOUSE_FLAG;

    typedef enum NH_WSI_MODIFIER_FLAG {
        NH_WSI_MODIFIER_SHIFT   = 1 << 0, 
        NH_WSI_MODIFIER_LOCK    = 1 << 1,
        NH_WSI_MODIFIER_CONTROL = 1 << 2,
        NH_WSI_MODIFIER_MOD1    = 1 << 3,
        NH_WSI_MODIFIER_MOD2    = 1 << 4,
        NH_WSI_MODIFIER_MOD3    = 1 << 5,
        NH_WSI_MODIFIER_MOD4    = 1 << 6,
        NH_WSI_MODIFIER_MOD5    = 1 << 7,
    } NH_WSI_MODIFIER_FLAG;

// STRUCTS =========================================================================================

    typedef struct nh_wsi_WindowEvent {
        NH_WSI_WINDOW_E type;
        nh_PixelPosition Position;
        nh_PixelSize Size;
    } nh_wsi_WindowEvent;

    typedef struct nh_wsi_MouseEvent {
        NH_WSI_MOUSE_E type;
        NH_WSI_TRIGGER_E trigger;
        nh_PixelPosition Position;
    } nh_wsi_MouseEvent;

    typedef struct nh_wsi_KeyboardEvent {
        NH_WSI_KEY_E special;
        NH_ENCODING_UTF32 codepoint;
        NH_WSI_TRIGGER_E trigger;
        NH_WSI_MODIFIER_FLAG state;
    } nh_wsi_KeyboardEvent;

    /**
     * Wrapper type for possible WSI events.
     */
    typedef struct nh_wsi_Event {
        NH_WSI_EVENT_E type;           /**<Indicates the type of event.*/
        nh_wsi_WindowEvent Window;     /**<Holds event data in case of window event.*/
        nh_wsi_MouseEvent Mouse;       /**<Holds event data in case of mouse event.*/
        nh_wsi_KeyboardEvent Keyboard; /**<Holds event data in case of keyboard event.*/
    } nh_wsi_Event;

// TYPEDEFS ========================================================================================

    typedef struct nh_gfx_SurfaceRequirements nh_gfx_SurfaceRequirements;
    typedef struct nh_wsi_Window nh_wsi_Window;

    typedef void (*nh_wsi_callback_f)(
        nh_wsi_Window *Window_p, nh_wsi_Event Event
    );

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_wsi_Window *nh_api_createWindow(
        NH_BYTE *namespace_p, nh_gfx_SurfaceRequirements *Requirements_p
    );

    /**
     * Todo.
     */
    NH_WSI_RESULT_E nh_api_setWindowEventListener(
        nh_wsi_Window *Window_p, nh_wsi_callback_f callback_f
    );

/** @} */

#endif // NH_WSI_H
