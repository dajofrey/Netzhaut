// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HTTP.h"

#include "../nh-core/System/Memory.h"
#include "../nh-core/Util/LinkedList.h"
#include "../nh-core/Util/String.h"

#include "../nh-core/Common/Macros.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include "Common/Macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

// DATA ============================================================================================

const char *requestNames_pp[] = {
    "GET",
    "HEAD",
    "POST",
    "PUT",
    "DELETE",
    "CONNECT",
    "OPTIONS",
    "TRACE",
    "PATCH",
};

const char *headerNames_pp[] = {
    "WWW-Authenticate",
    "Authorization",
    "Proxy-Authenticate",
    "Proxy-Authorization",
    "Age",
    "Cache-Control",
    "Clear-Site-Data",
    "Expires",
    "Pragma",
    "Warning",
    "Accept-CH",
    "Accept-CH-Lifetime",
    "Early-Data",
    "Content-DPR",
    "DPR",
    "Device-Memory",
    "Save-Data",
    "Viewport-Width",
    "Width",
    "Last-Modified",
    "ETag",
    "If-Match",
    "If-None-Match",
    "If-Modified-Since",
    "If-Unmodified-Since",
    "Vary",
    "Connection",
    "Keep-Alive",
    "Accept",
    "Accept-Charset",
    "Accept-Encoding",
    "Accept-Language",
    "Expect",
    "Max-Forwards",
    "Cookie",
    "Set-Cookie",
    "Cookie2",
    "Set-Cookie2",
    "Access-Control-Allow-Origin",
    "Access-Control-Allow-Credentials",
    "Access-Control-Allow-Headers",
    "Access-Control-Allow-Methods",
    "Access-Control-Expose-Headers",
    "Access-Control-Max-Age",
    "Access-Control-Request-Headers",
    "Access-Control-Request-Method",
    "Origin",
    "Timing-Allow-Origin",
    "DNT",
    "Tk",
    "Content-Disposition",
    "Content-Length",
    "Content-Type",
    "Content-Encoding",
    "Content-Language",
    "Content-Location",
    "Forwarded",
    "X-Forwarded-For",
    "X-Forwarded-Host",
    "X-Forwarded-Proto",
    "Via",
    "Location",
    "From",
    "Host",
    "Referer",
    "Referrer-Policy",
    "User-Agent",
    "Allow",
    "Server",
    "Accept-Ranges",
    "Range",
    "If-Range",
    "Content-Range",
    "Cross-Origin-Embedder-Policy",
    "Cross-Origin-Opener-Policy",
    "Cross-Origin-Resource-Policy",
    "Content-Security-Policy",
    "Content-Security-Policy-Report-Only",
    "Expect-CT",
    "Feature-Policy",
    "Strict-Transport-Security",
    "Upgrade-Insecure-Requests",
    "X-Content-Type-Options",
    "X-Download-Options",
    "X-Frame-Options",
    "X-Permitted-Cross-Domain-Policies",
    "X-Powered-By",
    "X-XSS-Protection",
    "Public-Key-Pins",
    "Public-Key-Pins-Report-Only",
    "Sec-Fetch-Site",
    "Sec-Fetch-Mode",
    "Sec-Fetch-User",
    "Sec-Fetch-Dest",
    "Last-Event-ID",
    "NEL",
    "Ping-From",
    "Ping-To",
    "Report-To",
    "Transfer-Encoding",
    "TE",
    "Trailer",
    "Sec-WebSocket-Key",
    "Sec-WebSocket-Extensions",
    "Sec-WebSocket-Accept",
    "Sec-WebSocket-Protocol",
    "Sec-WebSocket-Version",
    "Accept-Push-Policy",
    "Accept-Signature",
    "Alt-Svc",
    "Date",
    "Large-Allocation",
    "Link",
    "Push-Policy",
    "Retry-After",
    "Signature",
    "Signed-Headers",
    "Server-Timing",
    "Service-Worker-Allowed",
    "SourceMap",
    "Upgrade",
    "X-DNS-Prefetch-Control",
    "X-Firefox-Spdy",
    "X-Pingback",
    "X-Requested-With",
    "X-Robots-Tag",
    "X-UA-Compatible",
};

// DECLARE =========================================================================================

typedef struct nh_network_HTTPHeader {
    NH_NETWORK_HTTP_HEADER type;
    char *value_p;
} nh_network_HTTPHeader;

static NH_API_RESULT nh_network_getHTTPHeader(
    nh_network_HTTPHeader *Header_p, char *set_p
);

// REQUEST =========================================================================================

nh_network_HTTPRequest nh_network_initHTTPRequest(
    NH_NETWORK_HTTP_REQUEST type)
{
NH_NETWORK_BEGIN()

    nh_network_HTTPRequest Request;
    Request.type = type;
    NH_INIT_LINKED_LIST(Request.Headers)

NH_NETWORK_END(Request)
}

static NH_API_RESULT nh_network_getHTTPRequest(
    nh_network_HTTPRequest Request, char *path_p, char *set_p)
{
NH_NETWORK_BEGIN()

    sprintf(set_p, "GET %s HTTP/1.1", path_p);
   
    int index = strlen(set_p);
    for (int i = 0; i < Request.Headers.count; ++i) 
    {
        char headers_p[2048] = {'\0'};
        NH_NETWORK_CHECK(nh_network_getHTTPHeader(nh_core_getFromLinkedList(&Request.Headers, i), headers_p))
        strcpy(set_p + index, headers_p);
        index += strlen(headers_p);
    }
   
    strcpy(set_p + index, "\r\n\r\n");

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_network_sendHTTPRequest(
    nh_network_ClientSocket *ClientSocket_p, nh_network_HTTPRequest Request, char *path_p, 
    bool secure)
{
NH_NETWORK_BEGIN()

    char request_p[2048] = {'\0'};
    NH_NETWORK_CHECK(nh_network_getHTTPRequest(Request, path_p == NULL ? "/" : path_p, request_p))
    NH_NETWORK_CHECK(nh_network_send(ClientSocket_p, request_p, sizeof(char) * strlen(request_p), secure))
 
NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// RESPONSE ========================================================================================

NH_NETWORK_HTTP_RESPONSE nh_network_getHTTPResponseCode(
    char *response_p)
{
NH_NETWORK_BEGIN()

    if (response_p == NULL) {NH_NETWORK_END(-1)}
    NH_NETWORK_HTTP_RESPONSE code = atoi(response_p + 9);

NH_NETWORK_END(code)
}

// HEADER ==========================================================================================

NH_API_RESULT nh_network_appendHTTPHeader(
    nh_network_HTTPRequest *Request_p, NH_NETWORK_HTTP_HEADER type, char *value_p)
{
NH_NETWORK_BEGIN()

    nh_network_HTTPHeader *Header_p = nh_core_allocate(sizeof(nh_network_HTTPHeader));
    NH_NETWORK_CHECK_MEM(Header_p)

    Header_p->type = type;
    Header_p->value_p = nh_core_allocateBytes(value_p);
    
    NH_CHECK(NH_API_ERROR_BAD_STATE, nh_core_appendToLinkedList(&Request_p->Headers, Header_p))

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_network_getHTTPHeaderValue(
    char *response_p, char *set_p, NH_NETWORK_HTTP_HEADER type)
{
NH_NETWORK_BEGIN()

    const char *header_p = headerNames_pp[type];
    char *p = strstr(response_p, header_p);

    if (p != NULL) 
    {
        p = p + strlen(header_p);
        if (*p != ':') {NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}
        p = p + 1;
        while (*p == ' ') {p = p + 1;}
        char *value_p = p;
        while (*p != '\r' && *(p + 1) != '\n') {p = p + 1;}
        *p = '\0';
        strcpy(set_p, value_p);
        *p = '\r';
    }
    else {NH_NETWORK_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_network_getHTTPHeader(
    nh_network_HTTPHeader *Header_p, char *set_p)
{
NH_NETWORK_BEGIN()

    nh_String String = nh_core_initString(128); 

    NH_CHECK(NH_API_ERROR_BAD_STATE, nh_core_appendFormatToString(&String, "\r\n"))
    NH_CHECK(NH_API_ERROR_BAD_STATE, nh_core_appendFormatToString(&String, (char*)headerNames_pp[Header_p->type]))
    NH_CHECK(NH_API_ERROR_BAD_STATE, nh_core_appendFormatToString(&String, ": %s", Header_p->value_p))

    strcpy(set_p, String.bytes_p);
    nh_core_freeString(&String);
   
NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_network_HTTPPayload nh_network_convertToHTTPPayload(
    void *data_p)
{
NH_NETWORK_BEGIN()

    nh_network_HTTPPayload Payload;
    Payload.body_p = NULL;
    Payload.header_p = NULL;

    char *body_p = strstr(data_p, "\r\n\r\n");
    if (body_p == NULL) {NH_NETWORK_END(Payload)}

    char revert = *body_p;
    *body_p = '\0';
    Payload.header_p = nh_core_allocateBytes(data_p);

    *body_p = revert;
    body_p = body_p + 4;

    // remove trailing stuff 
    int end = 0;
    for (end = strlen(body_p) - 1; body_p[end] == ' ' || body_p[end] == '\r' || body_p[end] == '\n'; end--)
    body_p[end] = '\0';

    Payload.body_p = nh_core_allocateBytes(body_p);

NH_NETWORK_END(Payload)
}

// GET =============================================================================================

NH_API_RESULT nh_network_sendDataRequestViaHTTP(
    nh_network_ClientSocket *Socket_p, char *host_p, char *path_p, bool secure)
{
NH_NETWORK_BEGIN()

    nh_network_HTTPRequest Request = nh_network_initHTTPRequest(NH_NETWORK_HTTP_REQUEST_GET);

    NH_NETWORK_CHECK(nh_network_appendHTTPHeader(&Request, NH_NETWORK_HTTP_HEADER_HOST, host_p))
    NH_NETWORK_CHECK(nh_network_appendHTTPHeader(&Request, NH_NETWORK_HTTP_HEADER_CONNECTION, "close"))
    NH_NETWORK_CHECK(nh_network_appendHTTPHeader(&Request, NH_NETWORK_HTTP_HEADER_USER_AGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36"))
    NH_NETWORK_CHECK(nh_network_appendHTTPHeader(&Request, NH_NETWORK_HTTP_HEADER_ACCEPT_LANGUAGE, "*"))
    NH_NETWORK_CHECK(nh_network_appendHTTPHeader(&Request, NH_NETWORK_HTTP_HEADER_ACCEPT, "*/*"))
 
    NH_API_RESULT result = nh_network_sendHTTPRequest(Socket_p, Request, path_p, secure);

NH_NETWORK_END(result)
}

