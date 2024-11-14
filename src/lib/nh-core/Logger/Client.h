#ifndef NH_CORE_LOGGER_CLIENT_H
#define NH_CORE_LOGGER_CLIENT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

int nh_core_connectToMonitor(
    int port 
);

NH_API_RESULT nh_core_checkIfMonitorConnectionEstablished(
    int client_fd 
);

NH_API_RESULT nh_core_disconnectFromMonitor(
    int client_fd
);

int nh_core_sendMessageToMonitor(
    int client_socket, char *message_p, int length
);

int nh_core_waitForMonitorAck(
    int client_fd
);

#endif // NH_CORE_LOGGER_CLIENT_H
