#include <stdlib.h>
#include "server/server.h"
#include "http/request_parser.h"

#include <stdio.h>
#include <regex.h>
#include <stdbool.h>
#include <glib.h>

__attribute__((unused)) static bool validate_request_line(const char *request_line)
{
    const char *pattern = "^[A-Z]+\\s+(https?://[^\\s/$.?#].[^\\s]*|/[^\\s]*)\\s+HTTP/[0-9]+(.[0-9]+)?$";
    GError *error = NULL;
    GRegex *regex = g_regex_new(pattern, 0, 0, &error);
    // const char *request_line_pattern = "^[A-Z]+\\s+(https?:\\/\\/[^\\s\\/$.?#].[^\\s]*|\\/[^\\s]*)\\s+HTTP\\/\\d+\\.\\d+$";
    if (error != NULL)
    {
        g_printerr("Error compiling regex: %s\n", error->message);
        g_error_free(error);
        return false;
    }

    bool match = g_regex_match(regex, request_line, 0, NULL);
    g_regex_unref(regex);
    return match;

    // regex_t regex;
    // // const char *request_line_pattern = "^[A-Z]+\\s+(https?:\\/\\/[^\\s\\/$.?#].[^\\s]*|\\/[^\\s]*)\\s+HTTP\\/\\d+\\.\\d+$";
    // const char *pattern = "^[A-Z]+\\s+(https?://[^\\s/$.?#].[^\\s]*|/[^\\s]*)\\s+HTTP/[0-9]+(.[0-9]+)?$";
    // int reti = regcomp(&regex, pattern, REG_EXTENDED);
    // if (reti)
    // {
    //     fprintf(stderr, "Could not compile regex\n");
    //     return HP_SYSTEM_ERROR;     // TODO: What error to use here?
    // }
    // printf("Regex compiled...\n");
    // reti = regexec(&regex, request_line, 0, NULL, 0);
    // printf("reti = %d\n", reti);
    // regfree(&regex);
    // return !reti;
}

int main(void)
{
    // g_start_server();

    char *test_string = "GET /blogs/poth/1234 HTTP/2.0";
    // bool is_valid = validate_request_line(test_string);

    // if (is_valid)
    // {
    //     printf("The line is valid.\n");
    // }
    // else
    // {
    //     printf("The line is NOT valid.\n");
    // }

    printf("Starting...\n");

    // char *request_line = "GET /blog/posts/1234 HTTP/2.0";
    HttpRequestLine *parsed_line = http_request_line_parse(test_string);

    if (parsed_line != NULL)
    {
        printf("Request successfully parsed...\n");
    }

    http_request_line_free(parsed_line);

    printf("Stopping...\n");

    return EXIT_SUCCESS;
}