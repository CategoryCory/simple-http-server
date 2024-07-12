#ifndef REQUESTS_H
#define REQUESTS_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

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
 * @brief Parses an HTTP request.
 * 
 * @param request The request to parse as a string.
 * @param details A pointer to the HttpRequestDetails struct to populate.
 * @return Zero if parse was successful, non-zero otherwise.
 */
int parse_request(char *request, HttpRequestDetails *details);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // REQUESTS_H