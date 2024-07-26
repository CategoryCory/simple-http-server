#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "requests/requests.h"
#include "file_utilities/file_utilities.h"
#include "server/server.h"
#include "string_utilities/string_utilities.h"

void print_ht_entry(gpointer key, gpointer value, gpointer user_data)
{
    char *ud = (char *)user_data;
    if (ud == NULL)
    {
        printf("%s: %s\n", (char *)key, (char *)value);
    }
    else
    {
        printf("%s: %s (%s)\n", (char *)key, (char *)value, (char *)user_data);
    }
}

int main(void)
{
    // g_start_server();
    // const char *fn = "../data/sample_header.txt";
    // TextFile text_file;

    // if (read_text_file(fn, &text_file) == 0)
    // {
    char *request = "GET /blog/posts/1234 HTTP/1.1\r\n"
                    "Host: www.sample.com\r\n"
                    "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0\r\n"
                    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                    "Accept-Language: en-US,en;q=0.5\r\n"
                    "Accept-Encoding: gzip, deflate\r\n"
                    "Connection: keep-alive\r\n"
                    "Upgrade-Insecure-Requests: 1\r\n"
                    "Content-Type: text/html; charset=UTF-8\r\n"
                    "Content-Length: 345\r\n"
                    "\r\n"
                    "{\"field1\": \"value1\", \"field2\": \"value2\"}";


    HttpRequestDetails *details = init_http_details();
    if (parse_request(request, details) == 0)
    {
        printf("HTTP method: %s\n", details->request_method);
        printf("Path: %s\n", details->path);
        printf("Version: %d.%d\n", details->version.ver_major, details->version.ver_minor);

        g_hash_table_foreach(details->headers, print_ht_entry, NULL);
    }
    
    free_http_details(details);
        // free_text_file(&text_file);
    // }
    return EXIT_SUCCESS;
}