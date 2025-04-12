// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Thread.h"
#include "Process.h"

#include "../Util/Time.h"
#include "../System/Memory.h"
#include "../Common/Log.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#if defined(__unix__) || defined(__APPLE__)
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/time.h>
#elif defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

// INIT ============================================================================================

static inline void nh_core_initThread(
    nh_core_Thread *Thread_p)
{
    Thread_p->CurrentWorkload_p = NULL;
    Thread_p->depth = 0;
    Thread_p->id = 0;

    return;
}

static inline void nh_core_initWorkload(
    nh_core_Workload *Workload_p, bool firstTime)
{
    Workload_p->signal  = NH_SIGNAL_INACTIVE;
    Workload_p->crucial = false;

    Workload_p->SignalCounter.idle = 0;
    Workload_p->SignalCounter.ok   = 0;

    Workload_p->module   = 0;
    Workload_p->name_p   = NULL;
    Workload_p->path_p   = NULL;
    Workload_p->args_p   = NULL;
    Workload_p->Thread_p = NULL;

    Workload_p->init_f = NULL;
    Workload_p->run_f  = NULL;
    Workload_p->free_f = NULL;
    Workload_p->runCommand_f = NULL;

    Workload_p->Timing.idleDelay  = 0;
    Workload_p->Timing.runDelay   = 0;
    Workload_p->Timing.haltDelay  = 0;
    Workload_p->Timing.waitDelay  = 0;
    Workload_p->Timing.turnAround = 0.0;

    if (!firstTime) {nh_core_freeRingBuffer(&Workload_p->Commands);}

    nh_core_initRingBuffer(&Workload_p->Commands, 64, sizeof(nh_core_WorkloadCommand), NULL);
    nh_core_initRingBufferMarker(&Workload_p->Marker);

    return;
}

nh_core_ThreadPool nh_core_initThreadPool()
{
    nh_core_ThreadPool ThreadPool;
    ThreadPool.threadCount = 0;

    nh_core_initThread(&ThreadPool.Main);

    for (int i = 0; i < NH_MAX_THREADS; ++i) {
        nh_core_initThread(&ThreadPool.Threads_p[i]);
    }
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        nh_core_initWorkload(&ThreadPool.Workloads_p[i], true);
    }

    return ThreadPool;
}

NH_API_RESULT nh_core_freeThreadPool(
    nh_core_ThreadPool *ThreadPool_p)
{
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        nh_core_deactivateWorkload(&ThreadPool_p->Workloads_p[i]);
        nh_core_freeRingBuffer(&ThreadPool_p->Workloads_p[i].Commands);
    }

    return NH_API_SUCCESS;
}

// RUN =============================================================================================

static NH_API_RESULT nh_core_runWorkloadLoop(
    nh_core_Workload *Workload_p, bool *idle_p)
{
    nh_core_SystemTime TAT = nh_core_getSystemTime();

    // Run command if any.
    nh_core_WorkloadCommand *Command_p = NULL;
    while (Command_p = nh_core_incrementRingBufferMarker(&Workload_p->Commands, &Workload_p->Marker)) {
        if (Command_p->dummy) {break;}
        if (Workload_p->runCommand_f) {
            Workload_p->runCommand_f(Workload_p->args_p, Command_p);
        }
        Command_p->done = true;
        return NH_API_SUCCESS;
    }

    // Very important line. This runs the encapsulated program loop.
    Workload_p->signal = Workload_p->run_f(Workload_p->args_p);

    if (Command_p) {
        // This should only get triggered in case of a dummy command, used to wait for a workload loop.
        Command_p->done = true;
    }

    if (Workload_p->signal != NH_SIGNAL_IDLE && idle_p) {*idle_p = false;}

    switch (Workload_p->signal)
    {
        case NH_SIGNAL_OK    : 
            Workload_p->SignalCounter.ok++;
            break;
        case NH_SIGNAL_IDLE  : 
            Workload_p->SignalCounter.idle++;
            break;
        case NH_SIGNAL_FINISH : 
            nh_core_deactivateWorkload(Workload_p);
            break;
        case NH_SIGNAL_ERROR  : 
            return NH_API_ERROR_BAD_STATE;
    }

    Workload_p->Timing.turnAround = nh_core_getSystemTimeDiffInSeconds(TAT, nh_core_getSystemTime());
    
    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_runWorkload(
    nh_core_Workload *Workload_p, bool *idle_p)
{
    nh_core_Thread *Thread_p = nh_core_getThread();
    NH_CORE_CHECK_NULL(Thread_p)

    Thread_p->CurrentWorkload_p = Workload_p;

    if (Workload_p->signal == NH_SIGNAL_INIT) 
    {
        if (Workload_p->init_f != NULL) {
            Workload_p->args_p = Workload_p->init_f(Workload_p);
        }
        Workload_p->signal = NH_SIGNAL_DONE;
    }
    else if (Workload_p->signal == NH_SIGNAL_FREE) 
    {
        if (Workload_p->free_f != NULL) {
            Workload_p->free_f(Workload_p->args_p);
        }
        nh_core_initWorkload(Workload_p, false);
        Workload_p->signal = NH_SIGNAL_INACTIVE;
    }
    else {NH_CORE_CHECK(nh_core_runWorkloadLoop(Workload_p, idle_p))}

    Thread_p->CurrentWorkload_p = NULL;

    return NH_API_SUCCESS;
}

int nh_core_run()
{
    nh_core_ThreadPool *ThreadPool_p = &NH_PROCESS_POOL.Main.ThreadPool;

    bool idle = true;
    unsigned int count = 0;
    nh_core_Thread *Thread_p = nh_core_getThread();

    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) 
    {
        nh_core_Workload *Workload_p = &ThreadPool_p->Workloads_p[i];

        if (Workload_p->Thread_p == Thread_p) 
        {
            NH_API_RESULT result = nh_core_runWorkload(Workload_p, &idle);
            if (result != NH_API_SUCCESS) {return -1;}
            count++;
        }
    }

//    nh_core_logThread(ThreadPool_p, Thread_p);

    if (idle) {nh_sleepMs(10);}

    return count;
}

//static void nh_core_runThread(
//    void *index_p)
//{
//    nh_core_Thread *Thread_p = &ThreadPool.Threads_p[*((int*)index_p)];
//
//#ifdef __unix__
//    Thread_p->id = pthread_self();
//#elif defined(_WIN32) || defined (WIN32)
//    Thread_p->id = GetCurrentThreadId();
//#endif
//
//    nh_core_initializeThreadMemory();
//
//    bool run = true;
//    while (run) {run = nh_core_runWorkloads();}
//
//#ifdef __unix__
//
//    pthread_exit(NULL);
//
//#endif
//
//return ;
//}

static void nh_core_startThread(
    int index)
{
#ifdef __unix__
    // TODO
#elif defined(_WIN32) || defined (WIN32)
//        _beginthread(nh_core_runPhysicalThread, 0, (void*)Data_p);
#endif
    return;
}

// KEEP RUNNING? ===================================================================================

bool nh_core_keepRunning()
{
    nh_core_checkForks();
    return nh_core_activeWorkloads(true) > 0 || nh_core_activeForks() > 0 ? true : false;
}

// GET =============================================================================================

nh_core_Workload *nh_core_getWorkloads()
{
    return NH_PROCESS_POOL.Main.ThreadPool.Workloads_p;
}

nh_core_Thread *nh_core_getThread()
{
    nh_core_ThreadPool *ThreadPool_p = &NH_PROCESS_POOL.Main.ThreadPool;

    for (int i = 0; i < NH_MAX_THREADS; ++i) 
    {
#if defined(__unix__) || defined(__APPLE__)
        if (ThreadPool_p->Threads_p[i].id == pthread_self())
#elif defined(_WIN32) || defined (WIN32)
        if (ThreadPool_p->Threads_p[i].id == GetCurrentThreadId())
#endif
        {
            return &ThreadPool_p->Threads_p[i];
        }
    }

    return &(ThreadPool_p->Main);
}

nh_core_Workload *nh_core_getWorkload(
    void *args_p)
{
    if (args_p == NULL) {return NULL;}

    nh_core_ThreadPool *ThreadPool_p = &NH_PROCESS_POOL.Main.ThreadPool;

    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (args_p == ThreadPool_p->Workloads_p[i].args_p) {
            return &ThreadPool_p->Workloads_p[i];
        }
    }

    return NULL;
}

nh_core_Thread *nh_core_getThreadFromArgs(
    void *args_p)
{
    if (args_p == NULL) {return NULL;}

    nh_core_ThreadPool *ThreadPool_p = &NH_PROCESS_POOL.Main.ThreadPool;

    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (args_p == ThreadPool_p->Workloads_p[i].args_p) {
            return ThreadPool_p->Workloads_p[i].Thread_p;
        }
    }

    return NULL;
}

int nh_core_getThreadIndex()
{
    nh_core_ThreadPool *ThreadPool_p = &NH_PROCESS_POOL.Main.ThreadPool;

    nh_core_Thread *Thread_p = nh_core_getThread();
    for (int i = 0; i < NH_MAX_THREADS; ++i) {
        if (Thread_p == &ThreadPool_p->Threads_p[i]) {return i;}
    }

    return 0;
}

// DE/ACTIVATE =====================================================================================

static void nh_core_assignToThread(
    nh_core_Workload *Workload_p)
{
    if (NH_PROCESS_POOL.Main.ThreadPool.threadCount == 0) {
        Workload_p->Thread_p = &(NH_PROCESS_POOL.Main.ThreadPool.Main);
    } else {
        puts("TODO nh_core_assignToThread");
        exit(0);
    }

    return;
}

void nh_core_waitForCompletion(
    nh_core_Workload *Workload_p, NH_SIGNAL signal)
{
    // If the thread that handles the workload is executing this code, 
    // we can just execute the function directly.
    if (Workload_p->Thread_p == nh_core_getThread()) {
        nh_core_runWorkload(Workload_p, NULL);
    }

    // Otherwise we need to wait for the executing thread to do its work.
    else {while (Workload_p->signal != signal) {}}

    return;
}

void *nh_core_activateWorkload(
    void *(*init_f)(nh_core_Workload*), NH_SIGNAL (*run_f)(void*), void (*free_f)(void*), 
    NH_SIGNAL (*runCommand_f)(void*, nh_core_WorkloadCommand*), void *args_p, bool crucial)
{
    nh_core_Workload *Workload_p = NULL;

    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (NH_PROCESS_POOL.Main.ThreadPool.Workloads_p[i].signal == NH_SIGNAL_INACTIVE) {
            Workload_p = &NH_PROCESS_POOL.Main.ThreadPool.Workloads_p[i]; 
            break;
        }
    }

    if (Workload_p == NULL) {return NULL;}

    Workload_p->signal       = NH_SIGNAL_INIT;
    Workload_p->args_p       = args_p;
    Workload_p->init_f       = init_f;
    Workload_p->run_f        = run_f;
    Workload_p->free_f       = free_f;
    Workload_p->runCommand_f = runCommand_f;
    Workload_p->crucial      = crucial;

    nh_core_assignToThread(Workload_p);

    // Wait for completion of init function, if any.
    nh_core_waitForCompletion(Workload_p, NH_SIGNAL_DONE);

    return Workload_p->args_p;
}

NH_API_RESULT nh_core_deactivateWorkload(
    nh_core_Workload *Workload_p)
{
    // Check if already deactivated.
    if (Workload_p->signal == NH_SIGNAL_INACTIVE) {return NH_API_SUCCESS;}

    Workload_p->signal = NH_SIGNAL_FREE;
    nh_core_waitForCompletion(Workload_p, NH_SIGNAL_INACTIVE);

    return NH_API_SUCCESS;
}

void *nh_core_getWorkloadArg()
{
    return nh_core_getThread()->CurrentWorkload_p->args_p;
}

// EXECUTE =========================================================================================

NH_API_RESULT nh_core_executeWorkloadCommand(
    void *handle_p, int type, void *p, int byteSize)
{
    nh_core_Workload *Workload_p = nh_core_getWorkload(handle_p);
    NH_CORE_CHECK_NULL(Workload_p)

    nh_core_WorkloadCommand *Command_p = nh_core_advanceRingBuffer(&Workload_p->Commands);
    NH_CORE_CHECK_NULL(Command_p)

    Command_p->done = false;
    Command_p->dummy = false;
    Command_p->type = type;
    Command_p->p = p;
    Command_p->length = byteSize;

    if (byteSize > 0) {
        Command_p->p = nh_core_allocate(byteSize+1);
        memset(Command_p->p, 0, byteSize+1);
        memcpy(Command_p->p, p, byteSize);
    }

    // If the thread that handles the workload is executing this code,
    // we can just execute the function directly. But we need to remember
    // the previous workload and reset the pointer.
    if (Workload_p->Thread_p == nh_core_getThread()) {
        while (Command_p->done == false) {
            nh_core_Workload *PreviousWorkload_p = nh_core_getThread()->CurrentWorkload_p;
            NH_API_RESULT result = nh_core_runWorkload(Workload_p, NULL);
            nh_core_getThread()->CurrentWorkload_p = PreviousWorkload_p;
            NH_CORE_CHECK(result)
        }
    }

    // Otherwise we need to wait for the executing thread to do its work.
    // TODO Sleep.
    else {while (Command_p->done == false) {}}

    if (byteSize > 0) {nh_core_free(Command_p->p);}

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_executeWorkload(
    void *handle_p)
{
    nh_core_Workload *Workload_p = nh_core_getWorkload(handle_p);
    NH_CORE_CHECK_NULL(Workload_p)

    nh_core_WorkloadCommand *Command_p = nh_core_advanceRingBuffer(&Workload_p->Commands);
    NH_CORE_CHECK_NULL(Command_p)

    Command_p->done = false;
    Command_p->dummy = true;

    // If the thread that handles the workload is executing this code,
    // we can just execute the function directly. But we need to remember
    // the previous workload and reset the pointer.
    if (Workload_p->Thread_p == nh_core_getThread()) {
        nh_core_Workload *PreviousWorkload_p = nh_core_getThread()->CurrentWorkload_p;
        NH_API_RESULT result = nh_core_runWorkload(Workload_p, NULL);
        nh_core_getThread()->CurrentWorkload_p = PreviousWorkload_p;
        NH_CORE_CHECK(result)
    }

    // Otherwise we need to wait for the executing thread to do its work.
    // TODO Sleep.
    else {while (Command_p->done == false) {}}

    return NH_API_SUCCESS;
}

// COUNT ==========================================================================================

int nh_core_activeThreads()
{
    nh_core_ThreadPool *ThreadPool_p = &NH_PROCESS_POOL.Main.ThreadPool;

    int count = 0;
    for (int i = 0; i < NH_MAX_THREADS; ++i) {
        if (ThreadPool_p->Threads_p[i].id != 0) {count++;}
    }

    return count;
}

int nh_core_activeWorkloads(
    bool onlyCrucial)
{
    nh_core_ThreadPool *ThreadPool_p = &NH_PROCESS_POOL.Main.ThreadPool;

    int count = 0;
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (ThreadPool_p->Workloads_p[i].signal != NH_SIGNAL_INACTIVE) {
            if (onlyCrucial && !ThreadPool_p->Workloads_p[i].crucial) {continue;}
            count++;
        }
    }

    return count;
}

// SLEEP ===========================================================================================

NH_API_RESULT nh_sleepMs(
    int milliseconds)
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
    return NH_API_SUCCESS;
}
