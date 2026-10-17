#ifndef NH_NETWORK_HTTP_CLIENT_H
#define NH_NETWORK_HTTP_CLIENT_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../nh-api/nh-network.h"

// FUNCTIONS =======================================================================================

int nh_network_performHTTPRequest(
    const nh_api_HTTPRequest *Request_p, nh_api_HTTPResponse *Response_p
);

void nh_network_freeHTTPResponse(
    nh_api_HTTPResponse *Response_p
);

int nh_network_getHTTPHeader(
    const nh_api_HTTPResponse *Response_p, const char *name_p, char *value_p, size_t size
);

int nh_network_openLoopbackListener(
    int *port_p
);

int nh_network_acceptLoopbackRequest(
    int listener, int timeoutSeconds, char *target_p, size_t targetSize, const char *responseHtml_p
);

void nh_network_closeLoopbackListener(
    int listener
);

#endif // NH_NETWORK_HTTP_CLIENT_H
