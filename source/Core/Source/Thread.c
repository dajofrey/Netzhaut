// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Thread.h"
#include "../Header/Window.h"
#include "../Header/Tab.h"
#include "../Header/Memory.h"
#include "../Header/Tab.h"
#include "../Header/Macros.h"
#include "../Header/String.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>
#include <sys/types.h>

#ifdef __unix__
    #include <sys/sysinfo.h>
    #include <pthread.h>
#elif defined(_WIN32) || defined (WIN32)
    #include <windows.h>
#endif

// DATA ============================================================================================

Nh_ThreadPool *ThreadPool_p;

const char *threadTypes_pp[] = 
{
    "UNDEFINED",
    "CORE",
    "WINDOW",
    "TAB",               
};

// DECLARE =========================================================================================

typedef struct ThreadData {
    NH_RESULT *result_p;
    Nh_Thread *Thread_p;
    NH_THREAD type;
    NH_RESULT (*function_p)(void*);
    void *args_p;
} ThreadData;

static void Nh_runPhysicalThread(
    void *Args_p
);
static NH_RESULT Nh_initVirtualThread(
    NH_THREAD type, void *Args_p, NH_RESULT (*function_p)(void*)
);

static inline void Nh_initThread(
    int id
);
static inline Nh_Thread *Nh_getInactiveThread(
);

// THREAD POOL =====================================================================================

NH_RESULT Nh_createThreadPool()
{
NH_BEGIN()

    Nh_ThreadPool *ThreadPool_p = Nh_allocate(sizeof(Nh_ThreadPool));
    NH_CHECK_MEM(ThreadPool_p)

    Nh_setThreadPool(ThreadPool_p);

#ifdef __unix__
    ThreadPool_p->maxKernelThreads = 1;
//    ThreadPool_p->maxKernelThreads = get_nprocs_conf();
#elif defined(_WIN32) || defined (WIN32)
    ThreadPool_p->maxKernelThreads = 1;
#endif

    ThreadPool_p->activeThreads = 0;
    ThreadPool_p->activeKernelThreads = 0;
    ThreadPool_p->totalThreads = 64;

#ifdef __unix__
    ThreadPool_p->Pool = thpool_init(ThreadPool_p->maxKernelThreads);
    NH_CHECK_NULL(ThreadPool_p->Pool)
#elif defined(_WIN32) || defined (WIN32)

#endif

    ThreadPool_p->Threads_p = Nh_allocate(sizeof(Nh_Thread) * ThreadPool_p->totalThreads);
    NH_CHECK_MEM(ThreadPool_p->Threads_p)
    
    for (int i = 0; i < ThreadPool_p->totalThreads; ++i) {Nh_initThread(i);}

NH_END(NH_SUCCESS)
}

void Nh_freeThreadPool()
{
NH_BEGIN()

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();

    for (int i = 0; i < ThreadPool_p->totalThreads; ++i) {
        if (ThreadPool_p->Threads_p[i].Info.active) {Nh_free(ThreadPool_p->Threads_p[i].Data_p);}
    }

#ifdef __unix__
    thpool_destroy(ThreadPool_p->Pool);
#elif defined(_WIN32) || defined (WIN32)

#endif

    Nh_free(ThreadPool_p->Threads_p);
    Nh_free(ThreadPool_p);

    Nh_setThreadPool(NULL);

NH_SILENT_END()
}

void Nh_setThreadPool(
    Nh_ThreadPool *Pool_p)
{
NH_BEGIN()

    ThreadPool_p = Pool_p;

NH_SILENT_END()
}

Nh_ThreadPool *Nh_getThreadPool()
{
NH_BEGIN()
NH_END(ThreadPool_p)
}

// START ===========================================================================================

Nh_Thread *Nh_startThread(
    NH_THREAD type, NH_RESULT (*function_p)(void*), void *args_p)
{
NH_BEGIN()

#define PHYSICAL_THREAD() (ThreadPool_p->activeKernelThreads < ThreadPool_p->maxKernelThreads)

#include NH_CUSTOM_CHECK

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    NH_CHECK_NULL(NULL, ThreadPool_p)

    ThreadData *Data_p  = NULL;
    Nh_Thread *Thread_p = NULL;
    NH_RESULT result    = NH_PENDING;

    if (PHYSICAL_THREAD())
    {
        Data_p = Nh_allocate(sizeof(ThreadData));
        NH_CHECK_NULL(NULL, Data_p)

#include NH_DEFAULT_CHECK

        Data_p->result_p   = &result;
        Data_p->type       = type;
        Data_p->function_p = (NH_RESULT(*)(void*))function_p;
        Data_p->args_p     = args_p;
        Data_p->Thread_p   = NULL;

        // start thread
        #ifdef __unix__
            thpool_add_work(
                (threadpool)((Nh_ThreadPool*)ThreadPool_p)->Pool, Nh_runPhysicalThread, (void*)Data_p
            );
        #elif defined(_WIN32) || defined (WIN32)
            _beginthread(Nh_runPhysicalThread, 0, (void*)Data_p);
        #endif

        while (*Data_p->result_p == NH_PENDING) {}

        if (*Data_p->result_p != NH_SUCCESS) {NH_END(NULL)}
        Thread_p = Data_p->Thread_p;
    } 
    else {
        NH_RESULT result = Nh_initVirtualThread(type, args_p, function_p);

        if (result != NH_SUCCESS) {
            Nh_deactivateThread(Nh_getThread());
            Nh_switchThread(NULL);
            Nh_free(args_p);
            NH_END(NULL)
        }

        Thread_p = Nh_getThread();
        Nh_switchThread(NULL);
        Nh_free(args_p);
    }

NH_END(Thread_p)
}

// ACTIVATE ========================================================================================

NH_RESULT Nh_activateThread(
    NH_THREAD type, bool isVirtual)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_Thread *Thread_p = Nh_getInactiveThread();
    NH_CHECK_NULL(NH_ERROR_NOT_ENOUGH_THREADS, Thread_p)

#include NH_DEFAULT_CHECK

    Thread_p->type           = type;
    Thread_p->Flags.run      = true;
    Thread_p->Info.isVirtual = isVirtual;
    Thread_p->Info.owner     = -1;
    Thread_p->Info.current   = -1;
    Thread_p->Info.atomic    = false;

    if (isVirtual) 
    {
        Nh_switchThread(Thread_p);
        Thread_p->Info.owner = Nh_getKernelThread()->id;
    }

#ifdef __unix__
    if (isVirtual) {Thread_p->unixId = -1;}
    else {Thread_p->unixId = pthread_self();}
#elif defined(_WIN32) || defined (WIN32)
    if (isVirtual) {Thread_p->windowsId = -1;}
    else {Thread_p->windowsId = GetCurrentThreadId();}
#endif

    NH_CHECK(Nh_initializeThreadMemory())
     
    switch (type)
    {
        case NH_THREAD_CORE :

            Thread_p->Data_p = NULL;
            break;

        case NH_THREAD_WINDOW :
            
            Thread_p->Data_p = Nh_allocate(sizeof(Nh_Window));
            NH_CHECK_MEM(Thread_p->Data_p)
            break;

        case NH_THREAD_TAB :

            Thread_p->Data_p = Nh_allocate(sizeof(Nh_Tab));
            NH_CHECK_MEM(Thread_p->Data_p)
            break;
    }

    Thread_p->Info.active = true;
    Nh_getThreadPool()->activeThreads += 1;
    if (!isVirtual) {Nh_getThreadPool()->activeKernelThreads += 1;}

NH_END(NH_SUCCESS)
}

// DEACTIVATE ======================================================================================

NH_RESULT Nh_deactivateThread( // TODO handle hardware threads
    Nh_Thread *Thread_p)
{
NH_BEGIN()

    if (Thread_p == NULL) {
        Thread_p = Nh_getThread();
        NH_CHECK_NULL(Thread_p)
    }

    if (!Thread_p->Info.active) {NH_END(NH_SUCCESS)}

    Nh_free(Thread_p->Data_p);

    Nh_getThreadPool()->activeThreads -= 1;
    if (!Thread_p->Info.isVirtual) {Nh_getThreadPool()->activeKernelThreads -= 1;}

    Nh_initThread(Thread_p->id);

NH_END(NH_SUCCESS)
}

// VIRTUAL =========================================================================================

NH_RESULT Nh_runVirtualThreads()
{
NH_BEGIN()

    Nh_Thread *Thread_p = Nh_getThread();
    NH_CHECK_NULL(Thread_p)

    if (Thread_p->Info.isVirtual) {NH_END(NH_SUCCESS)}

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();

    for (int i = 0; i < ThreadPool_p->totalThreads; ++i) 
    {
        if (ThreadPool_p->Threads_p[i].Info.owner == Thread_p->id) 
        {
            Nh_switchThread(&ThreadPool_p->Threads_p[i]);

            switch (ThreadPool_p->Threads_p[i].type)
            {
                case NH_THREAD_WINDOW : NH_CHECK(Nh_runWindow()) break;
                case NH_THREAD_TAB    : NH_CHECK(Nh_runTab()) break;
            }

            Nh_switchThread(NULL);
        }
    }

NH_END(NH_SUCCESS)
}

bool Nh_isVirtualThread()
{
NH_BEGIN();

#include NH_CUSTOM_CHECK

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    NH_CHECK_NULL(false, ThreadPool_p)

#include NH_DEFAULT_CHECK

    for (int i = 0; i < ThreadPool_p->totalThreads; ++i) 
    {
        if (ThreadPool_p->Threads_p[i].type == NH_THREAD_UNDEFINED) {continue;}

#ifdef __unix__
        if (ThreadPool_p->Threads_p[i].unixId == pthread_self())
#elif defined(_WIN32) || defined (WIN32)
        if (ThreadPool_p->Threads_p[i].windowsId == GetCurrentThreadId())
#endif
        {
            NH_END(ThreadPool_p->Threads_p[i].Info.current != -1)
        }
    }

NH_END(true); // TODO bad state
}

// SWITCH ==========================================================================================

int Nh_switchThread(
    Nh_Thread *Thread_p)
{
NH_BEGIN()

    Nh_Thread *Kernel_p = Nh_getKernelThread();
    int old = Kernel_p->Info.current;
    Kernel_p->Info.current = Thread_p == NULL ? -1 : Thread_p->id;

NH_END(old)
}

void Nh_switchThreadFromContext(
    int context)
{
NH_BEGIN()

    Nh_Thread *Kernel_p = Nh_getKernelThread();
    Kernel_p->Info.current = context;

NH_END()
}

// GET =============================================================================================

Nh_Thread *Nh_getThread()
{
NH_BEGIN();

#include NH_CUSTOM_CHECK

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    NH_CHECK_NULL(NULL, ThreadPool_p);

#include NH_DEFAULT_CHECK

    for (int i = 0; i < ThreadPool_p->totalThreads; ++i) 
    {
        if (ThreadPool_p->Threads_p[i].type == NH_THREAD_UNDEFINED) {continue;}

#ifdef __unix__
        if (ThreadPool_p->Threads_p[i].unixId == pthread_self())
#elif defined(_WIN32) || defined (WIN32)
        if (ThreadPool_p->Threads_p[i].windowsId == GetCurrentThreadId())
#endif
        {
            if (ThreadPool_p->Threads_p[i].Info.current != -1) {
                NH_END(&ThreadPool_p->Threads_p[ThreadPool_p->Threads_p[i].Info.current])
            }
            NH_END(&ThreadPool_p->Threads_p[i])
        }
    }

NH_END(NULL);
}

Nh_Thread *Nh_getKernelThread()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    NH_CHECK_NULL(NULL, ThreadPool_p)

#include NH_DEFAULT_CHECK

    for (int i = 0; i < ThreadPool_p->totalThreads; ++i) 
    {
        if (ThreadPool_p->Threads_p[i].type == NH_THREAD_UNDEFINED) {continue;}

#ifdef __unix__
        if (ThreadPool_p->Threads_p[i].unixId == pthread_self())
#elif defined(_WIN32) || defined (WIN32)
        if (ThreadPool_p->Threads_p[i].windowsId == GetCurrentThreadId())
#endif
        {
            NH_END(&ThreadPool_p->Threads_p[i]);
        }
    }

NH_END(NULL)
}

Nh_Thread *Nh_getCoreThread()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    NH_CHECK_NULL(NULL, ThreadPool_p);

#include NH_DEFAULT_CHECK

NH_END(&ThreadPool_p->Threads_p[0])
}

Nh_Thread *Nh_getThreadFromData(
    void *Data_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    NH_CHECK_NULL(NULL, ThreadPool_p);

#include NH_DEFAULT_CHECK

    for (int i = 0; i < ThreadPool_p->totalThreads; ++i) {
        if (ThreadPool_p->Threads_p[i].Data_p == Data_p) {NH_END(&ThreadPool_p->Threads_p[i])}
    }
   
NH_END(NULL)
}

const char *Nh_stringifyThreadType(
    NH_THREAD type)
{
    return threadTypes_pp[type]; 
}

// SLEEP ===========================================================================================

NH_RESULT NH_sleepMs(
    int milliseconds)
{
NH_BEGIN()

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

NH_END(NH_SUCCESS)
}

// HELPER ==========================================================================================

static NH_RESULT Nh_initVirtualThread(
    NH_THREAD type, void *Args_p, NH_RESULT (*function_p)(void*))
{
NH_BEGIN()

    NH_CHECK(Nh_activateThread(type, true))
    NH_CHECK(function_p(Args_p))

NH_END(NH_SUCCESS)
}

static void Nh_runPhysicalThread(
    void *Args_p)
{
NH_BEGIN()

#define CLEAN_UP()                                         \
    if (Data_p->args_p != NULL) {Nh_free(Data_p->args_p);} \
    Nh_free(Data_p); 

    ThreadData *Data_p = Args_p;
    Nh_Thread *Thread_p = Data_p->Thread_p;

    *Data_p->result_p = Nh_activateThread(Data_p->type, false);
    if (*Data_p->result_p != NH_SUCCESS) {CLEAN_UP() NH_SILENT_END()}

    Data_p->Thread_p = Nh_getThread();
    Data_p->result_p = &Thread_p->result;

    *Data_p->result_p = Data_p->function_p(Data_p->args_p);

    Nh_deactivateThread(Thread_p);

    CLEAN_UP()

#ifdef __unix__

    pthread_exit(NULL);

#endif

NH_SILENT_END()
}

static inline void Nh_initThread(
    int id)
{
NH_BEGIN()

    Nh_Thread *Thread_p = &Nh_getThreadPool()->Threads_p[id];

    Thread_p->id     = id;
    Thread_p->type   = NH_THREAD_UNDEFINED;
    Thread_p->result = NH_SUCCESS;

    Thread_p->Info.active    = false;
    Thread_p->Info.isVirtual = true;
    Thread_p->Info.owner     = -1;

    Thread_p->Flags.run = false;

    Thread_p->Timing.idleDelay = 200;
    Thread_p->Timing.runDelay  = 5;
    Thread_p->Timing.haltDelay = 10;
    Thread_p->Timing.waitDelay = 10;
    Thread_p->Timing.turnAround = 0.0;

NH_SILENT_END()
}

static inline Nh_Thread *Nh_getInactiveThread()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    NH_CHECK_NULL(NULL, ThreadPool_p)

#include NH_DEFAULT_CHECK

    for (int i = 0; i < ThreadPool_p->totalThreads; ++i) {
        if (!ThreadPool_p->Threads_p[i].Info.active) {
            NH_END(&ThreadPool_p->Threads_p[i])
        }
    }

NH_END(NULL)
}

