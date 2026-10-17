#ifndef NH_API_NH_NETWORK_H
#define NH_API_NH_NETWORK_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include <stddef.h>

// TYPEDEFS ========================================================================================

/**
 * Streams response body bytes to the caller. Return the number of bytes consumed;
 * returning less than size aborts the request.
 */
typedef size_t (*nh_api_HTTPWrite_f)(
    void *user_p, const char *data_p, size_t size
);

/**
 * Streams request body bytes from the caller. Return the number of bytes written to buffer_p,
 * 0 at end of body, or (size_t)-1 to abort the request.
 */
typedef size_t (*nh_api_HTTPRead_f)(
    void *user_p, char *buffer_p, size_t size
);

// STRUCTS =========================================================================================

typedef struct nh_api_HTTPRequest {
    const char *method_p;          // "GET", "POST", ... (default "GET")
    const char *url_p;             // http:// or https:// URL
    const char **headers_pp;       // "Name: value" lines without CRLF
    int headerCount;
    const char *body_p;            // optional in-memory body
    size_t bodySize;               // body size (also required when streaming via read_f)
    nh_api_HTTPRead_f read_f;      // optional streamed body, used when body_p is NULL
    void *readUser_p;
    nh_api_HTTPWrite_f write_f;    // optional streamed response body
    void *writeUser_p;
    int timeoutSeconds;            // per I/O operation, 0 means 30 seconds
} nh_api_HTTPRequest;

typedef struct nh_api_HTTPResponse {
    int status;                    // HTTP status code, 0 if no response was received
    char *headers_p;               // raw header block (NUL-terminated), owned by response
    char *body_p;                  // buffered body (NUL-terminated) unless write_f was used
    size_t bodySize;
    char error_p[256];             // transport error description, empty on success
} nh_api_HTTPResponse;

// FUNCTIONS =======================================================================================

/**
 * Performs a blocking HTTP/1.1 request. HTTPS connections verify the peer certificate
 * and host name. Thread-safe. Returns 0 if a response was received (check status),
 * non-zero on transport errors (see error_p).
 */
int nh_api_performHTTPRequest(
    const nh_api_HTTPRequest *Request_p, nh_api_HTTPResponse *Response_p
);

void nh_api_freeHTTPResponse(
    nh_api_HTTPResponse *Response_p
);

/**
 * Copies the value of the first response header with the given case-insensitive name.
 * Returns 1 if found.
 */
int nh_api_getHTTPHeader(
    const nh_api_HTTPResponse *Response_p, const char *name_p, char *value_p, size_t size
);

/**
 * Opens a TCP listener on 127.0.0.1 with an ephemeral port. Returns the socket or -1.
 */
int nh_api_openLoopbackListener(
    int *port_p
);

/**
 * Waits for one HTTP request on the listener, copies its request target (e.g. "/cb?code=..")
 * into target_p, answers it with responseHtml_p and closes the connection.
 * Returns 1 on success, 0 on timeout, -1 on error.
 */
int nh_api_acceptLoopbackRequest(
    int listener, int timeoutSeconds, char *target_p, size_t targetSize, const char *responseHtml_p
);

void nh_api_closeLoopbackListener(
    int listener
);

#endif // NH_API_NH_NETWORK_H
