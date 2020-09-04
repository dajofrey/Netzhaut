// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/URL.h"
#include "../Header/Socket.h"
#include "../Header/HTTP.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/String.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_CUSTOM_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// DECLARE =========================================================================================

/**
 * Standard: https://url.spec.whatwg.org/
 */
typedef struct Nh_Net_URL {
    char *hostName_p;
    char *resolvedHostName_p;
    char *pathName_p;
    char *search_p;
    char *hash_p;
    Nh_List Sockets;
    struct Connection {
        Nh_Net_ClientSocket *Socket_p;
        NH_NET_PORT port;
    } Connection;
    struct Nh_Net_URL *Forward_p;
} Nh_Net_URL;

static NH_URI_SCHEME Nh_Net_parseURL(
    char *URL_p, char *hostName_p, char *pathName_p
);

static Nh_List Nh_Net_getHostSockets(
    char *hostName_p, char *resolvedHostName_p, NH_URI_SCHEME scheme
);
static Nh_Net_Data Nh_Net_getDataFromHost(
    Nh_Net_URL *URL_p, size_t *size_p, int *index_p
);

static void *Nh_Net_handleHTTPResponse(
    Nh_Net_URL *URL_p, size_t *size_p, int index, void *data_p
);

// INIT ============================================================================================

static void Nh_Net_initURL(
    Nh_Net_URL *URL_p)
{
NH_BEGIN()

    URL_p->hostName_p = NULL;
    URL_p->resolvedHostName_p = NULL;
    URL_p->pathName_p = NULL;
    URL_p->search_p = NULL;
    URL_p->hash_p = NULL;
    URL_p->Forward_p = NULL;

    URL_p->Connection.port = -1;
    URL_p->Connection.Socket_p = NULL;

    NH_INIT_LIST(URL_p->Sockets)

NH_SILENT_END()
}

// CREATE ==========================================================================================

Nh_Net_URL *Nh_Net_createURL(
    char *chars_p, NH_URI_SCHEME *scheme_p)
{
NH_BEGIN()

    char cpy_p[2048] = {'\0'};
    strcpy(cpy_p, chars_p);
    chars_p = cpy_p;

    char path_p[1024] = {'\0'};
    char hostName_p[1024] = {'\0'};

    NH_URI_SCHEME scheme = Nh_Net_parseURL(cpy_p, hostName_p, path_p);

    char resolvedHostName_p[1024] = {'\0'};
    Nh_List Sockets = Nh_Net_getHostSockets(hostName_p, resolvedHostName_p, scheme);
    if (Sockets.count <= 0) {NH_END(NULL)} // client couldn't find host

    Nh_Net_URL *URL_p = Nh_allocate(sizeof(Nh_Net_URL));
    NH_CHECK_MEM(NULL, URL_p)

    Nh_Net_initURL(URL_p);
    URL_p->Sockets = Sockets;
    URL_p->hostName_p = Nh_allocateChars(hostName_p);
    URL_p->resolvedHostName_p = Nh_allocateChars(resolvedHostName_p);
    URL_p->pathName_p = Nh_allocateChars(path_p);

    if (scheme_p != NULL) {*scheme_p = scheme;}

NH_END(URL_p)
}

static Nh_List Nh_Net_getHostSockets(
    char *hostName_p, char *resolvedHostName_p, NH_URI_SCHEME scheme)
{
NH_BEGIN()

    Nh_List Sockets;
    NH_INIT_LIST(Sockets)

    switch (scheme)
    {
        case NH_URI_SCHEME_HTTP      : NH_CHECK(Sockets, Nh_Net_getSockets(hostName_p, resolvedHostName_p, NH_NET_PORT_HTTP, &Sockets)) break;
        case NH_URI_SCHEME_HTTPS     : NH_CHECK(Sockets, Nh_Net_getSockets(hostName_p, resolvedHostName_p, NH_NET_PORT_HTTPS, &Sockets)) break;
        case NH_URI_SCHEME_UNDEFINED :
        {
            NH_CHECK(Sockets, Nh_Net_getSockets(hostName_p, resolvedHostName_p, NH_NET_PORT_HTTP, &Sockets))
            // TODO maybe try others
            break;
        }
    }

NH_END(Sockets)
}

static NH_URI_SCHEME Nh_Net_parseURL(
    char *URL_p, char *hostName_p, char *pathName_p)
{
NH_BEGIN()

    NH_URI_SCHEME scheme = NH_URI_SCHEME_UNDEFINED;

    if (URL_p[0] == 'h' && URL_p[1] == 't' && URL_p[2] == 't' && URL_p[3] == 'p' && URL_p[5] == ':') {
        scheme = NH_URI_SCHEME_HTTP;
    }
    if (URL_p[0] == 'h' && URL_p[1] == 't' && URL_p[2] == 't' && URL_p[3] == 'p' && URL_p[4] == 's' && URL_p[5] == ':') {
        scheme = NH_URI_SCHEME_HTTPS;
    }

    if (scheme != NH_URI_SCHEME_UNDEFINED) { // skip over scheme 
        while (URL_p[0] != ':') {URL_p = URL_p + 1;}
        while (URL_p[0] == ':' || URL_p[0] == '/') {URL_p = URL_p + 1;}
    }

    int pathBegin = 0;
    for (pathBegin = 0; URL_p[pathBegin] != '/' && URL_p[pathBegin] != '\0'; ++pathBegin);
    strcpy(pathName_p, &URL_p[pathBegin]);

    char revert = URL_p[pathBegin];    
    URL_p[pathBegin] = '\0'; // cut off path
    strcpy(hostName_p, URL_p);
    URL_p[pathBegin] = revert;

NH_END(scheme)
}

// FREE ============================================================================================

void Nh_Net_freeURL(
    Nh_URI URI)
{
NH_BEGIN()

NH_SILENT_END()
}

// COPY ============================================================================================

Nh_URI Nh_Net_copyURL(
    Nh_URI *URI_p)
{
}

// COMPARE =========================================================================================

bool Nh_Net_equivalentURL(
    Nh_URI URI1, Nh_URI URI2)
{
}

// DATA FORMAT =====================================================================================

NH_MEDIA Nh_Net_getMediaType(
    Nh_URI URI)
{
}

// GET DATA ========================================================================================

void *Nh_Net_getDataFromURL(
    Nh_Net_URL *URL_p, size_t *size_p, int index)
{
NH_BEGIN()

    void *payload_p = NULL;

    Nh_Net_Data Data = Nh_Net_getDataFromHost(URL_p, size_p, &index);
    if (Data.size == 0 || Data.data_p == NULL) {NH_END(payload_p)}

    switch (Data.port)
    {
        case NH_NET_PORT_HTTP  :
        case NH_NET_PORT_HTTPS : payload_p = Nh_Net_handleHTTPResponse(URL_p, size_p, index, Data.data_p); break;
    }

NH_END(payload_p)
}

static void *Nh_Net_handleHTTPResponse(
    Nh_Net_URL *URL_p, size_t *size_p, int index, void *data_p)
{
NH_BEGIN()

    switch (Nh_Net_getHTTPResponseCode(data_p))
    {
        case NH_NET_HTTP_RESPONSE_MOVED_PERMANENTLY :  
        {
            char headerValue_p[512] = {'\0'};
            NH_CHECK(NULL, Nh_Net_getHTTPHeaderValue(data_p, headerValue_p, NH_NET_HTTP_HEADER_LOCATION))

            URL_p->Forward_p = Nh_Net_createURL(headerValue_p, NULL);
            NH_CHECK_NULL(NULL, URL_p->Forward_p)

            Nh_free(data_p);
            NH_END(Nh_Net_getDataFromURL(URL_p->Forward_p, size_p, 0))
        }
        case NH_NET_HTTP_RESPONSE_FOUND : // TODO temporary forward
        {
            char headerValue_p[512] = {'\0'};
            NH_CHECK(NULL, Nh_Net_getHTTPHeaderValue(data_p, headerValue_p, NH_NET_HTTP_HEADER_LOCATION))

            URL_p->Forward_p = Nh_Net_createURL(headerValue_p, NULL);
            NH_CHECK_NULL(NULL, URL_p->Forward_p)

            Nh_free(data_p);
            NH_END(Nh_Net_getDataFromURL(URL_p->Forward_p, size_p, 0))
        }
        case NH_NET_HTTP_RESPONSE_BAD_GATEWAY : 
        {
            Nh_free(data_p);
            NH_END(Nh_Net_getDataFromURL(URL_p, size_p, index)) // try next socket
        }
        case NH_NET_HTTP_RESPONSE_BAD_REQUEST :
        case NH_NET_HTTP_RESPONSE_OK : 
        {
            void *body_p = Nh_Net_getHTTPResponseBody(data_p);
            Nh_free(data_p);
            NH_END(body_p)
        }
        default : exit(0);
    }

NH_END(NULL)
}

static Nh_Net_Data Nh_Net_getDataFromHost(
    Nh_Net_URL *URL_p, size_t *size_p, int *index_p)
{
NH_BEGIN()

    Nh_Net_Data Data = Nh_Net_initData();

    for (int i = *index_p; i < URL_p->Sockets.count; ++i, *index_p += 1)
    {
        Nh_Net_Socket *Socket_p = Nh_getListItem(&URL_p->Sockets, i);
        Nh_Net_ClientSocket *ClientSocket_p = Nh_Net_getClientSocket(*Socket_p);

        if (ClientSocket_p != NULL && Nh_Net_connect(ClientSocket_p, *Socket_p) == NH_SUCCESS) 
        {
            Data = Nh_Net_getDataFromSocket(
                ClientSocket_p, URL_p->hostName_p, URL_p->pathName_p, Socket_p->port
            );
            if (Data.size > 0 && Data.data_p != NULL) {
                *index_p += 1; 
                NH_END(Data)
            }
        }
    }

NH_END(Data)
}

// FREE ============================================================================================

void Nh_Net_freeData(
    Nh_URI URI)
{
}

