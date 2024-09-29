// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Network.h"
#include "Socket.h"
#include "HTTP.h"

#include "../nh-core/System/Memory.h"
#include "../nh-core/Util/String.h"

#include "../nh-core/Common/Macros.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

#include "Common/Macros.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// DATA ============================================================================================

nh_network_Network Network;

// INIT ============================================================================================

static void nh_network_initNetwork()
{
NH_NETWORK_BEGIN()

    Network.hostName_p = NULL;
    Network.SSL_p = NULL;
    NH_INIT_LINKED_LIST(Network.Sockets)

NH_NETWORK_SILENT_END()
}

static NH_API_RESULT nh_network_getHostName()
{
NH_NETWORK_BEGIN()

    if (Network.hostName_p != NULL) {NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)}

#ifdef __unix__

    char hostname_p[2048] = {'\0'};
    gethostname(hostname_p, 2048);
    Network.hostName_p = nh_core_allocateBytes(hostname_p);

#endif

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_network_createNetwork()
{
NH_NETWORK_BEGIN()

    nh_network_initNetwork();

    NH_CHECK(NH_API_ERROR_BAD_STATE, 
        nh_core_appendToLinkedList(&Network.Sockets, nh_network_createClientSocket(NH_NETWORK_PROTOCOL_FAMILY_IPV4, NH_NETWORK_SOCKET_TCP)
    ))
    NH_CHECK(NH_API_ERROR_BAD_STATE, 
        nh_core_appendToLinkedList(&Network.Sockets, nh_network_createClientSocket(NH_NETWORK_PROTOCOL_FAMILY_IPV6, NH_NETWORK_SOCKET_TCP)
    ))

    NH_NETWORK_CHECK(nh_network_getHostName())

NH_NETWORK_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_network_Network *nh_network_getNetwork()
{
NH_NETWORK_BEGIN()
NH_NETWORK_END(&Network)
}

