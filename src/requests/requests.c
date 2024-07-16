#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "requests.h"
#include "string_utilities.h"

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

int parse_request(char **request, const size_t request_length, HttpRequestDetails *details)
{
    // Initialize memory in HttpRequestDetails struct
    memset(details, 0, sizeof(HttpRequestDetails));

    // Get the HTTP verb
    char *req_line_token = strtok(request[0], " ");

    if (!is_valid_http_verb(req_line_token))
    {
        fprintf(stderr, "Invalid HTTP verb\n");
        return EXIT_FAILURE;
    }

    strncpy(details->request_type, req_line_token, REQUEST_TYPE_MAX_SIZE - 1);

    // Get URL path
    req_line_token = strtok(NULL, " ");
    strncpy(details->path, req_line_token, REQUEST_PATH_MAX_SIZE - 1);

    // Get HTTP version
    req_line_token = strtok(NULL, " ");

    if (!starts_with(req_line_token, "HTTP/"))
    {
        fprintf(stderr, "Invalid HTTP header\n");
        return EXIT_FAILURE;
    }

    char version[8];
    const uint8_t start_pos = 5;                            // Start after 'HTTP/'
    strcpy(version, req_line_token + start_pos);

    const char *maj_version = strtok(version, ".");
    const char *min_version = strtok(NULL, ".");

    details->version = (HttpVersion) {
        .ver_major = atoi(maj_version),
        .ver_minor = atoi(min_version)
    };
    
    // Get HTTP headers
    details->headers = g_hash_table_new_full(g_str_hash, 
                                             g_str_equal, 
                                             g_free, 
                                             g_free);       // Initialize new hash table
    uint8_t current_line = 1;                               // Start at line 1; skip request line (line 0)
    uint8_t max_header_tokens = 2;                          // Split only on first colon
    char **header_tokens = NULL;

    while (current_line < request_length && !is_empty(request[current_line]))
    {
        if (header_tokens != NULL) g_strfreev(header_tokens);

        header_tokens = g_strsplit(request[current_line], ":", max_header_tokens);

        if (header_tokens[0] == NULL || header_tokens[1] == NULL)
        {
            fprintf(stderr, "An error occurred while processing headers\n");

            // TODO: Do we need to free anything here?

            return EXIT_FAILURE;
        }

        char *k = g_strstrip(header_tokens[0]);
        char *v = g_strstrip(header_tokens[1]);

        g_hash_table_insert(details->headers, g_strdup(k), g_strdup(v));

        current_line++;
    }

    if (header_tokens != NULL) g_strfreev(header_tokens);

    return EXIT_SUCCESS;
}

void free_request(HttpRequestDetails *details)
{
    g_hash_table_destroy(details->headers);
}
