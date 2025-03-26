#ifndef NH_MONITOR_SERVER_H
#define NH_MONITOR_SERVER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

int nh_monitor_createMonitorSocket(
    int port
);

int nh_monitor_acceptLogger(
    int server_fd
); 

int nh_monitor_receiveMessageFromLogger(
    int client_socket, char *buffer_p, int length, int modulo
);

void nh_monitor_closeMonitorSockets(
    int client_socket, int server_fd
);

int nh_monitor_sendLoggerMessageAck(
    int client_fd
); 

#endif // NH_MONITOR_SERVER_H
