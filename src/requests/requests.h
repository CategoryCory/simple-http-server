#ifndef REQUESTS_H
#define REQUESTS_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <glib.h>
#include <stdint.h>

#define REQUEST_TYPE_MAX_SIZE   16
#define REQUEST_PATH_MAX_SIZE   1024

/**
 * @brief A struct representing an HTTP version number.
 * 
 */
typedef struct
{
    uint8_t ver_major;  ///< HTTP major version number as an int.
    uint8_t ver_minor;  ///< HTTP minor version number as an int.
} HttpVersion;

/**
 * @brief A struct representing the details of an HTTP request.
 * 
 */
typedef struct
{
    char request_type[REQUEST_TYPE_MAX_SIZE];   ///< HTTP request method as a string.
    char path[REQUEST_PATH_MAX_SIZE];           ///< HTTP request path as a string.
    HttpVersion version;                        ///< HTTP version as HttpVersion.
    GHashTable *headers;                        ///< HTTP request headers as a GHashTable.
} HttpRequestDetails;

/**
 * @brief Parses an HTTP request.
 * 
 * Given a string representing an HTTP request, parses the string as an HTTP request and
 * populates the provided HttpRequestDetails object.
 * 
 * @param request The request to parse as an array of strings.
 * @param request_length The number of elements in @p request.
 * @param details A pointer to the HttpRequestDetails struct to populate.
 * @return Zero if parse was successful, non-zero otherwise.
 */
int parse_request(char **request, const size_t request_length, HttpRequestDetails *details);

/**
 * @brief Frees an HTTP request details object.
 * 
 * This function frees all memory associated with a given HttpRequestDetails struct.
 * 
 * @param details A pointer to the HttpRequestDetails struct to be freed.
 */
void free_request(HttpRequestDetails *details);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // REQUESTS_H