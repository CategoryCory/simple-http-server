#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

int csv_to_dict(const char *file_name, Dictionary *dict)
{
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    const size_t buff_size = 255;
    char line[buff_size];
    char *key;
    char *value;

    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\n")] = '\0';

        key = strtok(line, ",");
        value = strtok(NULL, ",");

        if (key != NULL && value != NULL)
        {
            char *key_copy = strdup(key);
            char *value_copy = strdup(value);

            if (key_copy == NULL || value_copy == NULL)
            {
                perror("Failed to allocate memory");
                fclose(f);
                return EXIT_FAILURE;
            }
            
            dict_add_kv(dict, key_copy, value_copy);
        }
    }

    fclose(f);
    return EXIT_SUCCESS;
}