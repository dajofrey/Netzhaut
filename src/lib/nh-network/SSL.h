#ifndef NH_NETWORK_SSL_H
#define NH_NETWORK_SSL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * netzhaut _ Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Socket.h"

#include "../nh-core/Util/LinkedList.h"

#include <stdint.h>
#include <unistd.h>

#endif

/** @addtogroup lib_nh-network_functions
 *  @{
 */

    size_t nh_network_writeViaSSL(
        void *Connection_p, char *message_p, size_t messageSize
    );

    size_t nh_network_readViaSSL(
        void *Connection_p, char *set_p, size_t count
    );

    void *nh_network_connectViaSSL(
        char *hostName_p, int fd
    );

/** @} */

#endif 
