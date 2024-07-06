#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

#define INPUT_BUFFER_SIZE   255

int csv_to_dict(const char *file_name, Dictionary *dict)
{
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char line[INPUT_BUFFER_SIZE];
    DictionaryEntry entry;
    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        if (token != NULL)
        {
            strncpy(entry.key, token, sizeof(token));
        }

        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strncpy(entry.value, token, sizeof(token));
        }

        dict_add_entry(dict, &entry);
    }

    fclose(f);
    return EXIT_SUCCESS;
}