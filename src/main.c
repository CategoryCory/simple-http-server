#include <stdio.h>
#include <stdlib.h>
#include "requests/requests.h"

int main(void)
{
    char req[50] = "GET /blog/posts/1234 HTTP/1.1";
    HttpRequestDetails details;
    if (parse_request(req, &details) == 0)
    {
        printf("HTTP method: %s\n", details.request_type);
        printf("Path: %s\n", details.path);
        printf("Version: %d--%d\n", details.version.ver_major, details.version.ver_minor);
    }
    return EXIT_SUCCESS;
}