#ifndef NH_TERMINAL_TERMINAL_H
#define NH_TERMINAL_TERMINAL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Grid.h"
#include "Graphics.h"

#include "../Common/Config.h"
#include "../Common/Includes.h"

#include "../../nhtty/TTY/View.h"
#include "../../nhcore/Util/Time.h"

#endif

/** @addtogroup lib_nhterminal_structs
 *  @{
 */

    typedef struct nh_terminal_Terminal {
        nh_tty_TTY *TTY_p;
        NH_BYTE namespace_p[255];
        nh_terminal_Config Config;
        nh_terminal_Grid Grid;
        nh_terminal_Grid Grid2;
        nh_terminal_Graphics Graphics;
        nh_terminal_Graphics Graphics2;
        nh_tty_View *View_p;
        NH_BOOL ctrl;
        NH_BOOL leftMouse;
        nh_SystemTime LastClick;
    } nh_terminal_Terminal;

/** @} */

/** @addtogroup lib_nhterminal_typedefs
 *  @{
 */

    typedef nh_terminal_Terminal *(*nh_terminal_openTerminal_f)(
        NH_BYTE *config_p, nh_tty_TTY *TTY_p
    ); 

    typedef NH_TERMINAL_RESULT (*nh_terminal_cmd_setViewport_f)(
        nh_terminal_Terminal *Terminal_p, nh_gfx_Viewport *Viewport_p
    ); 

/** @} */

#endif 
