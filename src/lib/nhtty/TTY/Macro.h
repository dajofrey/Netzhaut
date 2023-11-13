#ifndef NH_TTY_TTY_MACRO_H
#define NH_TTY_TTY_MACRO_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Tiling.h"
#include "TopBar.h"
#include "Micro.h"
#include "ContextMenu.h"

#include "../Common/Includes.h"

#include "../../nhcore/Util/String.h"
#include "../../nhcore/Util/List.h"

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_MacroTab {
        nh_tty_TopBar TopBar;
        nh_tty_MicroWindow MicroWindow;
    } nh_tty_MacroTab;

    typedef struct nh_tty_MacroTile {
        int current;       /**<Current focused macro tab.*/
        nh_List MacroTabs; /**<List of macro tabs.*/
    } nh_tty_MacroTile;

    typedef struct nh_tty_MacroWindow {
        nh_tty_Tiling Tiling;
        nh_tty_ContextMenu *MouseMenu_p;
        NH_BOOL refreshGrid1;
        NH_BOOL refreshGrid2;
        NH_BOOL refreshCursor;
        NH_BOOL close;
        nh_tty_Tile *RootTile_p;
        nh_tty_Tile *LastFocus_p;
        nh_tty_Tile *Tile_p;
    } nh_tty_MacroWindow;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_MacroWindow *nh_tty_insertAndFocusWindow(
        void *TTY_p, int index
    );
    
    NH_TTY_RESULT nh_tty_destroyWindows(
        void *TTY_p
    );

    NH_TTY_RESULT nh_tty_updateMacroWindow(
        nh_tty_MacroWindow *Window_p
    );

    NH_TTY_RESULT nh_tty_handleMacroWindowInput(
        nh_tty_MacroWindow *Tab_p, nh_wsi_Event Event 
    );

    nh_tty_Tile *nh_tty_createMacroTile(
        nh_tty_Tile *Parent_p, nh_List **MicroTabs_pp, int index
    );

    void nh_tty_destroyMacroTile(
        nh_tty_MacroTile *Tile_p
    );

    NH_TTY_RESULT nh_tty_drawTileRow(
        nh_tty_Tile *Tile_p, nh_tty_Row *Row_p, int row
    );

    void nh_tty_drawEmptyGlyph(
        nh_tty_Glyph *Glyph_p, NH_BOOL reverse
    );

/** @} */

#endif // NH_TTY_TTY_MACRO_H
