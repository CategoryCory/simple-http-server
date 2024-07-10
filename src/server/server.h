#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>

#define REQUEST_TYPE_MAX_SIZE   16
#define REQUEST_PATH_MAX_SIZE   1024

/**
 * @brief A struct representing an HTTP version number.
 * 
 */
typedef struct
{
    uint8_t ver_major;
    uint8_t ver_minor;
} HttpVersion;

/**
 * @brief A struct representing the details of an HTTP request.
 * 
 */
typedef struct
{
    char request_type[REQUEST_TYPE_MAX_SIZE];
    char path[REQUEST_PATH_MAX_SIZE];
    HttpVersion version;
} HttpRequestDetails;


/**
 * @brief Get the file extension of a given file name.
 * 
 * @param f_name The name of the file as a string.
 * @return The file's extension as a string.
 */
const char *get_file_extension(const char *f_name);

/**
 * @brief Get the mime type based on a file's extension.
 * 
 * @param f_ext The file extension as a string.
 * @return The file's associated mime type as a string.
 */
const char *get_mime_type(const char *f_ext);

/**
 * @brief Parses an HTTP request.
 * 
 * @param request The request to parse as a string.
 * @param details A pointer to the HttpRequestDetails struct to populate.
 * @return Zero if parse was successful, non-zero otherwise.
 */
int parse_request(char *request, HttpRequestDetails *details);

/**
 * @brief Starts the HTTP server.
 * 
 */
void start_server(void);

#endif // SERVER_H