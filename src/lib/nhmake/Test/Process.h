#ifndef NH_MAKE_PROCESS_H
#define NH_MAKE_PROCESS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Channel.h"

#include "../Common/Includes.h"

#include <sys/types.h> 

#endif

/** @addtogroup lib_nhmake_structs
 *  @{
 */

#if defined(__unix__) || defined(__APPLE__)
    typedef pid_t NH_MAKE_PROCESS;
#endif

    typedef struct nh_make_ProcessIPC {
        nh_make_Channel In;
        nh_make_Channel Out;
    } nh_make_ProcessIPC;

    typedef struct nh_make_Process {
        NH_MAKE_PROCESS id;
        nh_make_ProcessIPC IPC;
    } nh_make_Process;

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    nh_make_Process nh_make_fork(
    );

/** @} */

#endif
