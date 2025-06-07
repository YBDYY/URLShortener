#include "../include/http-server.h"

int server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[3000] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Bind to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // 3. Listen for connections
    listen(server_fd, 10);

    printf("Server running on port %d\n", PORT);

    while (1) {
        // 4. Accept client
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        read(new_socket, buffer, 3000);

        printf("Request:\n%s\n", buffer);  // Show raw HTTP request

        // 5. Send simple HTTP response
        char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<h1>Hello from C server</h1>";
        send(new_socket, response, strlen(response), 0);

        close(new_socket);
    }

    return 0;
}
