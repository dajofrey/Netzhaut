#ifndef NH_TERMINAL_TERMINAL_VERTICES_H
#define NH_TERMINAL_TERMINAL_VERTICES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Grid.h"
#include "Graphics.h"

#endif

/** @addtogroup lib_nhterminal_functions
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_getBackgroundVertices(
        nh_terminal_GraphicsState *State_p, nh_terminal_Grid *Grid_p, nh_tty_Glyph *Glyph_p, int col, 
        int row, float vertices_p[12]
    );

    NH_TERMINAL_RESULT nh_terminal_getBoxVertices(
        nh_terminal_GraphicsState *State_p, nh_terminal_Grid *Grid_p, nh_terminal_Box *Box_p, NH_BOOL inner
    );

    NH_TERMINAL_RESULT nh_terminal_getOutlineVertices(
        nh_terminal_GraphicsState *State_p, nh_terminal_Grid *Grid_p, nh_terminal_Box *Box_p, NH_BOOL inner
    );

    NH_TERMINAL_RESULT nh_terminal_getForegroundVertices(
        nh_terminal_GraphicsState *State_p, nh_terminal_Grid *Grid_p, nh_tty_Glyph *Glyph_p, int col, 
        int row, float *vertices_p
    );

/** @} */

#endif // NH_TERMINAL_TERMINAL_VERTICES_H 
