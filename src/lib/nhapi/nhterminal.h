#ifndef NH_TERMINAL_H
#define NH_TERMINAL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhtty.h"
#include "nhgfx.h"

#endif

/** @defgroup api_nhterminal nhterminal
 *  @brief Open terminals for TTY presentation.
 *
 *  This module allows the presentation of TTY logic created by the 
 *  nhtty module without the use of proprietary terminals such as xterm 
 *  or konsole.
 */

/** @addtogroup api_nhterminal
 *  @{
 */

/** @addtogroup api_nhdocs
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_TERMINAL_RESULT
    {
        NH_TERMINAL_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_TERMINAL_ERROR_NULL_POINTER,
        NH_TERMINAL_ERROR_BAD_STATE,
        NH_TERMINAL_ERROR_MEMORY_ALLOCATION,
        NH_TERMINAL_ERROR_TERMINFO_DATA_CANNOT_BE_FOUND,
        NH_TERMINAL_ERROR_UNKNOWN_TERMINAL_TYPE,
        NH_TERMINAL_ERROR_TERMINAL_IS_HARDCOPY,
        NH_TERMINAL_ERROR_UNKNOWN_COMMAND,
        NH_TERMINAL_ERROR_INVALID_ARGUMENT,

    } NH_TERMINAL_RESULT;

// TYPEDEFS ========================================================================================

    typedef struct nh_terminal_Terminal nh_terminal_Terminal;

// FUNCTIONS =======================================================================================

    /**
     * @brief Open a TTY based terminal. 
     *
     * The TTY parameter handles TTY logic while the resulting terminal
     * handles the TTY presentation.
     *
     * @param TTY_p Pointer to TTY. Must not be NULL.
     * @return NULL on failure. A pointer to a newly created terminal on success.
     */
    nh_terminal_Terminal *nh_api_openTerminal(
        NH_BYTE *config_p, nh_tty_TTY *TTY_p
    );

    /**
     * @brief Set the viewport of a terminal.
     *
     * Without having a viewport to render into, the terminal won't know where
     * to present the input received by the associated TTY logic.
     *
     * @param Terminal_p Pointer to terminal. Must not be NULL.
     * @param Viewport_p Pointer to viewport. Must not be NULL.
     * @return @ref NH_TERMINAL_SUCCESS on success.
     */
    NH_TERMINAL_RESULT nh_api_setTerminalViewport(
        nh_terminal_Terminal *Terminal_p, nh_gfx_Viewport *Viewport_p
    );

/** @} */

#endif // NH_TERMINAL_H
