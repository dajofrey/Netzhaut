#ifndef NH_MAKE_CHANNEL_H
#define NH_MAKE_CHANNEL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stddef.h>

#endif

/** @addtogroup smcoreStructs
 *  @{
 */

    // TODO semaphore etc.
    typedef struct nh_make_Channel {
#if defined(__linux__) || defined(__APPLE__)
        int rw_p[2];
#endif
    } nh_make_Channel;

/** @} */

/** @addtogroup smcoreFunctions
 *  @{
 */

    NH_MAKE_RESULT nh_make_initChannel(
        nh_make_Channel *Channel_p
    );
    
    NH_MAKE_RESULT nh_make_openChannel(
        nh_make_Channel *Channel_p
    );
    
    void nh_make_closeChannelReadAccess(
        nh_make_Channel *Channel_p
    );
    
    void nh_make_closeChannelWriteAccess(
        nh_make_Channel *Channel_p
    );
    
    int nh_make_writeToChannel(
        nh_make_Channel *Channel_p, char *bytes_p, int byteCount
    );
    
    char *nh_make_readFromChannel(
        nh_make_Channel *Channel_p, size_t *size_p
    );

/** @} */

#endif 
