#include <ctype.h>
#include <stdbool.h>
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
