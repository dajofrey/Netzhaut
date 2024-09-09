#ifndef NH_CORE_RING_BUFFER_H
#define NH_CORE_RING_BUFFER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_initRingBuffer(
        nh_RingBuffer *Buffer_p, int itemCount, int itemByteSize, void init_f(nh_RingBuffer *Buffer_p, int itemCount)
    );

    void *nh_core_advanceRingBuffer(
        nh_RingBuffer *Buffer_p
    ); 

    void nh_resetRingBuffer(
        nh_RingBuffer *Buffer_p
    );

    void nh_core_freeRingBuffer(
        nh_RingBuffer *Buffer_p
    );

    void nh_core_initRingBufferMarker(
        nh_RingBufferMarker *Marker_p
    );
   
    void *nh_core_incrementRingBufferMarker(
        nh_RingBuffer *Buffer_p, nh_RingBufferMarker *Marker_p
    );
    
/** @} */

#endif 
