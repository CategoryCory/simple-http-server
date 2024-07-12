#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utilities.h"

int read_text_file(const char *file_name, TextFile *text_file)
{
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    size_t capacity = INITIAL_LINES;
    char **lines = malloc(capacity * sizeof(char *));
    if (lines == NULL)
    {
        perror("Failed to allocate memory");
        fclose(f);
        return EXIT_FAILURE;
    }

    char buffer[LINE_LENGTH];
    size_t line_count = 0;

    while (fgets(buffer, LINE_LENGTH, f))
    {
        // Resize if necessary
        if (line_count > capacity)
        {
            capacity *= 2;
            char **temp = realloc(lines, capacity * sizeof(char *));
            if (temp == NULL)
            {
                perror("Failed to reallocate memory");
                for (size_t i = 0; i < line_count; i++)
                {
                    free(lines[i]);
                }
                free(lines);
                fclose(f);
                return EXIT_FAILURE;
            }
            lines = temp;
        }

        lines[line_count] = strdup(buffer);
        if (lines[line_count] == NULL)
        {
            perror("Failed to allocate memory for line");
            for (size_t i = 0; i < line_count; i++)
            {
                free(lines[i]);
            }
            free(lines);
            fclose(f);
            return EXIT_FAILURE;
        }
        line_count++;
    }

    fclose(f);

    text_file->lines = lines;
    text_file->line_count = line_count;

    return EXIT_SUCCESS;
}

void free_text_file(TextFile *text_file)
{
    if (text_file != NULL)
    {
        for (size_t i = 0; i < text_file->line_count; i++)
        {
            free(text_file->lines[i]);
        }
        free(text_file->lines);
    }
}
