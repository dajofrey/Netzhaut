#ifndef NH_TTY_DRAW_H
#define NH_TTY_DRAW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "TTY.h"
#include "View.h"

#endif

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    NH_TTY_RESULT nh_tty_refreshCursor(
        nh_tty_TTY *TTY_p
    );

    NH_TTY_RESULT nh_tty_refreshGrid1(
        nh_tty_TTY *TTY_p
    ); 

    NH_TTY_RESULT nh_tty_refreshGrid1Row(
        nh_List *Tiles_p, nh_tty_View *View_p, int row, NH_BOOL sidebar
    );

    NH_TTY_RESULT nh_tty_getCursorPosition(
        nh_tty_Tile *MacroTile_p, nh_tty_Tile *MicroTile_p, NH_BOOL standardIO, int *x_p, int *y_p
    );
    
/** @} */

#endif 
