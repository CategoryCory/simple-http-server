#include <stdio.h>
#include <stdlib.h>
#include "requests/requests.h"
#include "file_utilities/file_utilities.h"

int main(void)
{
    const char *fn = "../data/sample_header.txt";
    TextFile text_file;

    if (read_text_file(fn, &text_file) == 0)
    {
        HttpRequestDetails details;
        if (parse_request(text_file.lines[0], &details) == 0)
        {
            printf("HTTP method: %s\n", details.request_type);
            printf("Path: %s\n", details.path);
            printf("Version: %d.%d\n", details.version.ver_major, details.version.ver_minor);
        }

        free_text_file(&text_file);
    }
    return EXIT_SUCCESS;
}