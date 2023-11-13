// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "SideBar.h"
#include "Program.h"
#include "TTY.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// INPUT ===========================================================================================

NH_TTY_RESULT nh_tty_handleSideBarHit(
    NH_WSI_MOUSE_E mouse, int row)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();

    switch (mouse) {
        case NH_WSI_MOUSE_LEFT :
            nh_tty_insertAndFocusWindow(TTY_p, row);
            break;
        case NH_WSI_MOUSE_RIGHT :
            NH_TTY_MACRO_TILE(TTY_p->Window_p->Tile_p)->current = row;
            break;
    }
    
NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

NH_TTY_RESULT nh_tty_drawSideBarRow(
    nh_tty_Glyph *Glyphs_p, int row)
{
NH_TTY_BEGIN()

    nh_tty_TTY *TTY_p = nh_core_getWorkloadArg();
    nh_tty_Config Config = nh_tty_getConfig();

    if (row < Config.windows) {
        Glyphs_p[0].codepoint = ' ';
        Glyphs_p[0].mark = NH_TTY_MARK_ACCENT;
        Glyphs_p[0].Attributes.reverse = nh_core_getListIndex(&TTY_p->Windows, TTY_p->Window_p) == row;
    } else {
        Glyphs_p[0].codepoint = ' ';
        Glyphs_p[0].mark = NH_TTY_MARK_ACCENT;
        Glyphs_p[0].Attributes.reverse = NH_TRUE;
    }

    Glyphs_p[1].mark = NH_TTY_MARK_ACCENT | NH_TTY_MARK_LINE_VERTICAL;
    Glyphs_p[1].Attributes.reverse = NH_FALSE;

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

