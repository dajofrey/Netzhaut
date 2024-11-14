#ifndef NH_CORE_LOGGER_SERVER_H
#define NH_CORE_LOGGER_SERVER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

int nh_core_createMonitorSocket(
    int port
);

int nh_core_acceptLogger(
    int server_fd
); 

int nh_core_receiveMessageFromLogger(
    int client_socket, char *buffer_p, int length, int modulo
);

void nh_core_closeMonitorSockets(
    int client_socket, int server_fd
);

int nh_core_sendLoggerMessageAck(
    int client_fd
); 

#endif // NH_CORE_LOGGER_SERVER_H
