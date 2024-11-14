#ifndef NH_CORE_THREAD_H
#define NH_CORE_THREAD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "../Util/RingBuffer.h"
#include "../Util/Time.h"
#include "../Loader/Loader.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
    
#ifdef __unix__
#include <pthread.h>
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

#endif

/** @addtogroup lib_nh-core_macros
 *  @{
 */

    #define NH_MAX_THREADS 64
    #define NH_MAX_WORKLOADS 128

/** @} */

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_core_Thread nh_core_Thread;

    typedef struct nh_core_WorkloadCommand {
        bool done;
        bool dummy;
        int type;
        void *p;
        int length;
    } nh_core_WorkloadCommand;

    typedef struct nh_SignalCounter {
        int ok;
        int idle;
    } nh_SignalCounter;

    typedef struct nh_core_Workload {
        nh_core_Thread *Thread_p;
        nh_SignalCounter SignalCounter;
        nh_core_RingBuffer Commands;
        nh_core_RingBufferMarker Marker;
        NH_SIGNAL signal;
        bool crucial;
        NH_MODULE_E module;
        char *name_p;
        char *path_p;
        void *args_p;
        void *(*init_f)(struct nh_core_Workload*);
        NH_SIGNAL (*run_f)(void*);
        void (*free_f)(void*);
        NH_SIGNAL (*runCommand_f)(void*, nh_core_WorkloadCommand*);
        struct {
            int idleDelay; 
            int runDelay;  
            int haltDelay; 
            int waitDelay; 
            double turnAround;
        } Timing;
    } nh_core_Workload;

    typedef struct nh_core_Thread {
        int depth;
        nh_core_Workload *CurrentWorkload_p;
        struct {
    #ifdef __unix__
            int writeTTY_p[2];
            int readTTY_p[2];
    #endif
        } IPC;
    #ifdef __unix__
        pthread_t id;              
    #elif defined(_WIN32) || defined (WIN32)
        DWORD id;              
    #endif
    } nh_core_Thread;

    typedef struct nh_core_ThreadPool {
        nh_core_Thread Main;
        nh_core_Thread Threads_p[NH_MAX_THREADS];
        nh_core_Workload Workloads_p[NH_MAX_WORKLOADS];
        int threadCount;    
    } nh_core_ThreadPool;

/** @} */

/** @addtogroup lib_nh-core_typedefs
 *  @{
 */

    typedef bool (*nh_core_keepRunning_f)(
    );

    typedef nh_core_Workload *(*nh_core_getWorkload_f)(
        void *args_p
    );
 
/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_core_ThreadPool nh_core_initThreadPool(
    );
    
    NH_API_RESULT nh_core_freeThreadPool(
        nh_core_ThreadPool *ThreadPool_p
    );
 
    bool nh_core_keepRunning(
    );

    void *nh_core_activateWorkload(
        void *(*init_f)(nh_core_Workload*), NH_SIGNAL (*run_f)(void*), void (*free_f)(void*), 
        NH_SIGNAL (*runCommand_f)(void*, nh_core_WorkloadCommand*), void *args_p, bool crucial
    );
    
    NH_API_RESULT nh_core_deactivateWorkload(
        nh_core_Workload *Workload_p 
    );

    /**
     * @param byteSize Specify 0 for no allocation.
     */
    NH_API_RESULT nh_core_executeWorkloadCommand(
        void *handle_p, int type, void *p, int byteSize
    );

    NH_API_RESULT nh_core_executeWorkload(
        void *handle_p
    );

    int nh_core_activeWorkloads(
        bool onlyCrucial
    );
    
    void *nh_core_getWorkloadArg(
    );

    nh_core_Workload *nh_core_getWorkload(
        void *args_p
    );

    nh_core_Workload *nh_core_getWorkloads(
    );

    int nh_core_activeThreads(
    );

    nh_core_Thread *nh_core_getThread(
    );
    
    nh_core_Thread *nh_core_getThreadFromArgs(
        void *args_p
    );

    int nh_core_getThreadIndex(
    );

    NH_API_RESULT nh_sleepMs(
        int milliseconds
    );

    void nh_core_waitForCompletion(
        nh_core_Workload *Workload_p, NH_SIGNAL signal
    );

/** @} */

#endif 
