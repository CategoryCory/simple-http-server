#ifndef DICT_UTILITIES_H
#define DICT_UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "dictionary.h"

/**
 * @brief Converts a CSV file containing two columns to a dictionary.
 * 
 * @param file_name The name of the CSV file as a string.
 * @param dict A pointer to the dictionary to populate.
 * @return 0 if the dictionary was successfully populated, non-zero otherwise.
 */
int csv_to_dict(const char *file_name, Dictionary *dict);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // DICT_UTILITIES_H