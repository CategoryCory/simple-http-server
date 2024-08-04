#ifndef REQUESTS_H
#define REQUESTS_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <glib.h>
#include <stdint.h>

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
    char *request_method;       ///< HTTP request method as a string.
    char *path;                 ///< HTTP request path as a string.
    HttpVersion version;        ///< HTTP version as HttpVersion.
    GHashTable *headers;        ///< HTTP request headers as a GHashTable.
} HttpRequestDetails;

/**
 * @brief An enum containing return codes for the HTTP parser.
 * 
 */
typedef enum
{
    HP_SUCCESS,                 ///< Request was parsed successfully.
    HP_INVALID_REQUEST,         ///< Request was invalid.
    HP_INVALID_METHOD,          ///< Request method was invalid or unsupported.
    HP_MISSING_HEADER,          ///< Request was missing a required header.
    HP_MEMORY_ERROR,            ///< An error occurred while allocating memory.
    HP_SYSTEM_ERROR             ///< Other error indicating a failure to parse request.
} HttpParserCodes;

/**
 * @brief Creates a new HttpRequestDetails struct.
 * 
 * Initializes and allocates a new instance of the HttpRequestDetails struct.
 * 
 * @return A pointer to the newly created HttpRequestDetails struct.
 */
HttpRequestDetails* init_http_details(void);

/**
 * @brief Destroys an HttpRequestDetails struct.
 * 
 * Frees all the memory allocated for an HttpRequestDetails struct.
 * 
 * @param details A pointer to the HttpRequestDetails struct to be destroyed.
 */
void free_http_details(HttpRequestDetails *details);

/**
 * @brief Parses an HTTP request.
 * 
 * Given a string representing an HTTP request, parses the string as an HTTP request and
 * populates the provided HttpRequestDetails object.
 * 
 * @param request The request to parse as a string.
 * @param details A pointer to the HttpRequestDetails struct to populate.
 * @return A value from the HttpParserCodes enum indicating status.
 */
HttpParserCodes parse_request(char *request, HttpRequestDetails *details);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // REQUESTS_H