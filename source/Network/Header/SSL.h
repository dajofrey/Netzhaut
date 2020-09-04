#ifndef NH_NET_SSL_H
#define NH_NET_SSL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut _ Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Socket.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/List.h"

#include <stdint.h>
#include <unistd.h>

#endif

/** @addtogroup NetworkFunctions Functions
 *  \ingroup Network
 *  @{
 */

    size_t Nh_Net_writeViaSSL(
        void *Connection_p, char *message_p, size_t messageSize
    );

    size_t Nh_Net_readViaSSL(
        void *Connection_p, char *set_p, size_t count
    );

    void *Nh_Net_connectViaSSL(
        int fd
    );

/** @} */

#endif 
