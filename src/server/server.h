#ifndef SERVER_H
#define SERVER_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdint.h>

/**
 * @brief Get the file extension of a file.
 * 
 * Given a file name, returns the extension of the provided file name.
 * 
 * @param f_name The name of the file as a string.
 * @return The file's extension as a string.
 */
const char *get_file_extension(const char *f_name);

/**
 * @brief Get the mime type of a file.
 * 
 * Given a file extension, retrieves the mime type associated with the provided extension.
 * 
 * @param f_ext The file extension as a string.
 * @return The file's associated mime type as a string.
 */
const char *get_mime_type(const char *f_ext);

/**
 * @brief Starts the HTTP server.
 * 
 */
void start_server(void);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // SERVER_H