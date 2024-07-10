#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"
#include "dictionary.h"

const char *get_file_extension(const char *f_name)
{
    const char *dot = strrchr(f_name, '.');
    if (!dot || dot == f_name) return "";

    return dot + 1;
}

const char *get_mime_type(const char *f_ext)
{
    if (strcasecmp(f_ext, "html") == 0 || strcasecmp(f_ext, "htm") == 0)
    {
        return "text/html";
    }
    else if (strcasecmp(f_ext, "txt") == 0)
    {
        return "text/plain";
    }
    else if (strcasecmp(f_ext, "jpg") == 0 || strcasecmp(f_ext, "jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcasecmp(f_ext, "png") == 0)
    {
        return "image/png";
    }
    else if (strcasecmp(f_ext, "json") == 0)
    {
        return "application/json";
    }
    else
    {
        return "application/octet-stream";
    }
}

const char *start_server(void)
{
    char *ip = "127.0.0.1";
    in_port_t port = 9999;

    int server_sock, client_sock, n;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    printf("TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("Socket bind error");
        exit(EXIT_FAILURE);
    }
    printf("Bind to port %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");

    while (true)
    {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        printf("Client connected.\n");

        bzero(buffer, 1024);
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        bzero(buffer, 1024);
        strncpy(buffer, "SERVER ACK", 1024);
        printf("Server: %s\n", buffer);
        send(client_sock, buffer, strlen(buffer), 0);

        close(client_sock);
        printf("Client disconnected.");
    }
}