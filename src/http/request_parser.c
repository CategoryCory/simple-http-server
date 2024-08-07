#include <glib.h>
#include <regex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request_parser.h"
#include "string_utilities.h"

const char *http_methods[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};

static bool is_valid_http_method(const char *v)
{
    size_t num_methods = sizeof(http_methods) / sizeof(http_methods[0]);

    for (size_t i = 0; i < num_methods; i++)
    {
        if (strcmp(v, http_methods[i]) == 0) return true;
    }

    return false;
}

HttpRequestDetails* init_http_details(void)
{
    HttpRequestDetails *new_details = calloc(1, sizeof(HttpRequestDetails));

    if (new_details == NULL)
    {
        perror("Failed to allocate memory for HttpRequestDetails");
        return NULL;
    }

    new_details->request_method = NULL;
    new_details->path = NULL;
    new_details->headers = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    return new_details;
}

void free_http_details(HttpRequestDetails *details)
{
    if (details != NULL)
    {
        free(details->request_method);
        free(details->path);
        g_hash_table_destroy(details->headers);
        free(details);
    }
}

HttpParserCodes parse_request(char *request, HttpRequestDetails *details)
{
    // Split request on newline characters
    size_t request_length;
    char **request_lines = str_split_newline(request, &request_length);

    // A request must contain at minimum a request line and Host header
    if (request_length < 2)
    {
        fprintf(stderr, "Invalid HTTP header");
        return HP_MISSING_HEADER;
    }

    // Get the HTTP method
    char *req_line_token = strtok(request_lines[0], " ");

    if (!is_valid_http_method(req_line_token))
    {
        fprintf(stderr, "Invalid HTTP method\n");
        return HP_INVALID_METHOD;
    }

    size_t method_length = strlen(req_line_token) + 1;

    details->request_method = malloc(method_length);
    if (details->request_method == NULL)
    {
        fprintf(stderr, "Error allocating memory\n");
        return HP_MEMORY_ERROR;
    }
    strcpy(details->request_method, req_line_token);

    // Get URL path
    req_line_token = strtok(NULL, " ");
    size_t path_length = strlen(req_line_token) + 1;

    details->path = malloc(path_length);
    if (details->path == NULL)
    {
        fprintf(stderr, "Error allocating memory\n");
        return HP_MEMORY_ERROR;
    }
    strcpy(details->path, req_line_token);

    // Get HTTP version
    req_line_token = strtok(NULL, " ");

    if (!starts_with(req_line_token, "HTTP/"))
    {
        fprintf(stderr, "Invalid HTTP header\n");
        return HP_MISSING_HEADER;
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
    size_t current_line = 1;                                // Start at line 1; skip request line (line 0)
    uint8_t max_header_tokens = 2;                          // Split only on first colon
    char **header_tokens = NULL;

    while (current_line < request_length && !is_empty(request_lines[current_line]))
    {
        if (header_tokens != NULL) g_strfreev(header_tokens);

        header_tokens = g_strsplit(request_lines[current_line], ":", max_header_tokens);

        if (header_tokens[0] == NULL || header_tokens[1] == NULL)
        {
            fprintf(stderr, "An error occurred while processing headers\n");

            // TODO: Do we need to free anything here?

            return HP_SYSTEM_ERROR;
        }

        char *k = g_strstrip(header_tokens[0]);
        char *v = g_strstrip(header_tokens[1]);

        g_hash_table_insert(details->headers, g_strdup(k), g_strdup(v));

        current_line++;
    }

    // Host header is required
    if (!g_hash_table_contains(details->headers, "Host"))
    {
        fprintf(stderr, "Missing HOST header");
        return HP_MISSING_HEADER;
    }

    // Cleanup
    if (request_lines != NULL) free_lines(request_lines, request_length);
    if (header_tokens != NULL) g_strfreev(header_tokens);

    return HP_SUCCESS;
}

///////////////////////////////////////////////////////////////////

static bool validate_request_line(const char *request_line)
{
    const char *request_line_pattern = "^[A-Z]+\\s+(https?:\\/\\/[^\\s\\/$.?#].[^\\s]*|\\/[^\\s]*)\\s+HTTP/[0-9]+(.[0-9]+)?$";
    GError *error = NULL;
    GRegex *regex = g_regex_new(request_line_pattern, 0, 0, &error);

    if (error != NULL)
    {
        g_printerr("Error compiling regex: %s\n", error->message);
        g_error_free(error);
        return false;
    }

    bool is_match = g_regex_match(regex, request_line, 0, NULL);

    g_regex_unref(regex);

    return is_match;
}

static GHashTable* create_http_method_map()
{
    GHashTable *method_map = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    g_hash_table_insert(method_map, "GET", GINT_TO_POINTER(HTTP_GET));
    g_hash_table_insert(method_map, "POST", GINT_TO_POINTER(HTTP_POST));
    g_hash_table_insert(method_map, "PUT", GINT_TO_POINTER(HTTP_PUT));
    g_hash_table_insert(method_map, "DELETE", GINT_TO_POINTER(HTTP_DELETE));
    g_hash_table_insert(method_map, "HEAD", GINT_TO_POINTER(HTTP_HEAD));
    g_hash_table_insert(method_map, "OPTIONS", GINT_TO_POINTER(HTTP_OPTIONS));
    g_hash_table_insert(method_map, "PATCH", GINT_TO_POINTER(HTTP_PATCH));
    g_hash_table_insert(method_map, "TRACE", GINT_TO_POINTER(HTTP_TRACE));
    g_hash_table_insert(method_map, "CONNECT", GINT_TO_POINTER(HTTP_CONNECT));

    return method_map;
}

__attribute__((unused)) static GHashTable* create_http_version_map()
{
    GHashTable *version_map = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    g_hash_table_insert(version_map, "HTTP/0.9", GINT_TO_POINTER(HTTP_0_9));
    g_hash_table_insert(version_map, "HTTP/1.0", GINT_TO_POINTER(HTTP_1_0));
    g_hash_table_insert(version_map, "HTTP/1.1", GINT_TO_POINTER(HTTP_1_1));
    g_hash_table_insert(version_map, "HTTP/2.0", GINT_TO_POINTER(HTTP_2_0));
    g_hash_table_insert(version_map, "HTTP/3.0", GINT_TO_POINTER(HTTP_3_0));

    return version_map;
}

static int get_enum_from_string(GHashTable *map, const char *str)
{
    void *value = g_hash_table_lookup(map, str);
    if (value == NULL)
    {
        fprintf(stderr, "Unknown value: %s\n", str);
        exit(EXIT_FAILURE);     // TODO: What to return here?
    }
    return *(int *)value;
}

HttpRequestLine* http_request_line_parse(const char *request_line)
{
    // Create a copy of request_line since strtok() will change the original string
    char *req_line_copy = strdup(request_line);
    if (req_line_copy == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;    // TODO: What to return here?
    }

    // // Validate request line to make sure it is valid
    if (!validate_request_line(req_line_copy))
    {
        // TODO: Error here?
        return NULL;
    }

    printf("Request line is valid; continuing to parse...\n");

    HttpRequestLine *parsed_request_line = malloc(sizeof(HttpRequestLine));     // TODO: memory allocation here
    GHashTable *method_map = create_http_method_map();
    // GHashTable *version_map = create_http_version_map();

    // // Get the HTTP method
    char *req_line_token = strtok(req_line_copy, " ");
    parsed_request_line->method = (HttpMethod) get_enum_from_string(method_map, req_line_token);
    // parsed_request_line->method = HTTP_GET;

    printf("Method parsed...continuing with URL...\n");

    // // Get the HTTP url
    req_line_token = strtok(NULL, " ");
    size_t url_length = strlen(req_line_token) + 1;
    parsed_request_line->url = malloc(url_length);                              // TODO: memory allocation here
    if (parsed_request_line->url == NULL)
    {
        // TODO: What to return here?
        return NULL;
    }
    strcpy(parsed_request_line->url, req_line_token);

    // Get the HTTP version
    req_line_token = strtok(NULL, " ");
    // parsed_request_line->version = (HttpVersionEnum) get_enum_from_string(version_map, req_line_token);
    parsed_request_line->version = HTTP_2_0;

    // Free allocated memory
    g_hash_table_destroy(method_map);
    // g_hash_table_destroy(version_map);

    return parsed_request_line;
}

void http_request_line_free(HttpRequestLine *request_line)
{
    if (request_line != NULL)
    {
        if (request_line->url != NULL)
        {
            free(request_line->url);
        }
        free(request_line);
    }
}
