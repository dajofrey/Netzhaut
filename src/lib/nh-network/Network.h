#ifndef NH_NETWORK_NETWORK_H
#define NH_NETWORK_NETWORK_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Common/Includes.h"

#include "../nh-core/Util/LinkedList.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup lib_nh-network_typedefs
 *  @{
 */

    typedef NH_API_RESULT (*nh_network_createNetwork_f)(
    );

/** @} */

/** @addtogroup lib_nh-network_structs
 *  @{
 */

    typedef struct nh_network_Network {
        nh_LinkedList Sockets;
        void *SSL_p;
        char *hostName_p;
    } nh_network_Network;

/** @} */

/** @addtogroup lib_nh-network_functions
 *  @{
 */

    NH_API_RESULT nh_network_createNetwork(
    );
    
    nh_network_Network *nh_network_getNetwork(
    );

/** @} */

#endif 
