#ifndef NH_TTY_TILING_H
#define NH_TTY_TILING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "TopBar.h"
#include "../Common/Includes.h"
#include "../../nhcore/Util/LinkedList.h"

typedef struct nh_tty_MacroWindow nh_tty_MacroWindow;

#endif

/** @addtogroup lib_nhtty_enums
 *  @{
 */

    typedef enum NH_TTY_TILE_TYPE_E { 
        NH_TTY_TILE_TYPE_MACRO,
        NH_TTY_TILE_TYPE_MICRO,
    } NH_TTY_TILE_TYPE_E;

    typedef enum NH_TTY_TILING_MODE_E { 
        NH_TTY_TILING_MODE_MICRO,
        NH_TTY_TILING_MODE_MACRO,
    } NH_TTY_TILING_MODE_E;

    typedef enum NH_TTY_TILING_STAGE_E { 
        NH_TTY_TILING_STAGE_DONE,
        NH_TTY_TILING_STAGE_OVERVIEW,
        NH_TTY_TILING_STAGE_INSERT,
    } NH_TTY_TILING_STAGE_E;

    typedef enum NH_TTY_TILE_ORIENTATION { 
        NH_TTY_TILE_ORIENTATION_VERTICAL,
        NH_TTY_TILE_ORIENTATION_HORIZONTAL,
    } NH_TTY_TILE_ORIENTATION;

/** @} */

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_Tiling {
        NH_TTY_TILING_MODE_E mode;
        NH_TTY_TILING_STAGE_E stage;
    } nh_tty_Tiling;

    typedef struct nh_tty_Tile {
        NH_TTY_TILE_TYPE_E type;
        void *p;
        NH_TTY_TILE_ORIENTATION orientation;
        NH_BOOL rightSeparator; 
        NH_BOOL refresh;
        NH_BOOL close;
        int rowPosition, colPosition;
        int rowSize, colSize;
        nh_LinkedList Children;
        struct nh_tty_Tile *Parent_p;
        struct nh_tty_Tile *Prev_p;
    } nh_tty_Tile;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_Tile *nh_tty_createTile(
        void *p, NH_TTY_TILE_TYPE_E type, nh_tty_Tile *Parent_p, int index
    );

    NH_TTY_RESULT nh_tty_closeTile(
        nh_tty_Tile *Tile_p, void *MicroTab_p
    );

    nh_tty_Tile *nh_tty_switchTile(
        nh_tty_MacroWindow *Window_p, nh_tty_Tile *Tile_p, int direction
    );

    NH_TTY_RESULT nh_tty_resetTiling(
        nh_tty_MacroWindow *Tab_p
    );

    NH_TTY_RESULT nh_tty_updateTiling(
        nh_tty_Tile *RootTile_p, int viewRows, int viewCols
    );

    NH_TTY_RESULT nh_tty_handleTilingInput(
        nh_tty_MacroWindow *Tab_p, nh_wsi_KeyboardEvent Event 
    );

    nh_List nh_tty_getTiles(
        nh_tty_Tile *Root_p
    );
    
/** @} */

#endif 
