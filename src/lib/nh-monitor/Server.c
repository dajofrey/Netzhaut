// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES  =======================================================================================

#include "Server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>

// FUNCTIONS =======================================================================================

int nh_monitor_createMonitorSocket(
    int port)
{
    int server_fd;
    struct sockaddr_in address;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        puts("Socket failed");
        return -1;
    }

    // Set the receive timeout for accept()
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 50000; // 50ms
    if (setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        puts("Bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        puts("Listen failed");
        close(server_fd);
        return -1;
    }

    int buffer_size = 262144;
    int buffer_size_send = 4;

    setsockopt(server_fd, SOL_SOCKET, SO_SNDBUF, &buffer_size_send, sizeof(buffer_size_send));
    setsockopt(server_fd, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size));

    return server_fd;
}

int nh_monitor_acceptLogger(
    int server_fd) 
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        return -1;
    }

    char info_p[255] = {0};
    snprintf(info_p, sizeof(info_p), "Accepted a client connection from %s:%d",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    puts(info_p);

    int flags = fcntl(client_socket, F_GETFL, 0);
    fcntl(client_socket, F_SETFL, flags);

    int buffer_size = 262144;
    int buffer_size_send = 3;

    setsockopt(client_socket, SOL_SOCKET, SO_SNDBUF, &buffer_size_send, sizeof(buffer_size_send));
    setsockopt(client_socket, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size));

    return client_socket;
}

void nh_monitor_closeMonitorSockets(
    int client_socket, int server_fd)
{
    close(client_socket);
    close(server_fd);
}

int nh_monitor_receiveMessageFromLogger(
    int socket, char *buffer, int size, int modulo) 
{
    size_t totalReceived = 0;
    char *buffer_p = (char *)buffer;

    while (totalReceived < size) {
        int bytes_received = recv(socket, buffer_p + totalReceived, size - totalReceived, 0);
        if (bytes_received < 0) {
            return -1; // Error occurred
        } else if (bytes_received == 0) {
            printf("Connection closed\n");
            return 0; // Connection closed
        }
        totalReceived += bytes_received;
        if ((totalReceived % modulo) == 0) {
            return totalReceived;
        }
    }

    if (totalReceived != size) {
        puts("Received data mismatch");
        exit(0);
    }

    return totalReceived;
}

int nh_monitor_sendLoggerMessageAck(
    int client_fd) 
{
    const char *ack_message = "ACK";
    size_t ack_length = strlen(ack_message);
    size_t total_sent = 0;

    while (total_sent < ack_length) {
        ssize_t bytes_sent = send(client_fd, ack_message + total_sent, ack_length - total_sent, 0);
        if (bytes_sent == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                usleep(1000);
                continue;
            } else {
                perror("Error sending ACK");
                return -1;  // Actual error occurred
            }
        }
        total_sent += bytes_sent;
        if (bytes_sent < ack_length && total_sent < ack_length) {
            fprintf(stderr, "Partial ACK sent, retrying to send remaining bytes\n");
        }
    }

    return 0;  // ACK sent successfully
}
