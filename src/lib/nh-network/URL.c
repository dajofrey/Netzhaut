//// LICENSE NOTICE ==================================================================================
//
///**
// * Netzhaut - Web Browser Engine
// * Copyright (C) 2022  Dajo Frey
// * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
// */
//
//// INCLUDES =======================================================================================
//
//#include "URL.h"
//#include "Socket.h"
//#include "HTTP.h"
//#include "HTTP.h"
//#include "Network.h"
//
//#include "../nh-core/System/Memory.h"
//#include "../nh-core/Util/String.h"
//#include "../nh-core/MediaType.h"
//
//#include "Common/Macros.h"
//#include NH_NETWORK_FLOW
//#include NH_NETWORK_DEFAULT_CHECK
//
//#include <string.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <limits.h>
//
//// DECLARE =========================================================================================
//
//typedef enum NH_NETWORK_PAYLOAD {
//    NH_NETWORK_PAYLOAD_HTTP,
//} NH_NETWORK_PAYLOAD;
//
//typedef struct nh_network_Payload {
//    NH_NETWORK_PAYLOAD type;
//    nh_network_HTTPPayload HTTP;
//} nh_network_Payload;
//
///**
// * Standard: https://url.spec.whatwg.org/
// */
//typedef struct nh_network_URL {
//    char *hostName_p;
//    char *resolvedHostName_p;
//    char *pathName_p;
//    char *search_p;
//    char *hash_p;
//    nh_LinkedList Sockets;
//    struct Connection {
//        nh_network_ClientSocket *Socket_p;
//        NH_NETWORK_PORT port;
//    } Connection;
//    struct nh_network_URL *Forward_p;
//    nh_network_Payload Payload;
//} nh_network_URL;
//
//static nh_network_Payload nh_network_getGenericPayloadFromURL(
//    nh_network_URL *URL_p, size_t *size_p, NH_URI_SCHEME *scheme_p, int index
//);
//
//// INIT ============================================================================================
//
//static void nh_network_initURL(
//    nh_network_URL *URL_p)
//{
//NH_NETWORK_BEGIN()
//
//    URL_p->hostName_p = NULL;
//    URL_p->resolvedHostName_p = NULL;
//    URL_p->pathName_p = NULL;
//    URL_p->search_p = NULL;
//    URL_p->hash_p = NULL;
//    URL_p->Forward_p = NULL;
//
//    URL_p->Connection.port = -1;
//    URL_p->Connection.Socket_p = NULL;
//
//    URL_p->Payload.type = -1;
//    URL_p->Payload.HTTP.body_p = NULL;
//    URL_p->Payload.HTTP.header_p = NULL;
//
//    NH_INIT_LINKED_LIST(URL_p->Sockets)
//
//NH_NETWORK_SILENT_END()
//}
//
//// CREATE ==========================================================================================
//
//static nh_LinkedList nh_network_getHostSockets(
//    char *hostName_p, char *resolvedHostName_p, NH_URI_SCHEME scheme)
//{
//NH_NETWORK_BEGIN()
//
//#include NH_NETWORK_CUSTOM_CHECK
//
//    nh_LinkedList Sockets;
//    NH_INIT_LINKED_LIST(Sockets)
//
//    switch (scheme)
//    {
//        case NH_URI_SCHEME_HTTP      : NH_NETWORK_CHECK(Sockets, nh_network_getSockets(hostName_p, resolvedHostName_p, NH_NETWORK_PORT_HTTP, &Sockets)) break;
//        case NH_URI_SCHEME_HTTPS     : NH_NETWORK_CHECK(Sockets, nh_network_getSockets(hostName_p, resolvedHostName_p, NH_NETWORK_PORT_HTTPS, &Sockets)) break;
//        case NH_URI_SCHEME_UNDEFINED :
//        {
//            NH_NETWORK_CHECK(Sockets, nh_network_getSockets(hostName_p, resolvedHostName_p, NH_NETWORK_PORT_HTTP, &Sockets))
//            // TODO maybe try others
//            break;
//        }
//    }
//
//#include NH_NETWORK_DEFAULT_CHECK
//
//NH_NETWORK_END(Sockets)
//}
//
//static NH_URI_SCHEME nh_network_parseURL(
//    char *URL_p, char *hostName_p, char *pathName_p)
//{
//NH_NETWORK_BEGIN()
//
//    NH_URI_SCHEME scheme = NH_URI_SCHEME_UNDEFINED;
//
//    if (URL_p[0] == 'h' && URL_p[1] == 't' && URL_p[2] == 't' && URL_p[3] == 'p' && URL_p[5] == ':') {
//        scheme = NH_URI_SCHEME_HTTP;
//    }
//    if (URL_p[0] == 'h' && URL_p[1] == 't' && URL_p[2] == 't' && URL_p[3] == 'p' && URL_p[4] == 's' && URL_p[5] == ':') {
//        scheme = NH_URI_SCHEME_HTTPS;
//    }
//
//    if (scheme != NH_URI_SCHEME_UNDEFINED) { // skip over scheme 
//        while (URL_p[0] != ':') {URL_p = URL_p + 1;}
//        while (URL_p[0] == ':' || URL_p[0] == '/') {URL_p = URL_p + 1;}
//    }
//
//    int pathBegin = 0;
//    for (pathBegin = 0; URL_p[pathBegin] != '/' && URL_p[pathBegin] != '\0'; ++pathBegin);
//    strcpy(pathName_p, &URL_p[pathBegin]);
//
//    char revert = URL_p[pathBegin];    
//    URL_p[pathBegin] = '\0'; // cut off path
//    strcpy(hostName_p, URL_p);
//    URL_p[pathBegin] = revert;
//
//NH_NETWORK_END(scheme)
//}
//
//static nh_network_URL *nh_network_createURLFromLocation(
//    char *location_p, NH_URI_SCHEME *scheme_p)
//{
//NH_NETWORK_BEGIN()
//
//#include NH_NETWORK_CUSTOM_CHECK
//
//    char path_p[1024] = {'\0'};
//    char hostName_p[1024] = {'\0'};
//
//    NH_URI_SCHEME scheme = nh_network_parseURL(location_p, hostName_p, path_p);
//    if (scheme_p != NULL) {*scheme_p = scheme;}
//
//    char resolvedHostName_p[1024] = {'\0'};
//    nh_LinkedList Sockets = nh_network_getHostSockets(hostName_p, resolvedHostName_p, scheme);
//    if (Sockets.count <= 0) {NH_NETWORK_END(NULL)}
//
//    nh_network_URL *URL_p = nh_core_allocate(sizeof(nh_network_URL));
//    NH_NETWORK_CHECK_MEM(NULL, URL_p)
//
//    nh_network_initURL(URL_p);
//    URL_p->Sockets = Sockets;
//
//    URL_p->hostName_p = nh_core_allocateBytes(hostName_p);
//    NH_NETWORK_CHECK_MEM(NULL, URL_p->hostName_p)
//
//    URL_p->resolvedHostName_p = nh_core_allocateBytes(resolvedHostName_p);
//    NH_NETWORK_CHECK_MEM(NULL, URL_p->resolvedHostName_p)
//
//    URL_p->pathName_p = nh_core_allocateBytes(path_p);
//    NH_NETWORK_CHECK_MEM(NULL, URL_p->pathName_p)
//
//#include NH_NETWORK_DEFAULT_CHECK
//
//NH_NETWORK_END(URL_p)
//}
//
//NH_NETWORK_RESULT nh_network_createURL(
//    nh_URI *URI_p)
//{
//NH_NETWORK_BEGIN()
//
//    // TODO URI_p->base ignored
//
//    URI_p->data_p = nh_network_createURLFromLocation(URI_p->location_p, &URI_p->scheme);
//    NH_NETWORK_CHECK_MEM(URI_p->data_p)
//
//NH_NETWORK_DIAGNOSTIC_END(NH_NETWORK_SUCCESS)
//}
//
//// FREE ============================================================================================
//
//void nh_network_freeURL(
//    nh_URI *URI_p)
//{
//NH_NETWORK_BEGIN()
//
//NH_NETWORK_SILENT_END()
//}
//
//// COPY ============================================================================================
//
//nh_URI nh_network_copyURL(
//    nh_URI *URI_p)
//{
//}
//
//// COMPARE =========================================================================================
//
//bool nh_network_equivalentURL(
//    nh_URI URI1, nh_URI URI2)
//{
//}
//
//// DATA FORMAT =====================================================================================
//
//NH_MEDIA nh_network_getMediaType(
//    nh_network_URL *URL_p)
//{
//NH_NETWORK_BEGIN()
//
//#include NH_NETWORK_CUSTOM_CHECK
//
//     if (URL_p == NULL) {NH_NETWORK_END(-1)}
//
//     switch (URL_p->Payload.type)
//     {
//         case NH_NETWORK_PAYLOAD_HTTP :
//         {
//             char headerValue_p[512] = {'\0'};
//             NH_NETWORK_CHECK(-1, nh_network_getHTTPHeaderValue(
//                 URL_p->Payload.HTTP.header_p, headerValue_p, NH_NETWORK_HTTP_HEADER_CONTENT_TYPE
//             ))
//
//             for (int i = 0; i < strlen(headerValue_p); ++i) {
//                 if (headerValue_p[i] == ';') {
//                     headerValue_p[i] = '\0'; break;
//                 }
//             }
//
//             NH_NETWORK_END(nh_core_getMediaTypeFromTemplate(headerValue_p))
//             break;
//         }
//     }
//
//#include NH_NETWORK_DEFAULT_CHECK
//
//NH_NETWORK_END(-1)
//}
//
//// GET DATA ========================================================================================
//
//#include NH_NETWORK_CUSTOM_CHECK
//
//static nh_network_Data nh_network_getDataFromHost(
//    nh_network_URL *URL_p, size_t *size_p, int *index_p)
//{
//NH_NETWORK_BEGIN()
//
//    nh_network_Data Data = nh_network_initData();
//
//    for (int i = *index_p; i < URL_p->Sockets.count; ++i, *index_p += 1)
//    {
//        nh_network_Socket *Socket_p = nh_core_getFromLinkedList(&URL_p->Sockets, i);
//        nh_network_ClientSocket *ClientSocket_p = nh_network_getClientSocket(*Socket_p);
//
//        if (ClientSocket_p != NULL && nh_network_connect(URL_p->hostName_p, ClientSocket_p, *Socket_p) == NH_NETWORK_SUCCESS) 
//        {
//            Data = nh_network_getDataFromSocket(
//                ClientSocket_p, URL_p->hostName_p, URL_p->pathName_p, Socket_p->port
//            );
//            if (Data.size > 0 && Data.data_p != NULL) {
//                *index_p += 1; 
//                NH_NETWORK_END(Data)
//            }
//        }
//    }
//
//NH_NETWORK_END(Data)
//}
//
//static nh_network_Payload nh_network_handleHTTPResponse(
//    nh_network_URL *URL_p, size_t *size_p, int index, void *data_p, NH_URI_SCHEME *scheme_p)
//{
//NH_NETWORK_BEGIN()
//
//    nh_network_Payload Payload;
//    Payload.type = -1;
//
//    switch (nh_network_getHTTPResponseCode(data_p))
//    {
//        case NH_NETWORK_HTTP_RESPONSE_MOVED_PERMANENTLY :  
//        {
//            char headerValue_p[512] = {'\0'};
//            NH_NETWORK_CHECK(Payload, nh_network_getHTTPHeaderValue(data_p, headerValue_p, NH_NETWORK_HTTP_HEADER_LOCATION))
//
//            nh_core_free(URL_p->Forward_p);
//            URL_p->Forward_p = nh_network_createURLFromLocation(headerValue_p, NULL);
//            NH_NETWORK_CHECK_NULL(Payload, URL_p->Forward_p)
//
//            nh_core_free(data_p);
//            NH_NETWORK_END(nh_network_getGenericPayloadFromURL(URL_p->Forward_p, size_p, scheme_p, 0))
//        }
//        case NH_NETWORK_HTTP_RESPONSE_FOUND : // TODO temporary forward
//        {
//            char headerValue_p[512] = {'\0'};
//            NH_NETWORK_CHECK(Payload, nh_network_getHTTPHeaderValue(data_p, headerValue_p, NH_NETWORK_HTTP_HEADER_LOCATION))
//
//            URL_p->Forward_p = nh_network_createURLFromLocation(headerValue_p, NULL);
//            NH_NETWORK_CHECK_NULL(Payload, URL_p->Forward_p)
//
//            nh_core_free(data_p);
//            NH_NETWORK_END(nh_network_getGenericPayloadFromURL(URL_p->Forward_p, size_p, scheme_p, 0))
//        }
//        case NH_NETWORK_HTTP_RESPONSE_BAD_GATEWAY : 
//        {
//            nh_core_free(data_p);
//            NH_NETWORK_END(nh_network_getGenericPayloadFromURL(URL_p, size_p, scheme_p, index)) // try next socket
//        }
//        case NH_NETWORK_HTTP_RESPONSE_MISDIRECTED_REQUEST :
//        {
//            break;
//        }
//        case NH_NETWORK_HTTP_RESPONSE_BAD_REQUEST :
//        {
//            nh_core_free(data_p);
//            NH_NETWORK_END(nh_network_getGenericPayloadFromURL(URL_p, size_p, scheme_p, index)) // try next socket
//        }
//        case NH_NETWORK_HTTP_RESPONSE_OK : 
//        {
//            Payload.type = NH_NETWORK_PAYLOAD_HTTP;
//            Payload.HTTP = nh_network_convertToHTTPPayload(data_p);
//            nh_core_free(data_p);
//            break;
//        }
//        default : printf("unhandled HTTP response code!"); exit(0);
//    }
//
//NH_NETWORK_END(Payload)
//}
//
//static nh_network_Payload nh_network_getGenericPayloadFromURL(
//    nh_network_URL *URL_p, size_t *size_p, NH_URI_SCHEME *scheme_p, int index)
//{
//NH_NETWORK_BEGIN()
//
//    nh_network_Payload Payload;
//    Payload.type = -1;
//
//    nh_network_Data Data = nh_network_getDataFromHost(URL_p, size_p, &index);
//    if (Data.size == 0 || Data.data_p == NULL) {NH_NETWORK_END(Payload)}
//
//    switch (Data.port)
//    {
//        case NH_NETWORK_PORT_HTTP  : 
//        {
//            if (scheme_p != NULL) {*scheme_p = NH_URI_SCHEME_HTTP;}
//            NH_NETWORK_END(nh_network_handleHTTPResponse(URL_p, size_p, index, Data.data_p, scheme_p))
//            break;
//        }
//        case NH_NETWORK_PORT_HTTPS : 
//        {
//            if (scheme_p != NULL) {*scheme_p = NH_URI_SCHEME_HTTPS;}
//            NH_NETWORK_END(nh_network_handleHTTPResponse(URL_p, size_p, index, Data.data_p, scheme_p))
//            break;
//        }
//    }
//
//NH_NETWORK_END(Payload)
//}
//
//void *nh_network_getPayloadFromURL(
//    nh_network_URL *URL_p, size_t *size_p, NH_URI_SCHEME *scheme_p, int index)
//{
//NH_NETWORK_BEGIN()
//
//    URL_p->Payload = nh_network_getGenericPayloadFromURL(URL_p, size_p, scheme_p, index);
//
//    switch (URL_p->Payload.type)
//    {
//        case NH_NETWORK_PAYLOAD_HTTP : NH_NETWORK_END(URL_p->Payload.HTTP.body_p)
//    }
//
//NH_NETWORK_END(NULL)
//}
//
//// FREE ============================================================================================
//
//void nh_network_freePayload(
//    nh_network_URL *URL_p)
//{
//NH_NETWORK_BEGIN()
//
//    nh_core_free(URL_p->Payload.HTTP.body_p);
//    nh_core_free(URL_p->Payload.HTTP.header_p);
//
//NH_NETWORK_SILENT_END()
//}
//
