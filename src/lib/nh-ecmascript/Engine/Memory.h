#ifndef NH_ECMASCRIPT_MEMORY_H
#define NH_ECMASCRIPT_MEMORY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @addtogroup lib_nh-ecmascript_enums
 *  @{
 */

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef enum NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT {
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_SHARED_MEMORY,
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_WRITE_SHARED_MEMORY,
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT_READ_MODIFY_WRITE_SHARED_MEMORY,
    } NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT;

/** @} */

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    // https://tc39.es/ecma262/#sec-data-blocks
    typedef struct nh_ecmascript_DataBlock {
        bool shared;
        nh_core_Array Data; 
    } nh_ecmascript_DataBlock;

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef struct nh_ecmascript_SharedDataBlockEvent {
        NH_ECMASCRIPT_SHARED_DATA_BLOCK_EVENT type;
        int order;
        bool noTear;
        nh_ecmascript_DataBlock *Block_p;
        unsigned int byteIndex;
        unsigned int elementSize;
        nh_core_Array Payload;
//      ? modifyOp;       
    } nh_ecmascript_SharedDataBlockEvent;

    // https://tc39.es/ecma262/#sec-agent-event-records
    typedef struct nh_ecmascript_AgentEventsRecord {
        int agentSignifier;
        nh_core_Array EventList;
        nh_core_Array AgentSynchronizesWith;
    } nh_ecmascript_AgentEventsRecord;

    // https://tc39.es/ecma262/#sec-candidate-executions
    typedef struct nh_ecmascript_CandidateExecution {
        nh_core_Array EventsRecords;
        nh_core_Array ChosenValues;
    } nh_ecmascript_CandidateExecution;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-event-set
    nh_ecmascript_eventSet(
        nh_ecmascript_CandidateExecution *CandidateExecution_p
    );

    // https://tc39.es/ecma262/#sec-sharedatablockeventset
    nh_core_List nh_ecmascript_sharedDataBlockEventSet(
        nh_ecmascript_CandidateExecution *CandidateExecution_p
    );
    
    // https://tc39.es/ecma262/#sec-hosteventset
    nh_core_List nh_ecmascript_hostEventSet(
        nh_ecmascript_CandidateExecution *CandidateExecution_p
    );
    
    // https://tc39.es/ecma262/#sec-composewriteeventbytes
    nh_core_Array nh_ecmascript_composeWriteEventBytes(
        nh_ecmascript_CandidateExecution *CandidateExecution_p, unsigned int byteIndex, nh_core_List WriteEvents
    );
    
    // https://tc39.es/ecma262/#sec-valueofreadevent
    nh_core_Array nh_ecmascript_valueOfReadEvent(
        nh_ecmascript_CandidateExecution *CandidateExecution_p, nh_ecmascript_SharedDataBlockEvent ReadEvent
    );

/** @} */

#endif
