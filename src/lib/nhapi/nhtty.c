// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhtty.h"
#include "nhencoding.h"

#include "../nhcore/Loader/Loader.h"

#include "../nhtty/TTY/TTY.h"
#include "../nhtty/TTY/StandardIO.h"
#include "../nhtty/TTY/Program.h"
#include "../nhtty/Shell/Socket.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ADD =============================================================================================

nh_tty_TTY *nh_api_openTTY(
    NH_BYTE *config_p)
{
    nh_tty_openTTY_f openTTY_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TTY, 0, "nh_tty_openTTY");
    return openTTY_f ? openTTY_f(config_p) : NULL;
}

NH_TTY_RESULT nh_api_closeTTY(
    nh_tty_TTY *TTY_p)
{
    nh_tty_closeTTY_f closeTTY_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TTY, 0, "nh_tty_closeTTY");
    return closeTTY_f ? closeTTY_f(TTY_p) : NH_TTY_ERROR_BAD_STATE;
}

NH_TTY_RESULT nh_api_addProgram(
    nh_tty_TTY *TTY_p, NH_TTY_PROGRAM_E type, NH_BOOL once)
{
    nh_tty_cmd_addProgram_f addProgram_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TTY, 0, "nh_tty_cmd_addProgram");
    return addProgram_f ? addProgram_f(TTY_p, type, once) : NH_TTY_ERROR_BAD_STATE;
}

NH_TTY_RESULT nh_api_claimStandardIO(
    nh_tty_TTY *TTY_p)
{
    nh_tty_cmd_claimStandardIO_f claimStandardIO_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TTY, 0, "nh_tty_cmd_claimStandardIO");
    return claimStandardIO_f ? claimStandardIO_f(TTY_p) : NH_TTY_ERROR_BAD_STATE;
}

NH_TTY_RESULT nh_api_unclaimStandardIO(
    nh_tty_TTY *TTY_p)
{
    nh_tty_cmd_unclaimStandardIO_f unclaimStandardIO_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TTY, 0, "nh_tty_cmd_unclaimStandardIO");
    return unclaimStandardIO_f ? unclaimStandardIO_f(TTY_p) : NH_TTY_ERROR_BAD_STATE;
}

NH_TTY_RESULT nh_api_sendEvent(
    nh_tty_TTY *TTY_p, nh_wsi_Event Event)
{
    nh_tty_cmd_sendEvent_f sendEvent_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TTY, 0, "nh_tty_cmd_sendEvent");
    return sendEvent_f ? sendEvent_f(TTY_p, Event) : NH_TTY_ERROR_BAD_STATE;
}

NH_TTY_RESULT nh_api_sendCommandToShell(
    int pid, NH_TTY_PROGRAM_E type)
{
    nh_tty_sendCommandToShell_f sendCommandToShell_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_TTY, 0, "nh_tty_sendCommandToShell");
    return sendCommandToShell_f ? sendCommandToShell_f(pid, type) : NH_TTY_ERROR_BAD_STATE;
}

