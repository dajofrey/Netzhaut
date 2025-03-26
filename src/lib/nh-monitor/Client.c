// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Client.h"
#include "Monitor.h"
#include "Common/Config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

// FUNCTIONS =======================================================================================

int nh_monitor_connectToMonitor(
    int port)
{
    int client_fd = 0;
    struct sockaddr_in server_address;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Socket creation failed");
        return -1;
    }

    memset(&server_address, 0, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        if (errno != EINPROGRESS) { // EINPROGRESS means connection is in progress
            puts("Connection failed immediately");
            close(client_fd);
            return -1;
        }
    }

    int buffer_size = 262144; // 1 MB
    int buffer_size_receive = 3; // 1mb

    setsockopt(client_fd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size));
    setsockopt(client_fd, SOL_SOCKET, SO_RCVBUF, &buffer_size_receive, sizeof(buffer_size_receive));

    return client_fd; // Connection initiated, not yet established
}

NH_API_RESULT nh_monitor_checkIfMonitorConnectionEstablished(
    int client_fd) 
{
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(client_fd, &write_fds);

    int result = select(client_fd + 1, NULL, &write_fds, NULL, &timeout);
    if (result < 0) {
        puts("select failed");
        return NH_API_ERROR_BAD_STATE;
    } else if (result == 0) {
        puts("Connection timed out");
        return NH_API_ERROR_BAD_STATE;
    } else if (FD_ISSET(client_fd, &write_fds)) {
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(client_fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
            puts("getsockopt failed");
            return NH_API_ERROR_BAD_STATE;
        }
        if (error != 0) {
            char error_p[255];
            sprintf(error_p, "Connection failed: %s", strerror(error));
            puts(error_p);
            return NH_API_ERROR_BAD_STATE;
        }
        puts("Connection established successfully!");
        return NH_API_SUCCESS;
    }

    return NH_API_ERROR_BAD_STATE;  // Default return if something unexpected occurs
}

NH_API_RESULT nh_monitor_disconnectFromMonitor(
    int client_fd)
{
    close(client_fd);
    return 0;
}

int nh_monitor_sendMessageToMonitor(
    int client_socket, char *message_p, int length) 
{
    int result = send(client_socket, message_p, length, 0);
    if (result == -1) {
        printf("send() failed with error %d: %s\n", errno, strerror(errno));
    }
    return result;
}

int nh_monitor_waitForMonitorAck(
    int client_fd) 
{
    char ack_buffer[4] = {0}; // Assuming "ACK" as the acknowledgment message
    int bytes_received = 0;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 5000; // 5 ms

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_fd, &read_fds);

    while (1) {
        int select_result = select(client_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (select_result > 0 && FD_ISSET(client_fd, &read_fds)) {
            bytes_received = recv(client_fd, ack_buffer, sizeof(ack_buffer) - 1, 0);
            if (bytes_received > 0) {
                ack_buffer[bytes_received] = '\0'; // Null-terminate the string
                if (strcmp(ack_buffer, "ACK") == 0) {
                    return 0; // ACK received successfully
                } else {
                    fprintf(stderr, "Unexpected message received: %s\n", ack_buffer);
                    return -1; // Unexpected message received
                }
            } else if (bytes_received == 0) {
                fprintf(stderr, "Connection closed by monitor\n");
                return -1;
            } else if (errno != EWOULDBLOCK && errno != EAGAIN) {
                perror("Error receiving ACK");
                return -1;
            }
        } else if (select_result == 0) {
            // Timeout occurred
            return -1;
        } else {
            return -1;
        }
    }
}
