#ifndef DICTIONARY_H
#define DICTIONARY_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdbool.h>
#include <stdlib.h>

#define INITIAL_CAPACITY    16
#define FNV_OFFSET          14695981039346656037UL
#define FNV_PRIME           1099511628211UL

/**
 * @brief A struct representing a key/value pair in the dictionary.
 * 
 */
typedef struct
{
    const char *key;    ///< Key as a string.
    void *value;        ///< Value as a void pointer.
} DictionaryEntry;

/**
 * @brief Struct representing a dictionary.
 * 
 */
typedef struct 
{
    DictionaryEntry *entries;   ///< Array of dictionary items.
    size_t capacity;            ///< Maximum capacity of the dictionary.
    size_t length;              ///< Current number of items in the dictionary.
} Dictionary;

/**
 * @brief A struct representing an iterator over a dictionary.
 * 
 */
typedef struct
{
    const char *key;        ///< The current key.
    void *value;            ///< The current value.
    Dictionary *_dict;      ///< The dictionary for the current iterator.
    size_t _index;          ///< The current index of the iterator.
} DictionaryIterator;

/**
 * @brief Initialize a new dictionary.
 * 
 * Initializes a new instance of a Dictionary struct.
 * 
 * @param dict A pointer to the dictionary to initialize.
 * @return Zero if initialization succeeded, non-zero otherwise.
 */
int dict_init(Dictionary *dict);

/**
 * @brief Destroys a dictionary instance.
 * 
 * Destroys a dictionary instance and frees all associated memory.
 * 
 * @param dict A pointer to the dictionary to destroy.
 */
void dict_destroy(Dictionary *dict);

/**
 * @brief Retrieves value associated with key.
 * 
 * Retrieves a value from the dictionary by looking up the provided key.
 * 
 * @param dict A pointer to the dictionary.
 * @param key The key as a string.
 * @return A pointer to the value if the key is found, NULL otherwise.
 */
void *dict_get(Dictionary *dict, const char *key);

/**
 * @brief Add a new key/value pair to the dictionary.
 * 
 * Adds the provided key and value pair to the associated Dictionary.
 * 
 * @param dict A pointer to the dictionary.
 * @param key The key to add, as a string.
 * @param value The value to add, as a void pointer.
 * @return The newly added key, as a string.
 */
const char *dict_add_kv(Dictionary *dict, const char *key, void *value);

/**
 * @brief Add a new DictionaryEntry to the dictionary.
 * 
 * Adds the provided DictionaryEntry object to the associated Dictionary.
 * 
 * @param dict A pointer to the dictionary.
 * @param entry A pointer to the DictionaryEntry to add.
 * @return The newly added key, as a string.
 */
const char *dict_add_entry(Dictionary *dict, DictionaryEntry *entry);

/**
 * @brief Checks if the dictionary contains a specified key.
 * 
 * Searches for a particular key in the associated dictionary.
 * 
 * @param dict A pointer to the dictionary.
 * @param key The key to search for, as a string.
 * @return True if the key was found, false otherwise.
 */
bool dict_contains_key(Dictionary *dict, const char *key);

/**
 * @brief Returns the current size of the dictionary.
 * 
 * Retrieves the current number of elements stored in the provided Dictionary object.
 * 
 * @param dict A pointer to the dictionary.
 * @return The current size of the dictionary.
 */
size_t dict_length(Dictionary *dict);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // DICTIONARY_H