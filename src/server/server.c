#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"
#include "dictionary.h"

const char *http_verbs[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};

static bool is_valid_http_verb(const char *v)
{
    size_t num_verbs = sizeof(http_verbs) / sizeof(http_verbs[0]);

    for (size_t i = 0; i < num_verbs; i++)
    {
        if (strcmp(v, http_verbs[i]) == 0) return true;
    }

    return false;
}

static bool starts_with(const char *str, const char *target)
{
    const size_t target_length = strlen(target);
    return strncmp(str, target, target_length) == 0;
}

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

int parse_request(char *request, HttpRequestDetails *details)
{
    memset(details, 0, sizeof(HttpRequestDetails));

    // Get the HTTP verb
    char *token = strtok(request, " ");

    if (!is_valid_http_verb(token))
    {
        fprintf(stderr, "Invalid HTTP verb\n");
        return EXIT_FAILURE;
    }

    strncpy(details->request_type, token, REQUEST_TYPE_MAX_SIZE - 1);

    // Get URL path
    token = strtok(NULL, " ");
    strncpy(details->path, token, REQUEST_PATH_MAX_SIZE - 1);

    // Get HTTP version
    token = strtok(NULL, " ");

    if (!starts_with(token, "HTTP/"))
    {
        fprintf(stderr, "Invalid HTTP header\n");
        return EXIT_FAILURE;
    }

    char version[8];
    const uint8_t start_pos = 5;   // Start after 'HTTP/'
    strcpy(version, token + start_pos);

    const char *token_maj_version = strtok(version, ".");
    const char *token_min_version = strtok(NULL, ".");

    details->version = (HttpVersion) {
        .ver_major = atoi(token_maj_version),
        .ver_minor = atoi(token_min_version)
    };

    return EXIT_SUCCESS;
}

void start_server(void)
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