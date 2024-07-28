#ifndef FILE_UTILITIES_H
#define FILE_UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define INITIAL_LINES   10
#define LINE_LENGTH     1024

/**
 * @brief A struct to hold the contents of a text file
 * 
 */
typedef struct
{
    char **lines; ///< An array of strings, each representing a line in the text file.
    size_t line_count; ///< The number of lines in the text file.
} TextFile;

/**
 * @brief Reads the contents of a text file into a TextFile struct.
 * 
 * This function reads the contents of the specified text file and stores each line as
 * a string in a dynamically allocated array within the provided TextFile struct.
 * 
 * @param file_name A string representing the name of the text file to be read.
 * @param text_file A pointer to the TextFile struct to populate.
 * @return int Zero on success, non-zero otherwise
 */
int read_text_file(const char *file_name, TextFile *text_file);

/**
 * @brief Frees the memory associated with a TextFile struct.
 * 
 * This function frees the memory allocated for each line in the TextFile struct as
 * well as the memory allocated for the array of lines.
 * 
 * @param text_file A pointer to the TextFile struct to destroy.
 */
void free_text_file(TextFile *text_file);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // FILE_UTILITIES_H