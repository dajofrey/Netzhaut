// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Memory.h"

#include "../../DOM/Header/HTMLCollection.h"
#include "../../DOM/Header/Event.h"
#include "../../DOM/Header/MouseEvent.h"
#include "../../DOM/Header/WheelEvent.h"

#include "../../../netzhaut/Memory.h"

#include NH_DEFAULT_CHECK
#include NH_FLOW
#include NH_JS_UTILS

#include <string.h>

// FUNCTIONS =======================================================================================

nh_core_List nh_ecmascript_eventSet(
    nh_ecmascript_CandidateExecution *CandidateExecution_p)
{
    // step 1
    nh_core_List Events = nh_core_initList(16);
    
    // step 2 
    for (int i = 0; i < CandidateExecution_p->EventsRecords.length; i++) 
    {
        nh_ecmascript_AgentEventsRecord *AgentEventsRecord_p = 
            &((nh_ecmascript_AgentEventsRecord*)CandidateExecution_p->EventsRecords.p)[i];

        // step a
        for (int j = 0; j < AgentEventsRecord_p->EventList.length; ++j) {
            nh_ecmascript_SharedDataBlockEvent *SharedDataBlockEvent_p = 
                &((nh_ecmascript_SharedDataBlockEvent*)AgentEventsRecord_p->EventList.p)[j];

            // step i
            nh_core_appendToList(&Events, SharedDataBlockEvent_p);
        }
    }

    return Events;
}

nh_core_List nh_ecmascript_sharedDataBlockEventSet(
    nh_ecmascript_CandidateExecution *CandidateExecution_p)
{
    // step 1
    nh_core_List Events = nh_core_initList(16);
    nh_core_List Tmp = nh_ecmascript_eventSet(CandidateExecution_p); 

    // step 2 
    for (int i = 0; i < Tmp.size; i++) 
    {
        nh_ecmascript_SharedDataBlockEvent *SharedDataBlockEvent_p = Tmp.data_pp[i]; 

        // step a
        if (SharedDataBlockEvent_p->type == NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_SHARED_MEMORY
        ||  SharedDataBlockEvent_p->type == NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_WRITE_SHARED_MEMORY
        ||  SharedDataBlockEvent_p->type == NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_MODIFY_WRITE_SHARED_MEMORY) {
            nh_core_appendToList(&Events, SharedDataBlockEvent_p);
        }
    }

    nh_core_freeList(&Tmp, false);

    return Events;
}

nh_core_List nh_ecmascript_hostEventSet(
    nh_ecmascript_CandidateExecution *CandidateExecution_p)
{
    // step 1
    nh_core_List Events = nh_core_initList(16);
    nh_core_List Tmp1 = nh_ecmascript_eventSet(CandidateExecution_p); 
    nh_core_List Tmp2 = nh_ecmascript_sharedDataBlockEventSet(CandidateExecution_p); 

    // step 2 
    for (int i = 0; i < Tmp.size; i++) 
    {
        nh_ecmascript_SharedDataBlockEvent *SharedDataBlockEvent_p = Tmp1.data_pp[i]; 

        // step
        if (!nh_core_isInList(&Tmp2, SharedDataBlockEvent_p)) {
            nh_core_appendToList(&Events, SharedDataBlockEvent_p);
        }
    }

    nh_core_freeList(&Tmp1, false);
    nh_core_freeList(&Tmp2, false);

    return Events;
}

nh_core_Array nh_ecmascript_composeWriteEventBytes(
    nh_ecmascript_CandidateExecution *CandidateExecution_p, unsigned int byteIndex, nh_core_List WriteEvents)
{
    // step 1
    unsigned int byteLocation = byteIndex;

    // step 2
    nh_core_Array BytesRead = nh_core_initArray(1, 255);

    // step3
    for (int i = 0; i < WriteEvents.size; ++i) {
        // TODO
    }

    return BytesRead;
}

nh_core_Array nh_ecmascript_valueOfReadEvent(
    nh_ecmascript_CandidateExecution *CandidateExecution_p, nh_ecmascript_SharedDataBlockEvent ReadEvent)
{
    nh_core_Array Result = nh_core_initArray(1, 255);
    
    // step 1
    if (ReadEvent.type != NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_SHARED_MEMORY
    &&  ReadEvent.type != NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_MODIFY_WRITE_SHARED_MEMORY) {
        return Result;
    }

    // TODO

    return Result;
}

