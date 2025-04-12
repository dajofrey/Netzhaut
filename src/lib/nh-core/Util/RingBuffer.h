#ifndef NH_CORE_RING_BUFFER_H
#define NH_CORE_RING_BUFFER_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

// STRUCTS =========================================================================================

typedef struct NH_ALIGN_16 nh_core_RingBufferMarker { 
    int index; 
    int overflow; 
} nh_core_RingBufferMarker; 

typedef struct NH_ALIGN_16 nh_core_RingBuffer { 
    nh_core_RingBufferMarker Marker; /**<For convenience.*/ 
    int itemCount, itemByteSize; 
    int index; 
    int overflow; 
    char *data_p; 
} nh_core_RingBuffer; 

// FUNCTIONS =======================================================================================
 
NH_API_RESULT nh_core_initRingBuffer(
    nh_core_RingBuffer *Buffer_p, int itemCount, int itemByteSize, void init_f(nh_core_RingBuffer *Buffer_p, int itemCount)
);

char *nh_core_advanceRingBuffer(
    nh_core_RingBuffer *Buffer_p
); 

void nh_resetRingBuffer(
    nh_core_RingBuffer *Buffer_p
);

void nh_core_freeRingBuffer(
    nh_core_RingBuffer *Buffer_p
);

void nh_core_initRingBufferMarker(
    nh_core_RingBufferMarker *Marker_p
);

char *nh_core_incrementRingBufferMarker(
    nh_core_RingBuffer *Buffer_p, nh_core_RingBufferMarker *Marker_p
);

#endif 
