// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "RingBuffer.h"
#include "String.h"

#include "../System/Memory.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_core_initRingBuffer(
    nh_core_RingBuffer *Buffer_p, int itemCount, int itemByteSize, void init_f(nh_core_RingBuffer *Buffer_p, int itemCount))
{
    nh_core_initRingBufferMarker(&Buffer_p->Marker);

    Buffer_p->itemCount = itemCount;
    Buffer_p->itemByteSize = itemByteSize;

    Buffer_p->overflow = 0;
    Buffer_p->index   = -1;
 
    Buffer_p->data_p = nh_core_allocate(itemCount * itemByteSize);
    NH_CORE_CHECK_MEM(Buffer_p->data_p)

    if (init_f) {
        init_f(Buffer_p, itemCount);
    }

    Buffer_p->overflow = 0;
    Buffer_p->index = -1;

    return NH_API_SUCCESS;
}

void *nh_core_advanceRingBuffer(
    nh_core_RingBuffer *Buffer_p)
{
    Buffer_p->index++;
    if (Buffer_p->index == Buffer_p->itemCount) {Buffer_p->index = 0; Buffer_p->overflow += 1;}
    
    return Buffer_p->data_p + (Buffer_p->index * Buffer_p->itemByteSize);
}

void nh_core_freeRingBuffer(
    nh_core_RingBuffer *Buffer_p)
{
    nh_core_free(Buffer_p->data_p);

    return;
}

void nh_core_initRingBufferMarker(
    nh_core_RingBufferMarker *Marker_p)
{
    Marker_p->overflow = 0;
    Marker_p->index = -1;

    return;
}

void *nh_core_incrementRingBufferMarker(
    nh_core_RingBuffer *Buffer_p, nh_core_RingBufferMarker *Marker_p)
{
    if (Buffer_p->index == Marker_p->index && Buffer_p->overflow == Marker_p->overflow) {return NULL;}

    Marker_p->index++;

    if (Buffer_p->overflow != Marker_p->overflow && Marker_p->index == Buffer_p->itemCount) {
        Marker_p->index = 0; Marker_p->overflow = Buffer_p->overflow;
    }

    return Buffer_p->data_p + (Marker_p->index * Buffer_p->itemByteSize);
}
