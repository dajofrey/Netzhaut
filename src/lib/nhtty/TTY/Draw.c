// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 *
 * See https://vt100.net/docs/vt100-ug/chapter3.html for escape sequences.
 */

// DEFINE ==========================================================================================

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

// INCLUDES ========================================================================================

#include "Draw.h"
#include "TopBar.h"
#include "SideBar.h"
#include "View.h"
#include "Program.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// CURSOR ==========================================================================================

NH_TTY_RESULT nh_tty_getCursorPosition(
    nh_tty_Tile *MacroTile_p, nh_tty_Tile *MicroTile_p, NH_BOOL standardIO, int *x_p, int *y_p)
{
NH_TTY_BEGIN()

    *x_p = -1;
    *y_p = -1;

    if (NH_TTY_MACRO_TAB(MacroTile_p)->TopBar.hasFocus) {
        NH_TTY_CHECK(nh_tty_getTopBarCursor(&NH_TTY_MACRO_TAB(MacroTile_p)->TopBar, x_p, y_p, MacroTile_p->rowPosition == 0))
    }
    else if (NH_TTY_MICRO_TILE(MicroTile_p)->Program_p != NULL && NH_TTY_MICRO_TILE(MicroTile_p)->Program_p->Prototype_p->Callbacks.getCursorPosition_f != NULL) {
        NH_TTY_CHECK(NH_TTY_MICRO_TILE(MicroTile_p)->Program_p->Prototype_p->Callbacks.getCursorPosition_f(NH_TTY_MICRO_TILE(MicroTile_p)->Program_p, x_p, y_p))
        if (*x_p < 0 || *y_p < 0) {
             // Indicates that the program doesn't want the cursor to be shown.
             NH_TTY_END(NH_TTY_SUCCESS)
        }
        *y_p += 2; 
        if (*x_p > -1 && *y_p > -1) {
            *x_p += MicroTile_p->colPosition;
            *y_p += MicroTile_p->rowPosition;
        }
    }

    if (*x_p > -1) {
        *x_p += MacroTile_p->colPosition + 1;
        *y_p += MacroTile_p->rowPosition;
    }

    nh_tty_Config Config = nh_tty_getConfig();
    *x_p += Config.Sidebar.state != NH_TTY_SIDEBAR_STATE_OFF && !standardIO ? 2 : 0;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_refreshCursor(
    nh_tty_TTY *TTY_p) 
{
NH_TTY_BEGIN()

    nh_tty_View *View_p = TTY_p->Views.pp[0];
    int x = -1, y = -1;
 
    if (TTY_p->hasFocus) {
        nh_tty_Tile *MicroTile_p = NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(TTY_p->Window_p->Tile_p))->Tile_p;
        NH_TTY_CHECK(nh_tty_getCursorPosition(TTY_p->Window_p->Tile_p, MicroTile_p, View_p->standardIO, &x, &y))
    }

    NH_TTY_CHECK(nh_tty_forwardCursor(View_p, x, y))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

static void nh_tty_normalizeGlyph(
    nh_tty_Glyph *Glyph_p)
{
NH_TTY_BEGIN()

    memset(Glyph_p, 0, sizeof(nh_tty_Glyph));
    Glyph_p->codepoint = ' ';

NH_TTY_SILENT_END()
}

static void nh_tty_drawVerticalBorderGlyph(
    nh_tty_Glyph *Glyph_p)
{
NH_TTY_BEGIN()

    memset(Glyph_p, 0, sizeof(nh_tty_Glyph));

    Glyph_p->codepoint = ' ';
    Glyph_p->mark = NH_TTY_MARK_LINE_VERTICAL | NH_TTY_MARK_ACCENT;
    Glyph_p->Attributes.reverse = NH_TRUE;

NH_TTY_SILENT_END()
}

static NH_TTY_RESULT nh_tty_draw(
    nh_tty_Tile *Tile_p, nh_tty_View *View_p, int row)
{
NH_TTY_BEGIN()

    // Normalize glyphs.
    for (int i = 0; i < Tile_p->colSize; ++i) {
        nh_tty_normalizeGlyph(View_p->Row.Glyphs_p+i);
    }

    // Get relative row with 0 being the first row of the tile.
    row = row - Tile_p->rowPosition;

    int cols = Tile_p->colSize;
    NH_BOOL topbar = NH_FALSE;

    // Get total rows and subtract row by vertical width of topbar.
    if (Tile_p->type == NH_TTY_TILE_TYPE_MACRO) {
        topbar = row == 0;
    } else {
        topbar = Tile_p->rowPosition != 0 && row == 0;
    }

    // Draw vertical border and subtract from cols, if necessary.
    if (Tile_p->rightSeparator) {
        nh_tty_drawVerticalBorderGlyph(&View_p->Row.Glyphs_p[cols-1]);
        cols--;
    }

    if (topbar) {
        if (Tile_p->type == NH_TTY_TILE_TYPE_MACRO) { 
            NH_TTY_END(nh_tty_drawTopBarRow(
                &NH_TTY_MACRO_TAB(Tile_p)->TopBar, &NH_TTY_MACRO_TAB(Tile_p)->MicroWindow, 
                View_p->Row.Glyphs_p, cols, row, View_p->standardIO
            ))
        } else {
            NH_TTY_END(nh_tty_drawTopBarRow(
                NULL, NULL, View_p->Row.Glyphs_p, cols, row, View_p->standardIO
            ))
        }
    }
 
    if (Tile_p->type == NH_TTY_TILE_TYPE_MACRO) { 
        NH_TTY_CHECK(nh_tty_drawMicroWindow(
            &NH_TTY_MACRO_TAB(Tile_p)->MicroWindow, View_p->Row.Glyphs_p, cols, Tile_p->rowSize, row, View_p->standardIO
        ))
    } else if (NH_TTY_MICRO_TILE(Tile_p)->Program_p) {
        NH_TTY_CHECK(NH_TTY_MICRO_TILE(Tile_p)->Program_p->Prototype_p->Callbacks.draw_f(
            NH_TTY_MICRO_TILE(Tile_p)->Program_p, View_p->Row.Glyphs_p, cols, Tile_p->rowSize-1, row-1
        ))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// REFRESH =========================================================================================

static NH_TTY_RESULT nh_tty_postProcessRow(
    nh_tty_View *View_p, int row)
{
NH_TTY_BEGIN()

    nh_tty_Row *Row_p = View_p->Grid1_p+row;

    if (View_p->standardIO) {NH_TTY_END(NH_TTY_SUCCESS)}

    // Post process line.
    for (int i = 0; i < View_p->cols; ++i) {
        nh_tty_Glyph *Glyph_p = &Row_p->Glyphs_p[i];
        if (Glyph_p->mark & NH_TTY_MARK_LINE_VERTICAL) {
            Glyph_p->codepoint = 'x';
            Glyph_p->Attributes.reverse = NH_FALSE;
            Glyph_p->mark |= NH_TTY_MARK_LINE_GRAPHICS;
            if (((Glyph_p+1)->Attributes.reverse && (Glyph_p+1)->mark & NH_TTY_MARK_LINE_HORIZONTAL)
            &&  ((Glyph_p-1)->Attributes.reverse && (Glyph_p-1)->mark & NH_TTY_MARK_LINE_HORIZONTAL)) {
                Glyph_p->Attributes.reverse = NH_TRUE;
                continue;
            } 
            if ((Glyph_p+1)->mark & NH_TTY_MARK_LINE_HORIZONTAL) {
                if ((Glyph_p-1)->mark & NH_TTY_MARK_LINE_HORIZONTAL) {
                    if (row > 0 && ((Row_p-1)->Glyphs_p[i].mark & NH_TTY_MARK_LINE_VERTICAL)) {
                        if ((Glyph_p+1)->Attributes.reverse) {
                            Glyph_p->codepoint = 'b';
                        } else if ((Glyph_p-1)->Attributes.reverse) {
                            Glyph_p->codepoint = 'c';
                        } else {
                            Glyph_p->codepoint = 'n';
                        }
                    } else {
                        Glyph_p->codepoint = 'w';
                    }
                }
                else {
                    Glyph_p->codepoint = (Glyph_p+1)->Attributes.reverse ? 'd' : 't';
                }
            }
            else if (i < View_p->cols-1 && ((Glyph_p-1)->mark & NH_TTY_MARK_LINE_HORIZONTAL)) {
                Glyph_p->codepoint = (Glyph_p-1)->Attributes.reverse ? 'e' : 'u';
            }
        }
        if (Glyph_p->mark & NH_TTY_MARK_LINE_HORIZONTAL && !Glyph_p->Attributes.reverse) {
            if (row > 0 && ((Row_p-1)->Glyphs_p[i].mark & NH_TTY_MARK_LINE_VERTICAL)) {
                Glyph_p->codepoint = 'v';
            }
        }
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_refreshGrid1Row(
    nh_List *Tiles_p, nh_tty_View *View_p, int row, NH_BOOL sidebar)
{
NH_TTY_BEGIN()

    sidebar = sidebar && !View_p->standardIO;

    memset(View_p->Row.Glyphs_p, 0, sizeof(nh_tty_Glyph)*View_p->cols);
    int offset = sidebar ? 2 : 0;

    for (int col = offset; col < View_p->cols;) {
        for (int tile = 0; tile < Tiles_p->size; ++tile) {

            nh_tty_Tile *Tile_p = Tiles_p->pp[tile];
            if (Tile_p->Children.count > 0) {continue;}

            Tile_p->refresh = NH_FALSE;

            if (Tile_p->rowPosition <= row
            &&  Tile_p->rowPosition  + Tile_p->rowSize > row
            &&  Tile_p->colPosition == col-offset)
            {
                NH_TTY_CHECK(nh_tty_draw(Tile_p, View_p, row))

                for (int i = 0; i < Tile_p->colSize; ++i) {
                    nh_tty_Glyph *Glyph_p = View_p->Grid1_p[row].Glyphs_p+col+i;
                    if (memcmp(Glyph_p, View_p->Row.Glyphs_p+i, sizeof(nh_tty_Glyph))) {
                        View_p->Grid1_p[row].update_p[col+i] = NH_TRUE;
                    }
                }

                memcpy(View_p->Grid1_p[row].Glyphs_p+col, View_p->Row.Glyphs_p, 
                    sizeof(nh_tty_Glyph)*Tile_p->colSize);

                col += Tile_p->colSize;
                break;
            }
        }
    }

    if (sidebar) {
        NH_TTY_CHECK(nh_tty_drawSideBarRow(View_p->Grid1_p[row].Glyphs_p, row))
        View_p->Grid1_p[row].update_p[0] = NH_TRUE;
        View_p->Grid1_p[row].update_p[1] = NH_TRUE;
    }

    NH_TTY_CHECK(nh_tty_postProcessRow(View_p, row))

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_refreshGrid1(
    nh_tty_TTY *TTY_p) 
{
NH_TTY_BEGIN()

    nh_tty_View *View_p = TTY_p->Views.pp[0];
    nh_tty_Config Config = nh_tty_getConfig();
    int offset = Config.Sidebar.state != NH_TTY_SIDEBAR_STATE_OFF && !View_p->standardIO ? 2 : 0;

    nh_tty_updateTiling(TTY_p->Window_p->RootTile_p, View_p->rows, View_p->cols-offset);
    nh_List Tiles = nh_tty_getTiles(TTY_p->Window_p->RootTile_p);

    for (int row = 0; row < View_p->rows; ++row) {
        NH_TTY_CHECK(nh_tty_refreshGrid1Row(&Tiles, View_p, row, Config.Sidebar.state != NH_TTY_SIDEBAR_STATE_OFF))
    }

    nh_core_freeList(&Tiles, NH_FALSE);

    NH_TTY_CHECK(nh_tty_forwardGrid1(View_p))

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}
