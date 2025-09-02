#ifndef NH_CORE_QUEUE_H
#define NH_CORE_QUEUE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "List.h"
#include "../Common/Includes.h"

// TYPE ============================================================================================

typedef nh_core_List nh_core_Queue;

// FUNCTIONS =======================================================================================

nh_core_Queue nh_core_initQueue(
    int chunkSize
);

NH_API_RESULT nh_core_enqueue(
    nh_core_Queue* Queue_p, void* handle_p
);

void* nh_core_dequeue(
    nh_core_Queue* Queue_p
);

void* nh_core_peekQueue(
    nh_core_Queue* Queue_p
);

#endif 
