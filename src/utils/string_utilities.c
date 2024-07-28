#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_utilities.h"

bool starts_with(const char *str, const char *target)
{
    const size_t target_length = strlen(target);
    return strncmp(str, target, target_length) == 0;
}

bool is_empty(const char *str)
{
    if (str == NULL) return true;

    while (*str != '\0')
    {
        if (!isspace((unsigned char)*str))
        {
            return false;
        }
        str++;
    }

    return true;
}

char** str_split_newline(const char *str, size_t *num_lines)
{
    if (str == NULL)
    {
        *num_lines = 0;
        return NULL;
    }

    // Set an initial capacity
    size_t capacity = 10;
    size_t count = 0;
    char **lines = malloc(capacity * sizeof(char *));
    if (lines == NULL)
    {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    const char *start = str;
    while (*start)
    {
        // Find end of current line
        const char *end = start;
        while (*end && *end != '\n' && *end != '\r') ++end;

        // Allocate memory for current line
        size_t length = end - start;
        lines[count] = malloc(length + 1);
        if (lines[count] == NULL)
        {
            perror("Unable to allocate memory");
            exit(EXIT_FAILURE);
        }
        strncpy(lines[count], start, length);
        lines[count][length] = '\0';
        ++count;

        // Skip newline characters
        start = end;
        while (*start == '\r' || *start == '\n') ++start;

        if (count == capacity)
        {
            capacity *= 2;
            lines = realloc(lines, capacity * sizeof(char *));
            if (lines == NULL)
            {
                perror("Unable to allocate memory");
                exit(EXIT_FAILURE);
            }
        }
    }

    *num_lines = count;
    return lines;
}

void free_lines(char **lines, size_t num_lines)
{
    for (size_t i = 0; i < num_lines; i++) free(lines[i]);

    free(lines);
}
