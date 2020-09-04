// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/HTTP.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/List.h"
#include "../../Core/Header/String.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

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

typedef struct Nh_Net_HTTPHeader {
    NH_NET_HTTP_HEADER type;
    char *value_p;
} Nh_Net_HTTPHeader;

static NH_RESULT Nh_Net_getHTTPRequest(
    Nh_Net_HTTPRequest Request, char *host_p, char *path_p, char *set_p, NH_BOOL secure
);
static NH_RESULT Nh_Net_getHTTPHeader(
    Nh_Net_HTTPHeader *Header_p, char *set_p
);

static NH_RESULT Nh_Net_sendHTTPRequest(
    Nh_Net_ClientSocket *ClientSocket_p, Nh_Net_HTTPRequest Request, char *host_p, char *path_p, 
    NH_BOOL secure
);

// REQUEST =========================================================================================

Nh_Net_HTTPRequest Nh_Net_initHTTPRequest(
    NH_NET_HTTP_REQUEST type)
{
NH_BEGIN()

    Nh_Net_HTTPRequest Request;
    Request.type = type;
    NH_INIT_LIST(Request.Headers)

NH_END(Request)
}

static NH_RESULT Nh_Net_sendHTTPRequest(
    Nh_Net_ClientSocket *ClientSocket_p, Nh_Net_HTTPRequest Request, char *host_p, char *path_p, 
    NH_BOOL secure)
{
NH_BEGIN()

    char request_p[2048] = {'\0'};
    NH_CHECK(Nh_Net_getHTTPRequest(Request, host_p, path_p == NULL ? "/" : path_p, request_p, secure))
    NH_CHECK(Nh_Net_send(ClientSocket_p, request_p, sizeof(char) * strlen(request_p), secure))
 
NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Net_getHTTPRequest(
    Nh_Net_HTTPRequest Request, char *host_p, char *path_p, char *set_p, NH_BOOL secure)
{
NH_BEGIN()

    if (secure == NH_FALSE) {
        sprintf(set_p, "GET %s HTTP/1.1", path_p);
    }
    else { 
        sprintf(set_p, "GET https://%s%s HTTP/1.1", host_p, path_p);
    }
   
    int index = strlen(set_p);
    for (int i = 0; i < Request.Headers.count; ++i) 
    {
        char headers_p[2048] = {'\0'};
        NH_CHECK(Nh_Net_getHTTPHeader(Nh_getListItem(&Request.Headers, i), headers_p))
        strcpy(set_p + index, headers_p);
        index += strlen(headers_p);
    }
   
    strcpy(set_p + index, "\r\n\r\n");

NH_END(NH_SUCCESS)
}

// RESPONSE ========================================================================================

NH_NET_HTTP_RESPONSE Nh_Net_getHTTPResponseCode(
    char *response_p)
{
NH_BEGIN()

    if (response_p == NULL) {NH_END(-1)}
    NH_NET_HTTP_RESPONSE code = atoi(response_p + 9);

NH_END(code)
}

// HEADER ==========================================================================================

NH_RESULT Nh_Net_appendHTTPHeader(
    Nh_Net_HTTPRequest *Request_p, NH_NET_HTTP_HEADER type, char *value_p)
{
NH_BEGIN()

    Nh_Net_HTTPHeader *Header_p = Nh_allocate(sizeof(Nh_Net_HTTPHeader));
    NH_CHECK_MEM(Header_p)

    Header_p->type = type;
    Header_p->value_p = Nh_allocateChars(value_p);
    
    NH_CHECK(Nh_addListItem(&Request_p->Headers, Header_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Net_getHTTPHeaderValue(
    char *response_p, char *set_p, NH_NET_HTTP_HEADER type)
{
NH_BEGIN()

    const char *header_p = headerNames_pp[type];
    char *p = strstr(response_p, header_p);

    if (p != NULL) 
    {
        p = p + strlen(header_p);
        if (*p != ':') {NH_END(NH_ERROR_BAD_STATE)}
        p = p + 1;
        while (*p == ' ') {p = p + 1;}
        char *value_p = p;
        while (*p != '\r' && *(p + 1) != '\n') {p = p + 1;}
        *p = '\0';
        strcpy(set_p, value_p);
        *p = '\r';
    }
    else {NH_END(NH_ERROR_BAD_STATE)}

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Net_getHTTPHeader(
    Nh_Net_HTTPHeader *Header_p, char *set_p)
{
NH_BEGIN()

    Nh_String *String_p = Nh_allocateString("\r\n");
    NH_CHECK_MEM(String_p)

    NH_CHECK(Nh_appendToString(String_p, (char*)headerNames_pp[Header_p->type]))
    NH_CHECK(Nh_appendFormatToString(String_p, ": %s", Header_p->value_p))

    strcpy(set_p, Nh_getChars(String_p));
    Nh_freeString(String_p, true);
   
NH_END(NH_SUCCESS)
}

void *Nh_Net_getHTTPResponseBody(
    void *data_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    char *body_p = strstr(data_p, "\r\n\r\n");
    if (body_p == NULL) {
        NH_END(NULL)
    }

    body_p = body_p + 4;

    // remove trailing stuff 
    int end = 0;
    for (end = strlen(body_p) - 1; body_p[end] == ' ' || body_p[end] == '\r' || body_p[end] == '\n'; end--)
    body_p[end] = '\0';

    char *allocatedBody_p = Nh_allocateChars(body_p);
    NH_CHECK_NULL(NULL, allocatedBody_p);

#include NH_DEFAULT_CHECK

NH_END(allocatedBody_p)
}

// GET =============================================================================================

NH_RESULT Nh_Net_sendDataRequestViaHTTP(
    Nh_Net_ClientSocket *Socket_p, char *host_p, char *path_p, NH_BOOL secure)
{
NH_BEGIN()

    Nh_Net_HTTPRequest Request = Nh_Net_initHTTPRequest(NH_NET_HTTP_REQUEST_GET);

    NH_CHECK(Nh_Net_appendHTTPHeader(&Request, NH_NET_HTTP_HEADER_HOST, host_p))
    NH_CHECK(Nh_Net_appendHTTPHeader(&Request, NH_NET_HTTP_HEADER_CONNECTION, "close"))
    NH_CHECK(Nh_Net_appendHTTPHeader(&Request, NH_NET_HTTP_HEADER_USER_AGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36"))
    NH_CHECK(Nh_Net_appendHTTPHeader(&Request, NH_NET_HTTP_HEADER_ACCEPT_LANGUAGE, "en-US"))
 
    NH_RESULT result = Nh_Net_sendHTTPRequest(Socket_p, Request, host_p, path_p, secure);

NH_END(result)
}

