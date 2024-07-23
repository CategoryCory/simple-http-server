#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <glib.h>
#include <gio/gio.h>
#include "server.h"
#include "requests.h"
#include "file_utilities.h"

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

static bool on_incoming_connection(__attribute__((unused)) GSocketService *service,
                                   GSocketConnection *connection,
                                   __attribute__((unused)) GObject *source_object,
                                   __attribute__((unused)) gpointer user_data)
{
    GInputStream *istream;
    GOutputStream *ostream;
    char buffer[1024];
    gssize size;

    istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));

    size = g_input_stream_read(istream, buffer, sizeof(buffer) - 1, NULL, NULL);
    if (size > 0)
    {
        buffer[size] = '\0';
        g_print("Received request:\n%s\n", buffer);

        const char *response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/plain\r\n"
                               "Content-Length: 13\r\n"
                               "\r\n"
                               "Hello, world!";
        
        g_output_stream_write(ostream, response, strlen(response), NULL, NULL);
    }

    g_object_unref(connection);
    return true;
}

int g_start_server(void)
{
    GSocketService *service;
    GError *error = NULL;
    GInetAddress *inet_address;
    GSocketAddress *socket_address;

    // g_type_init();

    service = g_socket_service_new();
    g_signal_connect(service, "incoming", G_CALLBACK(on_incoming_connection), NULL);

    inet_address = g_inet_address_new_any(G_SOCKET_FAMILY_IPV4);
    socket_address = g_inet_socket_address_new(inet_address, 9999);

    if (!g_socket_listener_add_address(G_SOCKET_LISTENER(service),      // GSocketListener *listener
                                       socket_address,                  // GSocketAddress *address
                                       G_SOCKET_TYPE_STREAM,            // GSocketType type
                                       G_SOCKET_PROTOCOL_TCP,           // GSocketProtocol protocol
                                       G_OBJECT(NULL),                  // GObject *source_object
                                       NULL,                            // GSocketAddress **effective_address
                                       &error))
    {
        g_printerr("Error: %s\n", error->message);
        g_clear_error(&error);
        return EXIT_FAILURE;
    }

    g_object_unref(socket_address);
    g_object_unref(inet_address);

    g_socket_service_start(service);
    g_print("HTTP server is listening on port 9999\n");

    GMainLoop *loop = g_main_loop_new(NULL, false);
    g_main_loop_run(loop);

    g_object_unref(service);
    return EXIT_SUCCESS;
}

void start_server(void)
{
    char *ip = "127.0.0.1";
    in_port_t port = 9999;

    int server_sock, client_sock, n;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[8192];

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
        HttpRequestDetails* details = init_http_details();

        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        printf("Client connected.\n");

        bzero(buffer, 1024);
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        parse_request(buffer, details);

        printf("********************************\n\n");
        printf("HTTP method: %s\n", details->request_method);
        printf("Path: %s\n", details->path);
        printf("Version: %d.%d\n", details->version.ver_major, details->version.ver_minor);

        char *conn = g_hash_table_lookup(details->headers, "Connection");
        printf("Connection: %s\n", conn);
        printf("********************************\n\n");

        bzero(buffer, 1024);
        strncpy(buffer, "SERVER ACK", 1024);
        printf("Server: %s\n", buffer);
        send(client_sock, buffer, strlen(buffer), 0);

        close(client_sock);
        printf("Client disconnected.");

        free_http_details(details);
    }
}