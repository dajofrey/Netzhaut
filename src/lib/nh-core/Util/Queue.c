// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Queue.h"
#include "../System/Memory.h"
#include <stdlib.h>

// FUNCTIONS =======================================================================================

nh_core_Queue nh_core_initQueue(
    int chunkSize) 
{
    return nh_core_initList(chunkSize);
}

NH_API_RESULT nh_core_enqueue(
    nh_core_Queue* Queue_p, void* handle_p)
{
    return nh_core_appendToList(Queue_p, handle_p);
}

void* nh_core_peekQueue(
    nh_core_Queue* Queue_p) 
{
    if (Queue_p->size == 0) return NULL;
    return Queue_p->pp[0]; // peek at front
}

void* nh_core_dequeue(
    nh_core_Queue* Queue_p)
{
    if (Queue_p->size == 0) return NULL;

    void* handle_p = Queue_p->pp[0];
    nh_core_removeFromList(Queue_p, false, 0); // remove from front
    return handle_p;
}
