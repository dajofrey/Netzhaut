#ifndef NH_TTY_TOPBAR_H
#define NH_TTY_TOPBAR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Program.h"
#include "TopBarMessage.h"

#include "../Common/Includes.h"

typedef struct nh_tty_MicroWindow nh_tty_MicroWindow;

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_TopBarSuggestions {
        nh_SystemTime Time;
	nh_tty_Program *ProgramSuggestion_p;
	NH_BYTE *CommandSuggestion_p;
	double interval;
	NH_BOOL update;
	int commandIndex;
	int programIndex;
    } nh_tty_TopBarSuggestions;

    typedef struct nh_tty_TopBar {
	nh_tty_TopBarSuggestions Suggestions;
        nh_tty_TopBarMessage Message;
        int state;
        int cursorX;
        NH_BOOL hasFocus;
	NH_BOOL refresh;
        nh_encoding_UTF32String Command;
        nh_Array History;
    } nh_tty_TopBar;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_TopBar nh_tty_initTopBar(
    );

    NH_TTY_RESULT nh_tty_freeTopBar(
        nh_tty_TopBar *TopBar_p
    );

    void nh_tty_toggleTopBar(
        nh_tty_TopBar *TopBar_p
    );

    NH_TTY_RESULT nh_tty_getTopBarCursor(
        nh_tty_TopBar *TopBar_p, int *x_p, int *y_p, NH_BOOL topTile
    );

    NH_TTY_RESULT nh_tty_handleTopBarInput(
        nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Canvas_p, nh_wsi_KeyboardEvent Event 
    );

    NH_TTY_RESULT nh_tty_drawTopBarRow(
        nh_tty_TopBar *TopBar_p, nh_tty_MicroWindow *Canvas_p, nh_tty_Glyph *Glyphs_p, int cols, int row,
        NH_BOOL standardIO
    );

    NH_TTY_RESULT nh_tty_updateTopBar(
        nh_tty_TopBar *TopBar_p
    );

/** @} */

#endif 
