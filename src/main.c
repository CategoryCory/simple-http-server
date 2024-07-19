#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "dictionary/dictionary.h"
#include "requests/requests.h"
#include "file_utilities/file_utilities.h"
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
    const char *fn = "../data/sample_header.txt";
    TextFile text_file;

    if (read_text_file(fn, &text_file) == 0)
    {
        HttpRequestDetails *details = init_http_details();
        if (parse_request(text_file.lines, text_file.line_count, details) == 0)
        {
            printf("HTTP method: %s\n", details->request_type);
            printf("Path: %s\n", details->path);
            printf("Version: %d.%d\n", details->version.ver_major, details->version.ver_minor);

            g_hash_table_foreach(details->headers, print_ht_entry, NULL);
        }
        
        free_http_details(details);
        free_text_file(&text_file);
    }
    return EXIT_SUCCESS;
}