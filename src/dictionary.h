#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>

#define INITIAL_CAPACITY    16

typedef struct
{
    const char *key;
    void *value;
} dict_entry;

typedef struct 
{
    dict_entry *entries;
    size_t capacity;
    size_t length;
} dictionary;

dictionary *dict_create(void);
void dict_destroy(dictionary *dict);

#endif // DICTIONARY_H