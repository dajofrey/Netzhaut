// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-network.h"
#include "nh-core.h"
#include "../nh-core/Loader/Loader.h"

#include <stdio.h>
#include <string.h>

// HELPERS =========================================================================================

static void *nh_api_loadNetworkSymbol(
    const char *name_p)
{
    nh_core_Loader *Loader_p = nh_api_getLoader();
    return !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_NETWORK, 0, (char*)name_p);
}

// FUNCTIONS =======================================================================================

int nh_api_performHTTPRequest(
    const nh_api_HTTPRequest *Request_p, nh_api_HTTPResponse *Response_p)
{
    typedef int (*nh_network_performHTTPRequest_f)(const nh_api_HTTPRequest *, nh_api_HTTPResponse *);
    nh_network_performHTTPRequest_f performHTTPRequest_f = nh_api_loadNetworkSymbol("nh_network_performHTTPRequest");
    if (!performHTTPRequest_f) {
        if (Response_p) {
            memset(Response_p, 0, sizeof(*Response_p));
            snprintf(Response_p->error_p, sizeof(Response_p->error_p), "nh-network is not available");
        }
        return -1;
    }
    return performHTTPRequest_f(Request_p, Response_p);
}

void nh_api_freeHTTPResponse(
    nh_api_HTTPResponse *Response_p)
{
    typedef void (*nh_network_freeHTTPResponse_f)(nh_api_HTTPResponse *);
    nh_network_freeHTTPResponse_f freeHTTPResponse_f = nh_api_loadNetworkSymbol("nh_network_freeHTTPResponse");
    if (freeHTTPResponse_f) {freeHTTPResponse_f(Response_p);}
}

int nh_api_getHTTPHeader(
    const nh_api_HTTPResponse *Response_p, const char *name_p, char *value_p, size_t size)
{
    typedef int (*nh_network_getHTTPHeader_f)(const nh_api_HTTPResponse *, const char *, char *, size_t);
    nh_network_getHTTPHeader_f getHTTPHeader_f = nh_api_loadNetworkSymbol("nh_network_getHTTPHeader");
    return getHTTPHeader_f ? getHTTPHeader_f(Response_p, name_p, value_p, size) : 0;
}

int nh_api_openLoopbackListener(
    int *port_p)
{
    typedef int (*nh_network_openLoopbackListener_f)(int *);
    nh_network_openLoopbackListener_f openLoopbackListener_f = nh_api_loadNetworkSymbol("nh_network_openLoopbackListener");
    return openLoopbackListener_f ? openLoopbackListener_f(port_p) : -1;
}

int nh_api_acceptLoopbackRequest(
    int listener, int timeoutSeconds, char *target_p, size_t targetSize, const char *responseHtml_p)
{
    typedef int (*nh_network_acceptLoopbackRequest_f)(int, int, char *, size_t, const char *);
    nh_network_acceptLoopbackRequest_f acceptLoopbackRequest_f = nh_api_loadNetworkSymbol("nh_network_acceptLoopbackRequest");
    return acceptLoopbackRequest_f ? acceptLoopbackRequest_f(listener, timeoutSeconds, target_p, targetSize, responseHtml_p) : -1;
}

void nh_api_closeLoopbackListener(
    int listener)
{
    typedef void (*nh_network_closeLoopbackListener_f)(int);
    nh_network_closeLoopbackListener_f closeLoopbackListener_f = nh_api_loadNetworkSymbol("nh_network_closeLoopbackListener");
    if (closeLoopbackListener_f) {closeLoopbackListener_f(listener);}
}
