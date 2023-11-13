#ifndef NH_TTY_CONTEXT_MENU_H
#define NH_TTY_CONTEXT_MENU_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "View.h"
#include "../Common/Includes.h"

#include "../../nhcore/Util/List.h"

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_ContextMenu {
        struct nh_tty_ContextMenu *Parent_p;
        nh_encoding_UTF32String Name;
        NH_BOOL active;
        NH_BOOL hit;
        nh_PixelPosition Position;
        int cCol, cRow;
        nh_List Items;
    } nh_tty_ContextMenu;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    void nh_tty_freeContextMenu(
        nh_tty_ContextMenu *Menu_p
    );

    void nh_tty_updateContextMenuHit(
        nh_tty_ContextMenu *Menu_p, nh_tty_ContextMenu *Parent_p, int x, int y, NH_BOOL activate
    );

    nh_tty_ContextMenu *nh_tty_isContextMenuHit(
        nh_tty_ContextMenu *Menu_p, nh_tty_ContextMenu *Parent_p, NH_BOOL recursive, int x, int y
    );

    nh_tty_ContextMenu *nh_tty_createMouseMenu1(
        int x, int y 
    );

    nh_tty_ContextMenu *nh_tty_createMouseMenu2(
        int x, int y 
    );

    NH_TTY_RESULT nh_tty_handleMouseMenuPress(
        nh_tty_ContextMenu *Root_p, nh_tty_ContextMenu *Menu_p
    );

    NH_TTY_RESULT nh_tty_drawContextMenuRecursively(
        nh_tty_ContextMenu *Menu_p, nh_tty_Row *Grid_p
    );

/** @} */

#endif 
