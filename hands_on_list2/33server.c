#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // Create a socket
    int server_socket, client_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define the server address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // Listen for incoming connections
    listen(server_socket, 5);
    printf("Server is listening for incoming connections...\n");

    // Accept a connection from a client
    client_socket = accept(server_socket, NULL, NULL);
    printf("Connected to a client\n");

    // Send a message to the client
    char server_message[] = "Hello, client!";
    send(client_socket, server_message, sizeof(server_message), 0);

    // Close the socket
    close(server_socket);

    return 0;
}

