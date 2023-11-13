// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Grid.h"
#include "Graphics.h"
#include "Vertices.h"

#include "../Vulkan/Render.h"
#include "../Common/Macros.h"
#include "../Common/Config.h"

#include "../../nhgfx/Base/Viewport.h"

#include "../../nhcore/System/Thread.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Process.h"
#include "../../nhcore/System/Logger.h"

#include "../../nhencoding/Encodings/UTF8.h"
#include "../../nhencoding/Encodings/UTF32.h"

#include <string.h>
#include <stdio.h>

// INIT ============================================================================================

NH_TERMINAL_RESULT nh_terminal_initGrid(
    nh_terminal_Grid *Grid_p)
{
NH_TERMINAL_BEGIN()

    memset(Grid_p, 0, sizeof(nh_terminal_Grid));

    Grid_p->Rows = nh_core_initList(128);
    Grid_p->Boxes = nh_core_initArray(sizeof(nh_terminal_Box), 16);

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_freeGrid(
    nh_terminal_Grid *Grid_p)
{
NH_TERMINAL_BEGIN()

    for (int row = 0; row < Grid_p->Rows.size; ++row) {
        nh_List *Cols_p = Grid_p->Rows.pp[row];
        for (int col = 0; col < Cols_p->size; ++col) {
            nh_core_free(Cols_p->pp[col]);
        }
        nh_core_freeList(Cols_p, NH_FALSE);
    }

    for (int row = 0; row < Grid_p->rows; ++row) {
        nh_core_free(Grid_p->Updates_pp[row]);
        nh_core_free(Grid_p->updates_pp[row]);
    }
    nh_core_free(Grid_p->Updates_pp);
    nh_core_free(Grid_p->updates_pp);

    nh_core_freeList(&Grid_p->Rows, NH_TRUE);
    nh_core_freeArray(&Grid_p->Boxes);

    nh_terminal_initGrid(Grid_p);

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_initTile(
    nh_terminal_Grid *Grid_p, nh_terminal_Tile *Tile_p, int row, int col)
{
NH_TERMINAL_BEGIN()

    memset(Tile_p, 0, sizeof(nh_terminal_Tile));

    memset(Tile_p->Foreground.vertices_p, 0, sizeof(float) * 20);
    memset(Tile_p->Background.vertices_p, 0, sizeof(float) * 12);

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

// UPDATE ==========================================================================================

static void nh_terminal_getTileRowAndColumn(
    nh_terminal_Grid *Grid_p, nh_terminal_Tile *Tile_p, int *row_p, int *col_p)
{
NH_TERMINAL_BEGIN()

    NH_BOOL done = NH_FALSE;

    for (int row = 0; !done && row < Grid_p->Rows.size; ++row) {
        nh_List *Row_p = Grid_p->Rows.pp[row];
        for (int col = 0; !done && col < Row_p->size; ++col) {
            if (Row_p->pp[col] == Tile_p) {
                *row_p = row;
                *col_p = col;
                done = NH_TRUE;
            }
        }
    }

NH_TERMINAL_SILENT_END()
}

nh_terminal_Tile *nh_terminal_getTile(
    nh_terminal_Grid *Grid_p, int row, int col)
{
NH_TERMINAL_BEGIN()

    if (Grid_p->cols <= col || Grid_p->rows <= row) {
        // If for some reason a tile is requested that is out of bounds, there is something wrong.
        NH_TERMINAL_END(NULL)
    }

    while (Grid_p->Rows.size <= row) {
        nh_core_appendToList(&Grid_p->Rows, NULL);
    }

    if (!Grid_p->Rows.pp[row]) {
        Grid_p->Rows.pp[row] = nh_core_allocate(sizeof(nh_List));
        NH_TERMINAL_CHECK_MEM_2(NULL, Grid_p->Rows.pp[row])
        *((nh_List*)Grid_p->Rows.pp[row]) = nh_core_initList(128);
    }

    nh_List *Cols_p = Grid_p->Rows.pp[row];

     while (Cols_p->size <= col) {
        nh_core_appendToList(Cols_p, NULL);
    }

    if (!Cols_p->pp[col]) {
        Cols_p->pp[col] = nh_core_allocate(sizeof(nh_terminal_Tile));
        NH_TERMINAL_CHECK_MEM_2(NULL, Cols_p->pp[col])
        NH_TERMINAL_CHECK_2(NULL, nh_terminal_initTile(Grid_p, Cols_p->pp[col], row, col))
    }

NH_TERMINAL_END(Cols_p->pp[col])
}

NH_BOOL nh_terminal_compareBackgroundAttributes(
    nh_tty_Glyph *Glyph1_p, nh_tty_Glyph *Glyph2_p)
{
NH_TERMINAL_BEGIN()

    // Compare attributes.
    if ((Glyph1_p->mark & NH_TTY_MARK_ACCENT) != (Glyph2_p->mark & NH_TTY_MARK_ACCENT)) {
        NH_TERMINAL_END(NH_TRUE)
    }

    if (Glyph1_p->Attributes.blink != Glyph2_p->Attributes.blink
    ||  Glyph1_p->Attributes.reverse != Glyph2_p->Attributes.reverse) {
        NH_TERMINAL_END(NH_TRUE)
    }

    // Compare Color.
    if (Glyph1_p->Background.custom != Glyph2_p->Background.custom
    ||  Glyph1_p->Background.Color.r != Glyph2_p->Background.Color.r
    ||  Glyph1_p->Background.Color.g != Glyph2_p->Background.Color.g
    ||  Glyph1_p->Background.Color.b != Glyph2_p->Background.Color.b
    ||  Glyph1_p->Background.Color.a != Glyph2_p->Background.Color.a) {
        NH_TERMINAL_END(NH_TRUE)
    }
    if (Glyph1_p->Foreground.custom != Glyph2_p->Foreground.custom
    ||  Glyph1_p->Foreground.Color.r != Glyph2_p->Foreground.Color.r
    ||  Glyph1_p->Foreground.Color.g != Glyph2_p->Foreground.Color.g
    ||  Glyph1_p->Foreground.Color.b != Glyph2_p->Foreground.Color.b
    ||  Glyph1_p->Foreground.Color.a != Glyph2_p->Foreground.Color.a) {
        NH_TERMINAL_END(NH_TRUE)
    }

NH_TERMINAL_END(NH_FALSE)
}

NH_BOOL nh_terminal_compareForegroundAttributes(
    nh_tty_Glyph *Glyph1_p, nh_tty_Glyph *Glyph2_p)
{
NH_TERMINAL_BEGIN()

    if ((Glyph1_p->mark & NH_TTY_MARK_ACCENT) != (Glyph2_p->mark & NH_TTY_MARK_ACCENT)) {
        NH_TERMINAL_END(NH_TRUE)
    }

    // Compare attributes.
    if (Glyph1_p->Attributes.bold != Glyph2_p->Attributes.bold
    ||  Glyph1_p->Attributes.faint != Glyph2_p->Attributes.faint
    ||  Glyph1_p->Attributes.italic != Glyph2_p->Attributes.italic
    ||  Glyph1_p->Attributes.underline != Glyph2_p->Attributes.underline
    ||  Glyph1_p->Attributes.blink != Glyph2_p->Attributes.blink
    ||  Glyph1_p->Attributes.reverse != Glyph2_p->Attributes.reverse
    ||  Glyph1_p->Attributes.invisible != Glyph2_p->Attributes.invisible
    ||  Glyph1_p->Attributes.struck != Glyph2_p->Attributes.struck
    ||  Glyph1_p->Attributes.wrap != Glyph2_p->Attributes.wrap
    ||  Glyph1_p->Attributes.wide != Glyph2_p->Attributes.wide) {
        NH_TERMINAL_END(NH_TRUE)
    }

    // Compare background.
    if (Glyph1_p->Foreground.custom != Glyph2_p->Foreground.custom
    ||  Glyph1_p->Foreground.Color.r != Glyph2_p->Foreground.Color.r
    ||  Glyph1_p->Foreground.Color.g != Glyph2_p->Foreground.Color.g
    ||  Glyph1_p->Foreground.Color.b != Glyph2_p->Foreground.Color.b
    ||  Glyph1_p->Foreground.Color.a != Glyph2_p->Foreground.Color.a) {
        NH_TERMINAL_END(NH_TRUE)
    }

NH_TERMINAL_END(NH_FALSE)
}

static NH_TERMINAL_RESULT nh_terminal_updateCursorTile(
    nh_terminal_Grid *Grid_p, nh_terminal_GraphicsState *State_p, nh_terminal_TileUpdate *Update_p)
{
NH_TERMINAL_BEGIN()

    // Check if inside draw area, if not don't show cursor.
    if (Update_p->row >= Grid_p->rows || Update_p->col >= Grid_p->cols || Update_p->row < 0 || Update_p->col < 0) {
        if (Grid_p->Cursor_p) {
            Grid_p->Cursor_p->Glyph.Attributes.blink = NH_FALSE;
            Grid_p->Cursor_p->dirty = NH_TRUE;
            Grid_p->Cursor_p = NULL;
        }
        NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
    }

    nh_terminal_Tile *Tile_p = ((nh_List*)Grid_p->Rows.pp[Update_p->row])->pp[Update_p->col];
    nh_terminal_Tile *OldCursor_p = Grid_p->Cursor_p;

    if (OldCursor_p) {
        // Update old cursor tile.
        OldCursor_p->Glyph.Attributes.blink = NH_FALSE;
        OldCursor_p->dirty = NH_TRUE;
    }

    // Set new cursor.
    Grid_p->Cursor_p = Tile_p;
    Grid_p->Cursor_p->Glyph.Attributes.blink = NH_TRUE;
    Grid_p->Cursor_p->dirty = NH_TRUE;

    // Trigger blink at move.
    int row, col = 0;
    nh_terminal_getTileRowAndColumn(Grid_p, OldCursor_p, &row, &col);
 
    if (row != Update_p->row || col != Update_p->col) {
        State_p->Blink.on = NH_TRUE;
        State_p->Blink.LastBlink = nh_core_getSystemTime();
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_updateTileVertices(
    nh_tty_Glyph *Glyph_p, nh_terminal_GraphicsState *State_p, nh_terminal_Grid *Grid_p, int col,
    int row, nh_terminal_Tile *Tile_p, NH_BOOL foreground)
{
NH_TERMINAL_BEGIN()

    if (foreground) {
        NH_TERMINAL_CHECK(nh_terminal_getForegroundVertices(
            State_p, Grid_p, Glyph_p, col, row, Tile_p->Foreground.vertices_p
        ))
    } else {
        NH_TERMINAL_CHECK(nh_terminal_getBackgroundVertices(
            State_p, Grid_p, Glyph_p, col, row, Tile_p->Background.vertices_p
        ))
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_updateTile(
    nh_terminal_Grid *Grid_p, void *state_p, nh_terminal_TileUpdate *Update_p, NH_BOOL *update_p)
{
NH_TERMINAL_BEGIN()

    // Only update cursor tile in case of cursor flag.
    if (Update_p->cursor) {
        if (update_p) {*update_p = NH_TRUE;}
        NH_TERMINAL_END(nh_terminal_updateCursorTile(Grid_p, state_p, Update_p))
    }

    nh_terminal_Tile *Tile_p = ((nh_List*)Grid_p->Rows.pp[Update_p->row])->pp[Update_p->col];

    // Compare codepoint.
    if (Tile_p->Glyph.codepoint != Update_p->Glyph.codepoint || Tile_p->Glyph.mark != Update_p->Glyph.mark)
    {
        NH_TERMINAL_CHECK(nh_terminal_updateTileVertices(
            &Update_p->Glyph, state_p, Grid_p, Update_p->col, Update_p->row, Tile_p, NH_TRUE))
        NH_TERMINAL_CHECK(nh_terminal_updateTileVertices(
            &Update_p->Glyph, state_p, Grid_p, Update_p->col, Update_p->row, Tile_p, NH_FALSE))
        Tile_p->dirty = NH_TRUE;
    }

    // Compare attributes.
    if (nh_terminal_compareForegroundAttributes(&Tile_p->Glyph, &Update_p->Glyph)
    ||  nh_terminal_compareBackgroundAttributes(&Tile_p->Glyph, &Update_p->Glyph))
    {
        Tile_p->dirty = NH_TRUE;
    }

    if (Tile_p->dirty) {
        if (Tile_p == Grid_p->Cursor_p) {
            NH_BOOL blink = Grid_p->Cursor_p->Glyph.Attributes.blink;
            memcpy(&Tile_p->Glyph, &Update_p->Glyph, sizeof(nh_tty_Glyph));
            Tile_p->Glyph.Attributes.blink = blink;
        } else {
            // This may overwrite the cursor tile, which could cause the blink attribute to disappear.
            // Which is why we need the if above.
            memcpy(&Tile_p->Glyph, &Update_p->Glyph, sizeof(nh_tty_Glyph));
        }
        if (update_p) {*update_p = NH_TRUE;}
    }

    // Update or reset right gap tile if necessary.
    if (Update_p->col == Grid_p->cols-2) {
        if (Update_p->Glyph.mark & NH_TTY_MARK_LINE_HORIZONTAL) {
            Update_p->col++;
            if (!(Update_p->Glyph.mark & NH_TTY_MARK_LINE_GRAPHICS)) {
                Update_p->Glyph.codepoint = ' ';
            }
            NH_TERMINAL_CHECK(nh_terminal_updateTile(Grid_p, state_p, Update_p, update_p))
        } else if (Update_p->Glyph.Attributes.reverse == NH_TRUE) {
            Update_p->Glyph.codepoint = 0;
            Update_p->col++;
            NH_TERMINAL_CHECK(nh_terminal_updateTile(Grid_p, state_p, Update_p, update_p))
        } else {
            Update_p->col++;
            memset(&Update_p->Glyph, 0, sizeof(nh_tty_Glyph));
            NH_TERMINAL_CHECK(nh_terminal_updateTile(Grid_p, state_p, Update_p, update_p))
        } 
    }
    // Update or reset bottom gap tile if necessary.
    if (Update_p->row == Grid_p->rows-2) {
        if (Update_p->Glyph.mark & NH_TTY_MARK_LINE_VERTICAL) {
            Update_p->row++;
            NH_TERMINAL_CHECK(nh_terminal_updateTile(Grid_p, state_p, Update_p, update_p))
        } else if (Update_p->col == 0 && (Update_p->Glyph.codepoint == 0 || Update_p->Glyph.codepoint == ' ')) {
            Update_p->row++;
            NH_TERMINAL_CHECK(nh_terminal_updateTile(Grid_p, state_p, Update_p, update_p))
        } else {
            Update_p->row++;
            memset(&Update_p->Glyph, 0, sizeof(nh_tty_Glyph));
            NH_TERMINAL_CHECK(nh_terminal_updateTile(Grid_p, state_p, Update_p, update_p))
        }
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_updateGrid(
    nh_terminal_Grid *Grid_p, void *state_p, nh_gfx_Text *Text_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_GraphicsState *State_p = state_p;
    nh_terminal_Config Config = nh_terminal_getConfig();

    State_p->FontInstance_p = nh_gfx_claimFontInstance(
        State_p->Fonts.pp[State_p->font], Config.fontSize
    );

    // Free data.
    nh_core_freeList(&State_p->Glyphs, NH_TRUE);
    nh_core_freeList(&State_p->Codepoints, NH_TRUE);

    nh_core_freeHashMap(State_p->Map);
    State_p->Map = nh_core_createHashMap();

    nh_terminal_freeGrid(Grid_p);

    // Update data.
    Grid_p->Size.width = State_p->Viewport_p->Settings.Size.width;
    Grid_p->Size.height = State_p->Viewport_p->Settings.Size.height;

    Grid_p->TileSize.width = nh_gfx_getTextWidth(Text_p);
    Grid_p->TileSize.height = Config.fontSize;

    Grid_p->cols = Grid_p->Size.width / nh_gfx_getTextWidth(Text_p);
    Grid_p->rows = Grid_p->Size.height / (Config.fontSize+abs(State_p->FontInstance_p->descender));

    // Add gap tiles.
    Grid_p->cols += 1;
    Grid_p->rows += 1;

    Grid_p->Updates_pp = nh_core_allocate(sizeof(void*) * Grid_p->rows);
    NH_TERMINAL_CHECK_MEM(Grid_p->Updates_pp)

    Grid_p->updates_pp = nh_core_allocate(sizeof(void*) * Grid_p->rows);
    NH_TERMINAL_CHECK_MEM(Grid_p->updates_pp)

    for (int row = 0; row < Grid_p->rows; ++row) {
        Grid_p->Updates_pp[row] = nh_core_allocate(sizeof(nh_terminal_TileUpdate) * Grid_p->cols);
        NH_TERMINAL_CHECK_MEM(Grid_p->Updates_pp[row])
        Grid_p->updates_pp[row] = nh_core_allocate(sizeof(NH_BOOL) * Grid_p->cols);
        NH_TERMINAL_CHECK_MEM(Grid_p->updates_pp[row])
        memset(Grid_p->updates_pp[row], 0, sizeof(NH_BOOL)*Grid_p->cols);
    }

    for (int row = 0; row < Grid_p->rows; ++row) {
        for (int col = 0; col < Grid_p->cols; ++col) {
            nh_terminal_Tile *Tile_p =
                nh_terminal_getTile(Grid_p, row, col);
            NH_TERMINAL_CHECK_NULL(Tile_p)
            NH_TERMINAL_CHECK(nh_terminal_updateTileVertices(
                &Tile_p->Glyph, state_p, Grid_p, col, row, Tile_p, NH_TRUE))
            NH_TERMINAL_CHECK(nh_terminal_updateTileVertices(
                &Tile_p->Glyph, state_p, Grid_p, col, row, Tile_p, NH_FALSE))
        }
    }

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_updateBoxes(
    nh_terminal_Grid *Grid_p, void *state_p, nh_Array *Boxes_p)
{   
NH_TERMINAL_BEGIN()

    if (Grid_p->Boxes.length > 0) {
        nh_core_freeArray(&Grid_p->Boxes);
    }

    for (int i = 0; i < Boxes_p->length; ++i) 
    {
        nh_terminal_Box *Box_p = nh_core_incrementArray(&Grid_p->Boxes);
        NH_TERMINAL_CHECK_MEM(Box_p)
        *Box_p = ((nh_terminal_Box*)Boxes_p->p)[i];

        if (Box_p->UpperLeft.x == Box_p->LowerRight.x && Box_p->UpperLeft.y == Box_p->UpperLeft.y) {
            NH_TERMINAL_CHECK(nh_terminal_getOutlineVertices(state_p, Grid_p, Box_p, NH_TRUE))
            NH_TERMINAL_CHECK(nh_terminal_getOutlineVertices(state_p, Grid_p, Box_p, NH_FALSE))
        } else {
            NH_TERMINAL_CHECK(nh_terminal_getBoxVertices(state_p, Grid_p, Box_p, NH_TRUE))
            NH_TERMINAL_CHECK(nh_terminal_getBoxVertices(state_p, Grid_p, Box_p, NH_FALSE))
        }
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

