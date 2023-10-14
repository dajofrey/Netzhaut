#ifndef NH_TTY_MICRO_H
#define NH_TTY_MICRO_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Program.h"
#include "Tiling.h"
#include "View.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_MicroTile {
        nh_tty_Program *Program_p;
    } nh_tty_MicroTile;

    typedef struct nh_tty_MicroTab {
        nh_tty_ProgramPrototype *Prototype_p; /**<Type of programs spawned by this micro tab.*/
        nh_tty_Tile *RootTile_p;              /**<Root tile.*/
        nh_tty_Tile *Tile_p;                  /**<Current focused tile.*/
        nh_tty_Tile *LastFocus_p;
    } nh_tty_MicroTab;

    typedef struct nh_tty_MicroWindow {
        int current;     /**<Current focused micro tab.*/
        nh_List *Tabs_p; /**<Pointer to micro tabs.*/
        nh_tty_View *View_p;
    } nh_tty_MicroWindow;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_Tile *nh_tty_createMicroTile(
        nh_tty_Tile *Parent_p, nh_tty_Program *Program_p, int index
    );

    void nh_tty_destroyMicroTile(
        nh_tty_MicroTile *Tile_p
    );

    nh_List *nh_tty_createMicroTabs(
        nh_tty_TTY *TTY_p
    );
    
    NH_TTY_RESULT nh_tty_appendMicroTab(
        nh_tty_MicroWindow *Window_p, nh_tty_ProgramPrototype *Prototype_p, NH_BOOL once
    );

    NH_TTY_RESULT nh_tty_getMicroTiles(
        nh_tty_MicroWindow *Window_p, nh_List *List_p
    );

    nh_tty_MicroWindow nh_tty_initMicroWindow(
        nh_List *MicroTabs_p
    );

    NH_TTY_RESULT nh_tty_destroyMicroWindow(
        nh_tty_MicroWindow *Window_p
    );

    NH_TTY_RESULT nh_tty_drawMicroWindow(
        nh_tty_MicroWindow *Window_p, nh_tty_Glyph *Glyphs_p, int cols, int rows, int row, NH_BOOL standardIO
    );

    nh_tty_Program *nh_tty_getCurrentProgram(
        nh_tty_MicroWindow *Window_p
    );

/** @} */

#endif // NH_TTY_MICRO_H
