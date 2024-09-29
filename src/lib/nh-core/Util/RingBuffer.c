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
#include "../Common/Macros.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

// ITERATOR ========================================================================================

NH_API_RESULT nh_core_initRingBuffer(
    nh_RingBuffer *Buffer_p, int itemCount, int itemByteSize, void init_f(nh_RingBuffer *Buffer_p, int itemCount))
{
NH_CORE_BEGIN()

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

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

void *nh_core_advanceRingBuffer(
    nh_RingBuffer *Buffer_p)
{
NH_CORE_BEGIN()

    Buffer_p->index++;
    if (Buffer_p->index == Buffer_p->itemCount) {Buffer_p->index = 0; Buffer_p->overflow += 1;}
    
NH_CORE_END(Buffer_p->data_p + (Buffer_p->index * Buffer_p->itemByteSize))
}

void nh_core_freeRingBuffer(
    nh_RingBuffer *Buffer_p)
{
NH_CORE_BEGIN()

    nh_core_free(Buffer_p->data_p);

NH_CORE_SILENT_END()
}

// MARK ============================================================================================

void nh_core_initRingBufferMarker(
    nh_RingBufferMarker *Marker_p)
{
NH_CORE_BEGIN()

    Marker_p->overflow = 0;
    Marker_p->index = -1;

NH_CORE_SILENT_END()
}

void *nh_core_incrementRingBufferMarker(
    nh_RingBuffer *Buffer_p, nh_RingBufferMarker *Marker_p)
{
NH_CORE_BEGIN()

    if (Buffer_p->index == Marker_p->index && Buffer_p->overflow == Marker_p->overflow) {NH_CORE_END(NULL)}

    Marker_p->index++;

    if (Buffer_p->overflow != Marker_p->overflow && Marker_p->index == Buffer_p->itemCount) {
        Marker_p->index = 0; Marker_p->overflow = Buffer_p->overflow;
    }

NH_CORE_END(Buffer_p->data_p + (Marker_p->index * Buffer_p->itemByteSize))
}

