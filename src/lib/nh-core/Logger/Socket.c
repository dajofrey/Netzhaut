#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"  // Localhost for testing
#define MESSAGE "Hello from client!"

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_core_connectToLogger(
    nh_core_Monitor *Monitor_p)
{
    int client_fd;
    struct sockaddr_in server_address;

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Define server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IP address from text to binary form and set it
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(client_fd);
        return -1;
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        close(client_fd);
        return -1;
    }
    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);
}

NH_API_RESULT nh_core_disconnectFromLogger(
    nh_core_Monitor *Monitor_p)
{
    // Close the connection
    close(client_fd);
    printf("Connection closed\n");

    return 0;
}
NH_API_RESULT nh_core_receiveMessageFromLogger(
    nh_core_Monitor *Monitor_p, char *message_p)
{
    int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("Receive failed");
    } else if (bytes_received == 0) {
        printf("Server closed the connection.\n");
    } else {
        buffer[bytes_received] = '\0';  // Null-terminate the received string
        printf("Received message from server: %s\n", buffer);
    }
}
