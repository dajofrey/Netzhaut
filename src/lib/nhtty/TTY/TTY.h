#ifndef NH_TTY_TTY_H
#define NH_TTY_TTY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Macro.h"
#include "../Common/Includes.h"
#include "../Common/Config.h"

#define CTRL_KEY(k) ((k) & 0x1f)

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_Clipboard {
        nh_Array Lines;
    } nh_tty_Clipboard;

    typedef struct nh_tty_Borders {
        NH_BOOL on;
    } nh_tty_Borders;

    typedef struct nh_tty_Topbars {
        NH_BOOL on;
    } nh_tty_TopBars;

    typedef struct nh_tty_Preview {
        NH_BOOL blink;
        double blinkFrequency;
        nh_SystemTime LastBlink;
    } nh_tty_Preview;

    typedef struct nh_tty_TTY {
        nh_tty_Clipboard Clipboard;
        nh_tty_Borders Borders;
        nh_tty_TopBars TopBars;
        nh_tty_Preview Preview;
        nh_tty_MacroWindow *Window_p;
        nh_tty_Tile *InsertTile_p;
        nh_List Views;
        nh_List Prototypes;
        nh_List Windows;
        NH_BOOL alt;
        NH_BOOL ctrl;
        NH_BOOL close;
        nh_wsi_KeyboardEvent LastEvent;
        NH_BOOL hasFocus;
        NH_BYTE namespace_p[255];
        nh_RingBuffer Events;
    } nh_tty_TTY;

/** @} */

/** @addtogroup lib_nhtty_typedefs
 *  @{
 */

    typedef nh_tty_TTY *(*nh_tty_openTTY_f)(
       NH_BYTE *config_p
    ); 

    typedef NH_TTY_RESULT (*nh_tty_closeTTY_f)(
        nh_tty_TTY *TTY_p
    );

    typedef NH_TTY_RESULT (*nh_tty_cmd_addProgram_f)(
        nh_tty_TTY *TTY_p, NH_TTY_PROGRAM_E type, NH_BOOL once
    );
    
    typedef NH_TTY_RESULT (*nh_tty_cmd_claimStandardIO_f)(
        nh_tty_TTY *TTY_p
    );
    
    typedef NH_TTY_RESULT (*nh_tty_cmd_unclaimStandardIO_f)(
        nh_tty_TTY *TTY_p
    );

    typedef NH_TTY_RESULT (*nh_tty_cmd_sendEvent_f)(
        nh_tty_TTY *TTY_p, nh_wsi_Event Event
    );

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    NH_TTY_RESULT nh_tty_resetClipboard(
    );

    nh_encoding_UTF32String *nh_tty_newClipboardLine(
    );

    nh_tty_Clipboard *nh_tty_getClipboard(
    );

/** @} */

#endif
