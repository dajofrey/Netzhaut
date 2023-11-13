// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// DEFINE ==========================================================================================

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

// INCLUDES ========================================================================================

#include "View.h"
#include "Draw.h"
#include "TTY.h"
#include "StandardIO.h"
#include "Program.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"
#include "../../nhcore/Util/RingBuffer.h"
#include "../../nhcore/Common/Macros.h"

#include "../../nhterminal/Terminal/Grid.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// SIZE ============================================================================================

NH_TTY_RESULT nh_tty_getViewSize(
    nh_tty_View *View_p) 
{
NH_TTY_BEGIN()

    if (View_p->standardIO) {
        NH_TTY_CHECK(nh_tty_getStandardOutputWindowSize(&View_p->cols, &View_p->rows))
    }

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_translateMousePosition(
    nh_tty_View *View_p, nh_wsi_MouseEvent Mouse, int *col_p, int *row_p)
{
NH_TTY_BEGIN()

    if (View_p->standardIO) {
        NH_TTY_CHECK(NH_TTY_ERROR_BAD_STATE)
    }

    float relativeX = ((float)Mouse.Position.x)/((float)View_p->Size.width);
    *col_p = (int)(relativeX*((float)View_p->cols));

    float relativeY = ((float)Mouse.Position.y)/((float)View_p->Size.height);
    *row_p = (int)(relativeY*((float)View_p->rows));

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_updateView(
    nh_tty_View *View_p, NH_BOOL *updated_p)
{
NH_TTY_BEGIN()

    if (View_p->cols == View_p->previousCols && View_p->rows == View_p->previousRows &&
        View_p->Size.width == View_p->PreviousSize.width && View_p->Size.height == View_p->PreviousSize.height) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
    }

    if (View_p->Row.Glyphs_p) {
        nh_core_free(View_p->Row.Glyphs_p);
    }
    View_p->Row.Glyphs_p = nh_core_allocate(sizeof(nh_tty_Glyph)*View_p->cols);

    for (int i = 0; i < View_p->previousRows; ++i) {
        nh_core_free(View_p->Grid1_p[i].Glyphs_p);
        nh_core_free(View_p->Grid1_p[i].update_p);
        nh_core_free(View_p->Grid2_p[i].Glyphs_p);
        nh_core_free(View_p->Grid2_p[i].update_p);
    }
    if (View_p->Grid1_p) {
        nh_core_free(View_p->Grid1_p);
        nh_core_free(View_p->Grid2_p);
    }

    View_p->Grid1_p = nh_core_allocate(sizeof(nh_tty_Row)*View_p->rows);
    View_p->Grid2_p = nh_core_allocate(sizeof(nh_tty_Row)*View_p->rows);
 
    for (int i = 0; i < View_p->rows; ++i) {
        View_p->Grid1_p[i].Glyphs_p = nh_core_allocate(sizeof(nh_tty_Glyph)*View_p->cols);
        memset(View_p->Grid1_p[i].Glyphs_p, 0, sizeof(nh_tty_Glyph)*View_p->cols);
        View_p->Grid1_p[i].update_p = nh_core_allocate(sizeof(NH_BOOL)*View_p->cols);
        memset(View_p->Grid1_p[i].update_p, NH_TRUE, sizeof(NH_BOOL)*View_p->cols);

        View_p->Grid2_p[i].Glyphs_p = nh_core_allocate(sizeof(nh_tty_Glyph)*View_p->cols);
        memset(View_p->Grid2_p[i].Glyphs_p, 0, sizeof(nh_tty_Glyph)*View_p->cols);
        View_p->Grid2_p[i].update_p = nh_core_allocate(sizeof(NH_BOOL)*View_p->cols);
        memset(View_p->Grid2_p[i].update_p, NH_TRUE, sizeof(NH_BOOL)*View_p->cols);
    }

    View_p->previousCols = View_p->cols;
    View_p->previousRows = View_p->rows;
    View_p->PreviousSize = View_p->Size;

    if (updated_p) {*updated_p = NH_TRUE;}

NH_TTY_END(NH_TTY_SUCCESS)
}

// CREATE/DESTROY ==================================================================================

static void nh_tty_initTilesBuffer(
    nh_RingBuffer *Buffer_p, int itemCount)
{
NH_TTY_BEGIN()

    for (int i = 0; i < itemCount; ++i) {
        nh_Array *Array_p = nh_core_advanceRingBuffer(Buffer_p);
        *Array_p = nh_core_initArray(sizeof(nh_terminal_TileUpdate), 255);
    }

NH_TTY_SILENT_END()
}

static void nh_tty_initBoxesBuffer(
    nh_RingBuffer *Buffer_p, int itemCount)
{
NH_TTY_BEGIN()

    for (int i = 0; i < itemCount; ++i) {
        nh_Array *Array_p = nh_core_advanceRingBuffer(Buffer_p);
        *Array_p = nh_core_initArray(sizeof(nh_terminal_Box), 16);
    }

NH_TTY_SILENT_END()
}

nh_tty_View *nh_tty_createView(
    nh_tty_TTY *TTY_p, void *p, NH_BOOL standardIO)
{
NH_TTY_BEGIN()

    nh_tty_View View;
    memset(&View, 0, sizeof(nh_tty_View));

    View.standardIO = standardIO;
    View.p = p;

    NH_CORE_CHECK_2(NULL, nh_core_initRingBuffer(
        &View.Forward.Tiles, 64, sizeof(nh_Array), nh_tty_initTilesBuffer
    ))
    NH_CORE_CHECK_2(NULL, nh_core_initRingBuffer(
        &View.Forward.Events, 64, sizeof(nh_wsi_Event), NULL 
    ))
    NH_CORE_CHECK_2(NULL, nh_core_initRingBuffer(
        &View.Forward.Boxes, 16, sizeof(nh_Array), nh_tty_initBoxesBuffer 
    ))

    nh_tty_View *View_p = nh_core_allocate(sizeof(nh_tty_View));
    NH_TTY_CHECK_MEM_2(NULL, View_p)

    *View_p = View;

    if (TTY_p) {
        nh_core_appendToList(&TTY_p->Views, View_p);
    }

NH_TTY_END(View_p)
}

NH_TTY_RESULT nh_tty_destroyView(
    nh_tty_TTY *TTY_p, nh_tty_View *View_p)
{
NH_TTY_BEGIN()

    for (int i = 0; i < 64; ++i) {
        nh_Array *Array_p = nh_core_advanceRingBuffer(&View_p->Forward.Tiles);
        nh_core_freeArray(Array_p);
    }
    for (int i = 0; i < 16; ++i) {
        nh_Array *Array_p = nh_core_advanceRingBuffer(&View_p->Forward.Boxes);
        nh_core_freeArray(Array_p);
    }

    nh_core_freeRingBuffer(&View_p->Forward.Tiles);
    nh_core_freeRingBuffer(&View_p->Forward.Events);
    nh_core_freeRingBuffer(&View_p->Forward.Boxes);
 
    nh_core_free(View_p->Row.Glyphs_p);
    for (int i = 0; i < View_p->rows; ++i) {
        nh_core_free(View_p->Grid1_p[i].Glyphs_p);
        nh_core_free(View_p->Grid1_p[i].update_p);
        nh_core_free(View_p->Grid2_p[i].Glyphs_p);
        nh_core_free(View_p->Grid2_p[i].update_p);
    }
    nh_core_free(View_p->Grid1_p);
    nh_core_free(View_p->Grid2_p);

    if (TTY_p) {
        nh_core_removeFromList2(&TTY_p->Views, NH_TRUE, View_p);
    } else {
        nh_core_free(View_p); 
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// FORWARD FUNCTIONS ===============================================================================
// These functions forward specific TTY internal data to the view for rendering.

NH_TTY_RESULT nh_tty_forwardCursor(
    nh_tty_View *View_p, int x, int y)
{
NH_TTY_BEGIN()

    if (View_p->standardIO) {
        NH_TTY_END(nh_tty_writeCursorToStandardOutput(x, y))
    }

    // Prepare cursor glyph.
    nh_tty_Glyph Glyph;
    memset(&Glyph, 0, sizeof(nh_tty_Glyph));

    // Write to nhterminal.
    nh_Array *Array_p = nh_core_advanceRingBuffer(&View_p->Forward.Tiles);
    nh_core_freeArray(Array_p);

    nh_terminal_TileUpdate Update;
    Update.row = y - 1;
    Update.col = x - 1;
    Update.Glyph = Glyph;
    Update.cursor = NH_TRUE;

    nh_core_appendToArray(Array_p, &Update, 1);

NH_TTY_END(NH_TTY_SUCCESS)
}

static NH_TTY_RESULT nh_tty_setContextMenus(
    nh_tty_ContextMenu *Menu_p, nh_Array *Boxes_p)
{
NH_TTY_BEGIN()

    if (!Menu_p->active || Menu_p->Items.size == 0) {NH_TTY_END(NH_TTY_SUCCESS)}

    nh_terminal_Box *Box_p = nh_core_incrementArray(Boxes_p);

    int width = 0;
    for (int i = 0; i < Menu_p->Items.size; ++i) {
        nh_encoding_UTF32String *String_p = &((nh_tty_ContextMenu*)Menu_p->Items.pp[i])->Name;
        if (String_p->length > width) {width = String_p->length;}
    }

    memset(Box_p, 0, sizeof(nh_terminal_Box));
    Box_p->UpperLeft.x = Menu_p->Position.x-1;
    Box_p->UpperLeft.y = Menu_p->Position.y;
    Box_p->LowerRight.x = Menu_p->Position.x+width+1;
    Box_p->LowerRight.y = Menu_p->Position.y+Menu_p->Items.size;

    for (int i = 0; i < Menu_p->Items.size; ++i) {
        nh_tty_setContextMenus(Menu_p->Items.pp[i], Boxes_p);
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_forwardGrid1(
    nh_tty_View *View_p)
{
NH_TTY_BEGIN()

    if (View_p->standardIO) {
        NH_TTY_END(nh_tty_writeToStandardOutput(View_p->Grid1_p, View_p->cols, View_p->rows))
    }

    // Write to nhterminal.
    nh_Array *Boxes_p = nh_core_advanceRingBuffer(&View_p->Forward.Boxes);
    nh_Array *Array_p = nh_core_advanceRingBuffer(&View_p->Forward.Tiles);
    nh_core_freeArray(Array_p);
    nh_core_freeArray(Boxes_p);

    for (int row = 0; row < View_p->rows; ++row) {
        for (int col = 0; col < View_p->cols; ++col) {
            if (!(View_p->Grid1_p[row].update_p[col])) {continue;}

            nh_terminal_TileUpdate Update;
            Update.row = row;
            Update.col = col;
            Update.Glyph = View_p->Grid1_p[row].Glyphs_p[col];
            Update.cursor = NH_FALSE;
            nh_core_appendToArray(Array_p, &Update, 1);

            View_p->Grid1_p[row].update_p[col] = NH_FALSE;
        }
    }

    // Forward boxes: Context Menu.
    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();
    if (TTY_p->Window_p->MouseMenu_p) {
        nh_tty_setContextMenus(TTY_p->Window_p->MouseMenu_p, Boxes_p);
    }

    // Forward boxes: Tile outlines.
    nh_List MacroTiles = nh_tty_getTiles(TTY_p->Window_p->RootTile_p);
    for (int i = 0; i < MacroTiles.size; ++i) {

        // Skip macro tile without payload.
        if (((nh_tty_Tile*)MacroTiles.pp[i])->p == NULL 
        || NH_TTY_MACRO_TILE(MacroTiles.pp[i])->MacroTabs.size <= 0 
        || NH_TTY_MACRO_TAB(MacroTiles.pp[i])->MicroWindow.Tabs_p == NULL) {continue;}

        nh_List MicroTiles = nh_tty_getTiles(NH_TTY_MICRO_TAB(NH_TTY_MACRO_TAB(MacroTiles.pp[i]))->RootTile_p);
        for (int j = 0; j < MicroTiles.size; ++j) {

            // Skip micro tile without payload.
            if (((nh_tty_Tile*)MicroTiles.pp[j])->p == NULL) {continue;}

            nh_tty_Program *Program_p = NH_TTY_MICRO_TILE(MicroTiles.pp[j])->Program_p;
            nh_tty_Program *CurrentProgram_p = nh_tty_getCurrentProgram(&NH_TTY_MACRO_TAB(TTY_p->Window_p->Tile_p)->MicroWindow);
            if (CurrentProgram_p == Program_p && TTY_p->hasFocus) {continue;}

            int x = 0, y = 0;
            NH_TTY_CHECK(nh_tty_getCursorPosition(MacroTiles.pp[i], MicroTiles.pp[j], View_p->standardIO, &x, &y))

            nh_terminal_Box *Box_p = nh_core_incrementArray(Boxes_p);
            memset(Box_p, 0, sizeof(nh_terminal_Box));
            Box_p->UpperLeft.x = x-2;
            Box_p->UpperLeft.y = y-1;
            Box_p->LowerRight.x = x-2;
            Box_p->LowerRight.y = y-1;
        }
        nh_core_freeList(&MicroTiles, NH_FALSE);
    }
    nh_core_freeList(&MacroTiles, NH_FALSE);

    // Forward boxes: Sidebar.
    nh_tty_Config Config = nh_tty_getConfig();
    for (int i = 0; Config.Sidebar.state && i < Config.windows; ++i) {
        nh_terminal_Box *Box_p = nh_core_incrementArray(Boxes_p);
        memset(Box_p, 0, sizeof(nh_terminal_Box));
        Box_p->accent = NH_TRUE;
        Box_p->UpperLeft.x = -1;
        Box_p->UpperLeft.y = i;
        Box_p->LowerRight.x = -1;
        Box_p->LowerRight.y = i;
    }
 
NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_forwardGrid2(
    nh_tty_View *View_p)
{
NH_TTY_BEGIN()

    // Write to nhterminal.
    nh_Array *Array_p = nh_core_advanceRingBuffer(&View_p->Forward.Tiles);
    nh_core_freeArray(Array_p);

    for (int row = 0; row < View_p->rows; ++row) {
        for (int col = 0; col < View_p->cols; ++col) {
            nh_terminal_TileUpdate Update;
            Update.row = row;
            Update.col = col;
            Update.Glyph = View_p->Grid2_p[row].Glyphs_p[col];
            Update.Glyph.mark |= NH_TTY_MARK_ELEVATED | NH_TTY_MARK_ACCENT;
            Update.cursor = NH_FALSE;
            nh_core_appendToArray(Array_p, &Update, 1);
        }
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_forwardEvent(
    nh_tty_View *View_p, nh_wsi_Event Event)
{
NH_TTY_BEGIN()

    if (View_p->standardIO) {
        NH_TTY_END(NH_TTY_SUCCESS)
    }

    // Write to nhterminal.
    nh_wsi_Event *Event_p = nh_core_advanceRingBuffer(&View_p->Forward.Events);
    NH_TTY_CHECK_MEM(Event_p)
    *Event_p = Event;

NH_TTY_END(NH_TTY_SUCCESS)
}

