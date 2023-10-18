#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // Create a socket
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define the server address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // Receive data from the server
    char server_response[256];
    recv(client_socket, &server_response, sizeof(server_response), 0);
    printf("Received from server: %s\n", server_response);

    // Close the socket
    close(client_socket);

    return 0;
}

