#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdbool.h>

/**
 * @brief Checks if the beginning of a string matches a given target.
 * 
 * Verifies whether a given string has the same beginning as a provided target string. This
 * function attemps to match a number of characters equal to the length of @p target.
 * 
 * @param str The string to search.
 * @param target The string to compare the beginning of @p str to.
 * @return True if @p str begins with @p target, false otherwise.
 */
bool starts_with(const char *str, const char *target);

/**
 * @brief Checks if a string is empty.
 * 
 * Verifies whether a given string is empty or contains only whitespace characters.
 * 
 * @param str The string to check.
 * @return True if @p str is empty or whitespace, false otherwise.
 */
bool is_empty(const char *str);

/**
 * @brief Splits a string on new lines.
 * 
 * Implements a platform-independent method for splitting a string containing newline
 * characters into an array of lines. Returned array is freed with `free_lines`.
 * 
 * @param str The string to split.
 * @param num_lines A pointer to a size_t representing the number of lines after splitting.
 * @return The split string as an array of strings. Freed with `free_lines`.
 */
char** str_split_newline(const char *str, size_t *num_lines);

/**
 * @brief Frees string array.
 * 
 * This function frees the char** array returned by `str_split_newline`.
 * 
 * @param lines The string array to be freed.
 * @param num_lines The number of strings in the string array.
 */
void free_lines(char **lines, size_t num_lines);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // STRING_UTILITIES_H