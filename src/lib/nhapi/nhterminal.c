// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhterminal.h"

#include "../nhcore/Loader/Loader.h"
#include "../nhterminal/Terminal/Terminal.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

nh_terminal_Terminal *nh_api_openTerminal(
    NH_BYTE *config_p, nh_tty_TTY *TTY_p)
{
    nh_terminal_openTerminal_f openTerminal_f = !NH_LOADER_P || !TTY_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TERMINAL, 0, "nh_terminal_openTerminal");
    return openTerminal_f ? openTerminal_f(config_p, TTY_p) : NULL;
}

NH_TERMINAL_RESULT nh_api_setTerminalViewport(
    nh_terminal_Terminal *Terminal_p, nh_gfx_Viewport *Viewport_p)
{
    nh_terminal_cmd_setViewport_f setViewport_f = !NH_LOADER_P || !Terminal_p || !Viewport_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TERMINAL, 0, "nh_terminal_cmd_setViewport");
    return setViewport_f ? setViewport_f(Terminal_p, Viewport_p) : NH_TERMINAL_ERROR_NULL_POINTER;
}

