// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Network.h"
#include "../Header/Socket.h"
#include "../Header/HTTP.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/String.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// DATA ============================================================================================

Nh_Net_Network Network;

// INIT ============================================================================================

static void Nh_Net_initNetwork()
{
NH_BEGIN()

    Network.hostName_p = NULL;
    Network.SSL_p = NULL;
    NH_INIT_LIST(Network.Sockets)

NH_SILENT_END()
}

static NH_RESULT Nh_Net_getHostName()
{
NH_BEGIN()

    if (Network.hostName_p != NULL) {NH_END(NH_SUCCESS)}

#ifdef __unix__

    char hostname_p[2048] = {'\0'};
    gethostname(hostname_p, 2048);
    Network.hostName_p = Nh_allocateChars(hostname_p);

#endif

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Net_createNetwork()
{
NH_BEGIN()

    Nh_Net_initNetwork();

    NH_CHECK(Nh_addListItem(&Network.Sockets, Nh_Net_createClientSocket(NH_NET_PROTOCOL_FAMILY_IPV4, NH_NET_SOCKET_TCP)))
    NH_CHECK(Nh_addListItem(&Network.Sockets, Nh_Net_createClientSocket(NH_NET_PROTOCOL_FAMILY_IPV6, NH_NET_SOCKET_TCP)))

    NH_CHECK(Nh_Net_getHostName())

NH_END(NH_SUCCESS)
}

Nh_Net_Network *Nh_Net_getNetwork()
{
NH_BEGIN()
NH_END(&Network)
}

