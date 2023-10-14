#ifndef NH_ECMASCRIPT_MEMORY_H
#define NH_ECMASCRIPT_MEMORY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @addtogroup lib_nhecmascript_enums
 *  @{
 */

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef enum NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT {
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_SHARED_MEMORY,
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_WRITE_SHARED_MEMORY,
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_MODIFY_WRITE_SHARED_MEMORY,
    } NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT;

/** @} */

/** @addtogroup lib_nhecmascript_structs
 *  @{
 */

    // https://tc39.es/ecma262/#sec-data-blocks
    typedef struct nh_ecmascript_DataBlock {
        NH_BOOL shared;
        nh_Array Data; 
    } nh_ecmascript_DataBlock;

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef struct nh_ecmascript_SharedDataBlockEvent {
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT type;
        int order;
        NH_BOOL noTear;
        nh_ecmascript_DataBlock *Block_p;
        unsigned int byteIndex;
        unsigned int elementSize;
        nh_Array Payload;
//      ? modifyOp;       
    } nh_ecmascript_SharedDataBlockEvent;

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef struct nh_ecmascript_AgentEventsRecord {
        int agentSignifier;
        nh_Array EventList;
        nh_Array AgentSynchronizesWith;
    } nh_ecmascript_AgentEventsRecord;

    // https://tc39.es/ecma262/#sec-candidate-executions
    typedef struct nh_ecmascript_CandidateExecution {
        nh_Array EventsRecords;
        nh_Array ChosenValues;
    } nh_ecmascript_CandidateExecution;

/** @} */

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-event-set
    nh_ecmascript_eventSet(
        nh_ecmascript_CandidateExecution *CandidateExecution_p
    );

    // https://tc39.es/ecma262/#sec-sharedatablockeventset
    nh_List nh_ecmascript_sharedDataBlockEventSet(
        nh_ecmascript_CandidateExecution *CandidateExecution_p
    );
    
    // https://tc39.es/ecma262/#sec-hosteventset
    nh_List nh_ecmascript_hostEventSet(
        nh_ecmascript_CandidateExecution *CandidateExecution_p
    );
    
    // https://tc39.es/ecma262/#sec-composewriteeventbytes
    nh_Array nh_ecmascript_composeWriteEventBytes(
        nh_ecmascript_CandidateExecution *CandidateExecution_p, unsigned int byteIndex, nh_List WriteEvents
    );
    
    // https://tc39.es/ecma262/#sec-valueofreadevent
    nh_Array nh_ecmascript_valueOfReadEvent(
        nh_ecmascript_CandidateExecution *CandidateExecution_p, nh_ecmascript_SharedDataBlockEvent ReadEvent
    );

/** @} */

#endif
