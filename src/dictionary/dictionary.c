#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

Dictionary *dict_create(void)
{
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (dict == NULL) return NULL;

    dict->length = 0;
    dict->capacity = INITIAL_CAPACITY;

    dict->entries = calloc(dict->capacity, sizeof(DictionaryEntry));
    if (dict->entries == NULL)
    {
        free(dict);
        return NULL;
    }

    return dict;
}

void dict_destroy(Dictionary *dict)
{
    for (size_t i = 0; i < dict->capacity; i++)
    {
        free((void *)dict->entries[i].key);
    }

    free(dict->entries);
    free(dict);
}

static uint64_t hash_key(const char *key)
{
    uint64_t hash = FNV_OFFSET;
    for (const char *p = key; *p; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}

static const char *dict_set_entry(DictionaryEntry *entries, size_t capacity, const char *key, void *value, size_t *plength)
{
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    while (entries[index].key != NULL)
    {
        if (strcmp(key, entries[index].key) == 0)
        {
            entries[index].value = value;
            return entries[index].key;
        }

        index++;

        if (index >= capacity)
        {
            index = 0;
        }
    }

    if (plength != NULL)
    {
        key = strdup(key);
        if (key == NULL)
        {
            return NULL;
        }
        (*plength)++;
    }

    entries[index].key = (char*)key;
    entries[index].value = value;
    return key;
}

static bool dict_expand(Dictionary *dict)
{
    size_t new_capacity = dict->capacity * 2;
    if (new_capacity < dict->capacity)
    {
        return false;
    }

    DictionaryEntry *new_entries = calloc(new_capacity, sizeof(DictionaryEntry));
    if (new_entries == NULL) return false;

    for (size_t i = 0; i < dict->capacity; i++)
    {
        DictionaryEntry entry = dict->entries[i];
        if (entry.key != NULL)
        {
            dict_set_entry(new_entries, new_capacity, entry.key, entry.value, NULL);
        }
    }

    free(dict->entries);
    dict->entries = new_entries;
    dict->capacity = new_capacity;
    return true;
}

void *dict_get(Dictionary *dict, const char *key)
{
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(dict->capacity - 1));

    while (dict->entries[index].key != NULL)
    {
        if (strcmp(key, dict->entries[index].key) == 0)
        {
            return dict->entries[index].value;
        }

        index++;

        if (index >= dict->capacity)
        {
            index = 0;
        }
    }

    return NULL;
}

const char *dict_set(Dictionary *dict, const char *key, void *value)
{
    if (value == NULL) return NULL;

    if (dict->length >= dict->capacity / 2)
    {
        if (!dict_expand(dict)) return NULL;
    }

    return dict_set_entry(dict->entries, dict->capacity, key, value, &dict->length);
}

size_t dict_length(Dictionary *dict)
{
    return dict->length;
}

DictionaryIterator dict_iterator(Dictionary *dict)
{
    DictionaryIterator it = { ._dict = dict, ._index = 0 };
    return it;
}

bool dict_next(DictionaryIterator *it)
{
    Dictionary *dict = it->_dict;
    while (it->_index < dict->capacity)
    {
        size_t index = it->_index;
        it->_index++;
        if (dict->entries[index].key != NULL)
        {
            DictionaryEntry entry = dict->entries[index];
            it->key = entry.key;
            it->value = entry.value;
            return true;
        }
    }
    return false;
}
