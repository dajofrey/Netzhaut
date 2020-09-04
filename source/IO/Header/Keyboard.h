#ifndef NH_IO_KEYBOARD_H
#define NH_IO_KEYBOARD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Core/Header/RingIterator.h"
#include "../../Core/Header/System.h"

#include <stdbool.h>

typedef struct Nh_Tab Nh_Tab;
typedef struct NH_IO_Input NH_IO_Input;

#endif

/** @addtogroup IOEnums Enums
 *  \ingroup IO
 *  @{
 */

    typedef enum NH_IO_KEY {
        NH_IO_KEY_UNDEFINED   ,
        
        // Modifiers
        NH_IO_KEY_SHIFT_L     , 
        NH_IO_KEY_SHIFT_R     ,
        NH_IO_KEY_CONTROL_L   ,
        NH_IO_KEY_CONTROL_R   ,
        NH_IO_KEY_CAPS_LOCK   ,
        NH_IO_KEY_SHIFT_LOCK  , 
        NH_IO_KEY_META_L      , 
        NH_IO_KEY_META_R      , 
        NH_IO_KEY_ALT_L       , 
        NH_IO_KEY_ALT_R       , 
        NH_IO_KEY_SUPER_L     , 
        NH_IO_KEY_SUPER_R     , 
        NH_IO_KEY_HYPER_L     , 
        NH_IO_KEY_HYPER_R     , 
    
        // Latin 1 (ISO/IEC 8859-1 = Unicode U+0020..U+00FF)
        NH_IO_KEY_SPACE       , 
        NH_IO_KEY_EXCLAM      , 
        NH_IO_KEY_QUOTEDBL    , 
        NH_IO_KEY_NUMBERSIGN  , 
        NH_IO_KEY_DOLLAR      , 
        NH_IO_KEY_PERCENT     , 
        NH_IO_KEY_AMPERSAND   , 
        NH_IO_KEY_APOSTROPHE  , 
        NH_IO_KEY_PARENLEFT   , 
        NH_IO_KEY_PARENRIGHT  , 
        NH_IO_KEY_ASTERISK    , 
        NH_IO_KEY_PLUS        , 
        NH_IO_KEY_COMMA       , 
        NH_IO_KEY_MINUS       , 
        NH_IO_KEY_PERIOD      , 
        NH_IO_KEY_SLASH       , 
        NH_IO_KEY_0           , 
        NH_IO_KEY_1           , 
        NH_IO_KEY_2           , 
        NH_IO_KEY_3           , 
        NH_IO_KEY_4           , 
        NH_IO_KEY_5           , 
        NH_IO_KEY_6           , 
        NH_IO_KEY_7           , 
        NH_IO_KEY_8           , 
        NH_IO_KEY_9           , 
        NH_IO_KEY_COLON       , 
        NH_IO_KEY_SEMICOLON   , 
        NH_IO_KEY_LESS        , 
        NH_IO_KEY_EQUAL       , 
        NH_IO_KEY_GREATER     , 
        NH_IO_KEY_QUESTION    , 
        NH_IO_KEY_AT          , 
        NH_IO_KEY_CAPITAL_A   ,
        NH_IO_KEY_CAPITAL_B   ,
        NH_IO_KEY_CAPITAL_C   ,
        NH_IO_KEY_CAPITAL_D   ,
        NH_IO_KEY_CAPITAL_E   ,
        NH_IO_KEY_CAPITAL_F   ,
        NH_IO_KEY_CAPITAL_G   ,
        NH_IO_KEY_CAPITAL_H   ,
        NH_IO_KEY_CAPITAL_I   ,
        NH_IO_KEY_CAPITAL_J   ,
        NH_IO_KEY_CAPITAL_K   ,
        NH_IO_KEY_CAPITAL_L   ,
        NH_IO_KEY_CAPITAL_M   ,
        NH_IO_KEY_CAPITAL_N   ,
        NH_IO_KEY_CAPITAL_O   ,
        NH_IO_KEY_CAPITAL_P   ,
        NH_IO_KEY_CAPITAL_Q   ,
        NH_IO_KEY_CAPITAL_R   ,
        NH_IO_KEY_CAPITAL_S   ,
        NH_IO_KEY_CAPITAL_T   ,
        NH_IO_KEY_CAPITAL_U   ,
        NH_IO_KEY_CAPITAL_V   ,
        NH_IO_KEY_CAPITAL_W   ,
        NH_IO_KEY_CAPITAL_X   ,
        NH_IO_KEY_CAPITAL_Y   ,
        NH_IO_KEY_CAPITAL_Z   ,
        NH_IO_KEY_BRACKETLEFT , 
        NH_IO_KEY_BACKSLASH   , 
        NH_IO_KEY_BRACKETRIGHT, 
        NH_IO_KEY_ASCIICIRCUM , 
        NH_IO_KEY_UNDERSCORE  , 
        NH_IO_KEY_GRAVE       , 
        NH_IO_KEY_SMALL_A     , 
        NH_IO_KEY_SMALL_B     , 
        NH_IO_KEY_SMALL_C     , 
        NH_IO_KEY_SMALL_D     , 
        NH_IO_KEY_SMALL_E     , 
        NH_IO_KEY_SMALL_F     , 
        NH_IO_KEY_SMALL_G     , 
        NH_IO_KEY_SMALL_H     , 
        NH_IO_KEY_SMALL_I     , 
        NH_IO_KEY_SMALL_J     , 
        NH_IO_KEY_SMALL_K     , 
        NH_IO_KEY_SMALL_L     , 
        NH_IO_KEY_SMALL_M     , 
        NH_IO_KEY_SMALL_N     , 
        NH_IO_KEY_SMALL_O     , 
        NH_IO_KEY_SMALL_P     , 
        NH_IO_KEY_SMALL_Q     , 
        NH_IO_KEY_SMALL_R     , 
        NH_IO_KEY_SMALL_S     , 
        NH_IO_KEY_SMALL_T     , 
        NH_IO_KEY_SMALL_U     , 
        NH_IO_KEY_SMALL_V     , 
        NH_IO_KEY_SMALL_W     , 
        NH_IO_KEY_SMALL_X     , 
        NH_IO_KEY_SMALL_Y     , 
        NH_IO_KEY_SMALL_Z     , 
        NH_IO_KEY_BRACELEFT   , 
        NH_IO_KEY_BAR         , 
        NH_IO_KEY_BRACERIGHT  , 
        NH_IO_KEY_ASCIITILDE  ,
    
        NH_IO_KEY_COUNT       , 
    } NH_IO_KEY;

/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */  

    typedef struct NH_IO_KeyboardEvent {
        NH_IO_KEY key;    
        int trigger; 
    } NH_IO_KeyboardEvent;            

    typedef struct NH_IO_Keyboard {
        Nh_RingIterator Events;
        bool state_p[NH_IO_KEY_COUNT];              
    } NH_IO_Keyboard;

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_RESULT Nh_IO_initKeyboard(
        NH_IO_Keyboard *Keyboard_p, int bufferSize
    ); 

    void Nh_IO_handleKeyboard(
        Nh_Tab *Tab_p
    );
    
    NH_RESULT Nh_IO_stringifyKeyboard(
        bool *press_p, char *string_p
    );

/** @} */

#endif 
