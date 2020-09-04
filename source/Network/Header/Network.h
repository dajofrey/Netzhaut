#ifndef NH_NET_NETWORK_H
#define NH_NET_NETWORK_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Core/Header/List.h"
#include "../../API/Header/Netzhaut.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup NetworkStructs Structs
 *  \ingroup Network
 *  @{
 */

    typedef struct Nh_Net_Network {
        Nh_List Sockets;
        void *SSL_p;
        char *hostName_p;
    } Nh_Net_Network;

/** @} */

/** @addtogroup NetworkFunctions Functions
 *  \ingroup Network
 *  @{
 */

    NH_RESULT Nh_Net_createNetwork(
    );
    
    Nh_Net_Network *Nh_Net_getNetwork(
    );

/** @} */

#endif 
