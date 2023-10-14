// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Micro.h"
#include "Program.h"
#include "TTY.h"
#include "Draw.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MICRO TILES =====================================================================================

nh_tty_Tile *nh_tty_createMicroTile(
    nh_tty_Tile *Parent_p, nh_tty_Program *Program_p, int index)
{
NH_TTY_BEGIN()

    nh_tty_MicroTile *Tile_p = nh_core_allocate(sizeof(nh_tty_MicroTile));
    NH_TTY_CHECK_MEM_2(NULL, Tile_p)

    Tile_p->Program_p = Program_p == NULL || Parent_p == NULL ? 
        Program_p : NH_TTY_MICRO_TILE(Parent_p)->Program_p;
 
NH_TTY_END(nh_tty_createTile(Tile_p, NH_TTY_TILE_TYPE_MICRO, Parent_p, index))
}

void nh_tty_destroyMicroTile(
    nh_tty_MicroTile *Tile_p)
{
NH_TTY_BEGIN()

    if (Tile_p->Program_p) {
        nh_tty_destroyProgramInstance(Tile_p->Program_p);
    }
    nh_core_free(Tile_p);

NH_TTY_SILENT_END()
}

NH_TTY_RESULT nh_tty_getMicroTiles(
    nh_tty_MicroWindow *Window_p, nh_List *List_p)
{
NH_TTY_BEGIN()

    *List_p = nh_core_initList(4);

    for (int i = 0; Window_p->Tabs_p && i < Window_p->Tabs_p->size; ++i) {
        nh_List List = nh_tty_getTiles(((nh_tty_MicroTab*)Window_p->Tabs_p->pp[i])->RootTile_p);
        nh_core_appendItemsToList(List_p, &List);
        nh_core_freeList(&List, NH_FALSE);
    }

NH_TTY_END(NH_TTY_SUCCESS)
}

// MICRO TABS ======================================================================================

static nh_tty_MicroTab *nh_tty_createMicroTab(
    nh_tty_ProgramPrototype *Prototype_p, NH_BOOL once)
{
NH_TTY_BEGIN()

    nh_tty_MicroTab *Tab_p = nh_core_allocate(sizeof(nh_tty_MicroTab));
    NH_TTY_CHECK_MEM_2(NULL, Tab_p)

    nh_tty_Tile *Tile_p = nh_tty_createMicroTile(NULL, nh_tty_createProgramInstance(Prototype_p, once), 0);
    NH_TTY_CHECK_MEM_2(NULL, Tile_p)

    Tab_p->RootTile_p  = Tile_p;
    Tab_p->LastFocus_p = Tile_p;
    Tab_p->Tile_p      = Tile_p;
    Tab_p->Prototype_p = Prototype_p;

NH_TTY_END(Tab_p)
}

nh_List *nh_tty_createMicroTabs(
    nh_tty_TTY *TTY_p)
{
NH_TTY_BEGIN()

    nh_List *Tabs_p = nh_core_allocate(sizeof(nh_List));
    NH_TTY_CHECK_MEM_2(NULL, Tabs_p)

    *Tabs_p = nh_core_initList(8); // Don't change size to TTY_p->Prototypes.size, it might not be initialized.

    for (int i = 0; i < TTY_p->Prototypes.size; ++i) {
        nh_tty_ProgramPrototype *Prototype_p = TTY_p->Prototypes.pp[i];
        nh_tty_MicroTab *Tab_p = nh_tty_createMicroTab(Prototype_p, NH_FALSE);
        NH_TTY_CHECK_NULL_2(NULL, Tab_p)
        nh_core_appendToList(Tabs_p, Tab_p);
    }

NH_TTY_END(Tabs_p)
}

static void nh_tty_freeMicroTab(
    nh_tty_MicroTab *Tab_p)
{
NH_TTY_BEGIN()

    nh_tty_closeTile(Tab_p->RootTile_p, Tab_p);
    nh_core_free(Tab_p);

NH_TTY_SILENT_END()
}

NH_TTY_RESULT nh_tty_appendMicroTab(
    nh_tty_MicroWindow *Window_p, nh_tty_ProgramPrototype *Prototype_p, NH_BOOL once)
{
NH_TTY_BEGIN()

    nh_tty_MicroTab *Tab_p = nh_tty_createMicroTab(Prototype_p, once);
    NH_TTY_CHECK_NULL(Tab_p)

    nh_core_appendToList(Window_p->Tabs_p, Tab_p);

NH_TTY_END(NH_TTY_SUCCESS)
}

// MICRO WINDOW ====================================================================================

nh_tty_MicroWindow nh_tty_initMicroWindow(
    nh_List *MicroTabs_p)
{
NH_TTY_BEGIN()

    nh_tty_MicroWindow Window;

    Window.Tabs_p = MicroTabs_p;
    Window.current = 0;
    Window.View_p = nh_tty_createView(NULL, NULL, NH_FALSE);

NH_TTY_END(Window)
}

NH_TTY_RESULT nh_tty_destroyMicroWindow(
    nh_tty_MicroWindow *Window_p)
{
NH_TTY_BEGIN()

    if (Window_p->Tabs_p) {
        for (int i = 0; i < Window_p->Tabs_p->size; ++i) {
            nh_tty_freeMicroTab(Window_p->Tabs_p->pp[i]);
        }
        nh_core_freeList(Window_p->Tabs_p, NH_FALSE);
        nh_core_free(Window_p->Tabs_p);
        Window_p->Tabs_p = NULL;
    }

    NH_TTY_CHECK(nh_tty_destroyView(NULL, Window_p->View_p))
 
NH_TTY_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_drawMicroWindow(
    nh_tty_MicroWindow *Window_p, nh_tty_Glyph *Glyphs_p, int cols, int rows, int row, NH_BOOL standardIO)
{
NH_TTY_BEGIN()

    if (!Window_p->Tabs_p || Window_p->Tabs_p->size == 0) {NH_TTY_END(NH_TTY_SUCCESS)}

    Window_p->View_p->cols = cols;
    Window_p->View_p->rows = rows;
    Window_p->View_p->standardIO = standardIO;

    nh_tty_Tile *RootTile_p = ((nh_tty_MicroTab*)Window_p->Tabs_p->pp[Window_p->current])->RootTile_p;

    nh_tty_updateTiling(RootTile_p, Window_p->View_p->rows, Window_p->View_p->cols);
    nh_List Tiles = nh_tty_getTiles(RootTile_p);

    NH_TTY_CHECK(nh_tty_updateView(Window_p->View_p, NULL))
    NH_TTY_CHECK(nh_tty_refreshGrid1Row(&Tiles, Window_p->View_p, row, NH_FALSE))

    memcpy(Glyphs_p, Window_p->View_p->Grid1_p[row].Glyphs_p, sizeof(nh_tty_Glyph)*cols);
    nh_core_freeList(&Tiles, NH_FALSE);
 
NH_TTY_END(NH_TTY_SUCCESS)
}

// CURRENT PROGRAM =================================================================================

nh_tty_Program *nh_tty_getCurrentProgram(
    nh_tty_MicroWindow *Window_p)
{
NH_TTY_BEGIN()

    nh_tty_MicroTab *Tab_p = !Window_p->Tabs_p || Window_p->Tabs_p->size <= Window_p->current 
        ? NULL : Window_p->Tabs_p->pp[Window_p->current];

NH_TTY_END(!Tab_p ? NULL : NH_TTY_MICRO_TILE(Tab_p->Tile_p)->Program_p)
}

