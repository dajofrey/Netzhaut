#ifndef NH_TTY_SHELL_SOCKET_H
#define NH_TTY_SHELL_SOCKET_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stddef.h>
#include <unistd.h>
#include <sys/un.h>

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_ShellSocket {
        struct sockaddr_un address;
        int fd;
        uint16_t port;
    } nh_tty_ShellSocket;

/** @} */

/** @addtogroup lib_nhtty_typedefs
 *  @{
 */

    typedef NH_TTY_RESULT (*nh_tty_sendCommandToShell_f)(
        int pid, NH_TTY_PROGRAM_E type 
    );

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    NH_TTY_RESULT nh_tty_createShellSocket(
        nh_tty_ShellSocket *Socket_p, int pid
    );

    void nh_tty_closeShellSocket(
        nh_tty_ShellSocket *Socket_p, int pid
    );

    NH_TTY_RESULT nh_tty_handleShellSocket(
        nh_tty_ShellSocket *Socket_p
    );

/** @} */

#endif 
