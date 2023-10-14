#ifndef NH_TERMINAL_GRID_H
#define NH_TERMINAL_GRID_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../../nhcore/Util/Time.h"
#include "../../nhgfx/Fonts/Text.h"

#endif

/** @addtogroup lib_nhterminal_structs
 *  @{
 */

    typedef struct nh_terminal_Box {
        nh_PixelPosition UpperLeft;
        nh_PixelPosition LowerRight;
        float innerVertices_p[18];
        float outerVertices_p[18];
    } nh_terminal_Box;

    typedef struct nh_terminal_TileUpdate {
        nh_tty_Glyph Glyph;
        int row;
        int col;
        NH_BOOL cursor;
    } nh_terminal_TileUpdate;

    typedef struct nh_terminal_TileForeground {
        float vertices_p[24];
    } nh_terminal_TileForeground;

    typedef struct nh_terminal_TileBackground {
        float vertices_p[12];
    } nh_terminal_TileBackground;

    typedef struct nh_terminal_Tile {
        nh_terminal_TileForeground Foreground;
        nh_terminal_TileBackground Background;
        nh_tty_Glyph Glyph;
        NH_BOOL dirty;
    } nh_terminal_Tile;

    typedef struct nh_terminal_Grid {
        nh_List Rows;
        nh_Array Boxes;
        nh_PixelSize TileSize;
        nh_PixelSize Size;
        int cols;
        int rows;
        nh_terminal_Tile *Cursor_p;
        nh_terminal_TileUpdate **Updates_pp;
        NH_BOOL **updates_pp;
    } nh_terminal_Grid;

/** @} */

/** @addtogroup lib_nhterminal_functions
 *  @{
 */

    nh_terminal_Tile *nh_terminal_getTile(
        nh_terminal_Grid *Grid_p, int row, int col
    );

    NH_TERMINAL_RESULT nh_terminal_initGrid(
        nh_terminal_Grid *Grid_p
    );

    NH_TERMINAL_RESULT nh_terminal_freeGrid(
        nh_terminal_Grid *Grid_p
    );

    NH_TERMINAL_RESULT nh_terminal_updateGrid(
        nh_terminal_Grid *Grid_p, void *state_p, nh_gfx_Text *Text_p
    );

    NH_BOOL nh_terminal_compareBackgroundAttributes(
        nh_tty_Glyph *Glyph1_p, nh_tty_Glyph *Glyph2_p
    );
    
    NH_BOOL nh_terminal_compareForegroundAttributes(
        nh_tty_Glyph *Glyph1_p, nh_tty_Glyph *Glyph2_p
    );

    NH_TERMINAL_RESULT nh_terminal_updateTile(
        nh_terminal_Grid *Grid_p, void *state_p, nh_terminal_TileUpdate *Update_p, NH_BOOL *update_p
    );

    NH_TERMINAL_RESULT nh_terminal_updateBoxes(
        nh_terminal_Grid *Grid_p, void *state_p, nh_Array *Boxes_p
    );

/** @} */

#endif 
