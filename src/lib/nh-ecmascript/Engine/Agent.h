#ifndef NH_ECMASCRIPT_AGENT_H
#define NH_ECMASCRIPT_AGENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../../nh-core/Util/Stack.h"
#include "../../nh-core/Util/Array.h"

#endif

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

//    typedef struct nh_ecmascript_AgentArgs {
//        NH_UNICODE_ENCODING encoding;
//        NH_BYTE *p;
//    } nh_ecmascript_AgentArgs;

    // https://tc39.es/ecma262/#sec-agents
    typedef struct nh_ecmascript_Agent {
        nh_Stack ExecutionContextStack;
        NH_BOOL littleEndian;
        NH_BOOL canBlock;
        int signifier;
        NH_BOOL isLockFree1;
        NH_BOOL isLockFree2;
        NH_BOOL isLockFree8;
//        nh_ecmascript_CandidateExecution CandidateExecution;
        nh_List KeptAlive;
    } nh_ecmascript_Agent;

/** @} */

/** @addtogroup lib_nh-ecmascript_typedefs
 *  @{
 */

    typedef void *(*nh_ecmascript_initAgent_f)(
        void *args_p
    );
    typedef NH_SIGNAL (*nh_ecmascript_runAgent_f)(
        void *args_p
    );

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    void *nh_ecmascript_initAgent(
        void *args_p
    );
    
    NH_SIGNAL nh_ecmascript_runAgent(
        void *args_p
    );

    nh_ecmascript_Agent *nh_ecmascript_getCurrentAgent(
    );

/** @} */

#endif
