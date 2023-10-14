#ifndef NH_TTY_VIEW_H
#define NH_TTY_VIEW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#ifdef __unix__
    #include <termios.h>
#endif

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_Forward {
        nh_RingBuffer Tiles;
        nh_RingBuffer Boxes;
        nh_RingBuffer Events;
    } nh_tty_Forward;

    typedef struct nh_tty_View {
        nh_tty_Forward Forward;
        NH_BOOL standardIO;
        int cols, rows;
        int previousCols, previousRows;
        nh_PixelSize Size;
        nh_PixelSize PreviousSize;
        nh_tty_Row Row;
        nh_tty_Row *Grid1_p; /**<Grid for program drawing.*/
        nh_tty_Row *Grid2_p; /**<Grid for context-menu drawing.*/
        void *p;
#ifdef __unix__
        struct termios Termios;
#endif
    } nh_tty_View;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    NH_TTY_RESULT nh_tty_getViewSize(
        nh_tty_View *View_p
    ); 

    NH_TTY_RESULT nh_tty_translateMousePosition(
        nh_tty_View *View_p, nh_wsi_MouseEvent Mouse, int *col_p, int *row_p
    );

    NH_TTY_RESULT nh_tty_updateView(
        nh_tty_View *View_p, NH_BOOL *updated_p
    );

    nh_tty_View *nh_tty_createView(
        nh_tty_TTY *TTY_p, void *p, NH_BOOL standardIO
    );
    
    NH_TTY_RESULT nh_tty_destroyView(
        nh_tty_TTY *TTY_p, nh_tty_View *View_p
    );

// FORWARD FUNCTIONS
// These functions forward specific TTY internal data to the view for rendering etc..

    NH_TTY_RESULT nh_tty_forwardCursor(
        nh_tty_View *View_p, int x, int y
    );

    NH_TTY_RESULT nh_tty_forwardGrid1(
        nh_tty_View *View_p
    );

    NH_TTY_RESULT nh_tty_forwardGrid2(
        nh_tty_View *View_p
    );

    NH_TTY_RESULT nh_tty_forwardEvent(
        nh_tty_View *View_p, nh_wsi_Event Event
    );

/** @} */

#endif 
