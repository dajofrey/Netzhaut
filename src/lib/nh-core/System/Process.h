#ifndef NH_CORE_PROCESS_H
#define NH_CORE_PROCESS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Thread.h"
#include "Channel.h"

#include "../Util/String.h"
#include "../Util/Time.h"

#include "../Common/Includes.h"

#include <stdint.h>
#include <stdlib.h>
    
#ifdef __unix__
    #include <sys/types.h>
#elif defined(_WIN32) || defined(WIN32)
#endif

#endif

/** @addtogroup lib_nh-core_macros
 *  @{
 */

    #define NH_MAX_FORKS 16

/** @} */

/** @addtogroup lib_nh-core_structs
 *  @{
 */

#ifdef __unix__
    typedef pid_t NH_PROCESS;
#endif

    typedef struct nh_IPC {
        nh_Channel In;
        nh_Channel Out;
    } nh_IPC;

    typedef struct nh_Fork {
        NH_PROCESS id;
        nh_IPC IPC;
    } nh_Fork;

    typedef struct nh_Process {
        NH_PROCESS id;
        nh_ThreadPool ThreadPool;
        nh_IPC IPC;
    } nh_Process;

    typedef struct nh_ProcessPoolIPC {
        nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
    } nh_ProcessPoolIPC;

    typedef struct nh_ProcessPool {
        nh_ProcessPoolIPC IPC;
        nh_Process Main;
        nh_Fork Forks_p[NH_MAX_FORKS];
        int forks;
    } nh_ProcessPool;

/** @} */

/** @addtogroup lib_nh-core_vars
 *  @{
 */

    extern nh_ProcessPool NH_PROCESS_POOL;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_API_RESULT nh_core_initProcessPool(
    );

    NH_API_RESULT nh_core_freeProcessPool(
    );

    /**
     * Forks the executing process. This creates a child process which can obtain its own process handle
     * using nh_core_getFork. The function returns NULL if the executing process is the new child process, if
     * the executing process is the main process it returns the child process handle.
     * @return Process handle or NULL.
     */
    nh_Fork *nh_core_fork(
    );

    /**
     * Returns the process handle of the executing process if it is a child of the main process
     * and was created using nh_core_fork. Otherwise returns NULL.
     * @return Process handle or NULL.
     */
    nh_Fork *nh_core_getFork(
    );

    void nh_checkForks(
    );

    int nh_core_activeForks(
    );

    void nh_killForks(
    );

    /**
     * @param write_p Must have prefix "NH_IPC_ID", "NH_IPC_UPDATE" or "NH_IPC_LOG".
     */
    char *nh_core_writeToProcess(
        nh_Process *Proc_p, char *write_p, int writeLen, bool getResponse
    );

    /**
     * @param write_p Must have prefix "NH_IPC_ID", "NH_IPC_UPDATE" or "NH_IPC_LOG".
     */
    char *_nh_core_writeToProcess(
        nh_Process *Proc_p, char *write_p, int writeLen, bool getResponse
    );

    NH_SIGNAL nh_core_runIPCHandler(
        void *args_p
    );

    NH_API_RESULT nh_handleIPCReceive(
        char *bytes_p
    );

/** @} */

#endif 
