#ifndef NH_MONITOR_CLIENT_H
#define NH_MONITOR_CLIENT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Common/Includes.h"

int nh_monitor_connectToMonitor(
    int port 
);

NH_API_RESULT nh_monitor_checkIfMonitorConnectionEstablished(
    int client_fd 
);

NH_API_RESULT nh_monitor_disconnectFromMonitor(
    int client_fd
);

int nh_monitor_sendMessageToMonitor(
    int client_socket, char *message_p, int length
);

int nh_monitor_waitForMonitorAck(
    int client_fd
);

#endif
