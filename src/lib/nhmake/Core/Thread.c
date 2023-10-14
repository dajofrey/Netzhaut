// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Thread.h"

#include "../Common/Macros.h"

#include <string.h>
#include <stdio.h>

#if defined(__linux__)
    #include <sys/sysinfo.h>
#endif

#if defined(__linux__) || defined(__APPLE__)
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/time.h>
#elif defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

// DATA ============================================================================================

typedef struct nh_make_ThreadPool {
    nh_make_Thread Threads_p[NH_MAKE_MAX_THREADS];
    int threadCount;    
} nh_make_ThreadPool;

static nh_make_ThreadPool NH_MAKE_THREAD_POOL;

// INIT ============================================================================================

static inline void nh_make_initThread(
    nh_make_Thread *Thread_p)
{
NH_MAKE_BEGIN()

    Thread_p->depth = 0;
    Thread_p->id = 0;
    Thread_p->running = NH_MAKE_FALSE;
    Thread_p->Runtime_p = NULL;

NH_MAKE_SILENT_END()
}

void nh_make_initThreadPool()
{
NH_MAKE_BEGIN()

    NH_MAKE_THREAD_POOL.threadCount = 0;

    for (int i = 0; i < NH_MAKE_MAX_THREADS; ++i) {
        nh_make_initThread(&NH_MAKE_THREAD_POOL.Threads_p[i]);
    }

NH_MAKE_SILENT_END()
}

// RUN =============================================================================================

typedef struct nh_make_ThreadArguments {
    NH_BYTE **args_pp;
    int args;
    nh_make_Runtime *Runtime_p;
} nh_make_ThreadArguments;

static void *nh_make_executeThread(
    void *args_p)
{
NH_MAKE_BEGIN()

    nh_make_ThreadArguments *Arguments_p = args_p;

    NH_MAKE_RESULT result = nh_make_executeRuntime(Arguments_p->Runtime_p, Arguments_p->args_pp, Arguments_p->args);

    nh_make_Thread *Thread_p = _nh_make_getThread();
    Thread_p->running = NH_MAKE_FALSE;
    Thread_p->Runtime_p = NULL;

    free(args_p);

NH_MAKE_END((void*)result)
}

NH_MAKE_RESULT nh_make_runThread(
    nh_make_Runtime *Runtime_p, NH_BYTE **args_pp, int args)
{
NH_MAKE_BEGIN()

    nh_make_Thread *Thread_p = NULL;
    for (int i = 0; i < NH_MAKE_MAX_THREADS; ++i) {
        if (!NH_MAKE_THREAD_POOL.Threads_p[i].running) {
            Thread_p = &NH_MAKE_THREAD_POOL.Threads_p[i];
        }
    } 

    NH_MAKE_CHECK_NULL(Thread_p)

    nh_make_ThreadArguments *Arguments_p = malloc(sizeof(nh_make_ThreadArguments));
    NH_MAKE_CHECK_NULL(Arguments_p)

    Arguments_p->Runtime_p = Runtime_p;
    Arguments_p->args = args;
    Arguments_p->args_pp = args_pp;

    Thread_p->running = NH_MAKE_TRUE;
    Thread_p->Runtime_p = Runtime_p;

#if defined(__linux__) || defined(__APPLE__)

    pthread_create(&Thread_p->id, NULL, nh_make_executeThread, (void*)Arguments_p);

#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// COUNT ==========================================================================================

int nh_make_isRunning()
{
NH_MAKE_BEGIN()

    int count = 0;
    for (int i = 0; i < NH_MAKE_MAX_THREADS; ++i) {
        if (NH_MAKE_THREAD_POOL.Threads_p[i].running) {count++;}
    }

NH_MAKE_END(count)
}

// GET =============================================================================================

nh_make_Thread *_nh_make_getThread()
{
    for (int i = 0; i < NH_MAKE_MAX_THREADS; ++i) 
    {
#if defined(__linux__) || defined(__APPLE__)
        if (NH_MAKE_THREAD_POOL.Threads_p[i].id == pthread_self())
#elif defined(_WIN32) || defined (WIN32)
        if (NH_MAKE_THREAD_POOL.Threads_p[i].id == GetCurrentThreadId())
#endif
        {
            return &NH_MAKE_THREAD_POOL.Threads_p[i];
        }
    }

    return NULL;
}

// SLEEP ===========================================================================================

NH_MAKE_RESULT nh_make_sleepMs(
    int milliseconds)
{
NH_MAKE_BEGIN()

#if defined(WIN32)
    Sleep(milliseconds);
#elif defined(__linux__)
    #if _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000000;
        nanosleep(&ts, NULL);
    #else
        usleep(milliseconds * 1000);
    #endif
#elif defined(__APPLE__)
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

