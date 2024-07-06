#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>

#define INITIAL_CAPACITY    16
#define FNV_OFFSET          14695981039346656037UL
#define FNV_PRIME           1099511628211UL

typedef struct
{
    const char *key;
    void *value;
} DictionaryEntry;

typedef struct 
{
    DictionaryEntry *entries;
    size_t capacity;
    size_t length;
} Dictionary;

typedef struct
{
    const char *key;
    void *value;
    Dictionary *_dict;
    size_t _index;
} DictionaryIterator;

Dictionary *dict_create(void);
void dict_destroy(Dictionary *dict);
void *dict_get(Dictionary *dict, const char *key);
const char *dict_add_kv(Dictionary *dict, const char *key, void *value);
const char *dict_add_entry(Dictionary *dict, DictionaryEntry *entry);
size_t dict_length(Dictionary *dict);

#endif // DICTIONARY_H