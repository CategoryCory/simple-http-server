#include "dictionary.h"

dictionary *dict_create(void)
{
    dictionary *dict = malloc(sizeof(dictionary));
    if (dict == NULL) return NULL;

    dict->length = 0;
    dict->capacity = INITIAL_CAPACITY;

    dict->entries = calloc(dict->capacity, sizeof(dict_entry));
    if (dict->entries == NULL)
    {
        free(dict);
        return NULL;
    }

    return dict;
}

void dict_destroy(dictionary *dict)
{
    for (size_t i = 0; i < dict->capacity; i++)
    {
        free((void *)dict->entries[i].key);
    }

    free(dict->entries);
    free(dict);
}