#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "requests.h"

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
