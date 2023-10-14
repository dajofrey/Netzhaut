// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Socket.h"

#include "../Common/Macros.h"

#include "../TTY/Program.h"
#include "../TTY/TTY.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/System/Thread.h"
#include "../../nhcore/Util/File.h"

#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>

// SOCKET ==========================================================================================

static void nh_tty_setUDSFilePath(
    NH_BYTE path_p[255], int pid)
{
NH_TTY_BEGIN()

    memset(path_p, 0, 255);
    sprintf(path_p, "/tmp/nhtty_%d.uds", pid); // UNIX Domain Socket file.

NH_TTY_SILENT_END()
}

// https://openbook.rheinwerk-verlag.de/linux_unix_programmierung/Kap11-017.htm
NH_TTY_RESULT nh_tty_createShellSocket(
    nh_tty_ShellSocket *Socket_p, int pid)
{
NH_TTY_BEGIN()

    NH_BYTE path_p[255] = {0};
    nh_tty_setUDSFilePath(path_p, pid);

    if ((Socket_p->fd = socket(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0)) <= 0) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE) 
    }

    unlink(path_p);

    memset(&Socket_p->address, 0, sizeof(struct sockaddr_un));
    Socket_p->address.sun_family = AF_LOCAL;
    strcpy(Socket_p->address.sun_path, path_p);

    if (bind(Socket_p->fd, (struct sockaddr *) &Socket_p->address, sizeof(Socket_p->address)) != 0) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }

    listen(Socket_p->fd, 5);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

void nh_tty_closeShellSocket(
    nh_tty_ShellSocket *Socket_p, int pid)
{
NH_TTY_BEGIN()

    NH_BYTE path_p[255] = {0};
    nh_tty_setUDSFilePath(path_p, pid);

    close(Socket_p->fd);
    unlink(path_p);

NH_TTY_SILENT_END()
}

NH_TTY_RESULT nh_tty_handleShellSocket(
    nh_tty_ShellSocket *Socket_p)
{
NH_TTY_BEGIN()

    socklen_t addrlen = sizeof(struct sockaddr_in);
    int new_socket = accept(Socket_p->fd, (struct sockaddr *)&Socket_p->address, &addrlen);
   
    if (new_socket <= 0) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS) // Nothing to do.
    }

    int buffer = 255;
    NH_BYTE buffer_p[buffer];
    memset(buffer_p, 0, buffer);
    int size = recv(new_socket, buffer_p, buffer-1, 0);
    if (size > 0) {
        NH_TTY_PROGRAM_E type = atoi(buffer_p);
        nh_tty_addProgram(nh_core_getWorkloadArg(), nh_tty_createProgramPrototype(type), NH_TRUE);
    }

    close(new_socket);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

NH_TTY_RESULT nh_tty_sendCommandToShell(
    int pid, NH_TTY_PROGRAM_E type)
{
NH_TTY_BEGIN()

    NH_BYTE path_p[255] = {0};
    nh_tty_setUDSFilePath(path_p, pid);

    if (!nh_fileExistsOnMachine(path_p, NULL)) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)}
 
    int fd;
    if ((fd=socket(PF_LOCAL, SOCK_STREAM, 0)) <= 0) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }

    struct sockaddr_un address;
    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, path_p);

    if (connect(fd, (struct sockaddr *)&address, sizeof(address)) != 0) {
        NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_BAD_STATE)
    }

    NH_BYTE type_p[255] = {0};
    sprintf(type_p, "%d", type);

    send(fd, type_p, strlen(type_p), 0);
    close(fd);

NH_TTY_DIAGNOSTIC_END(NH_TTY_SUCCESS)
}

