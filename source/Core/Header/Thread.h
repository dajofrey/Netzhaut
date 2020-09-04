#ifndef NH_THREAD_H
#define NH_THREAD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "List.h"

#include "../../API/Header/Netzhaut.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
    
#ifdef __unix__
#include "External/thpool.h"
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

#endif

/** @addtogroup CoreEnums Enums
 *  \ingroup Core
 *  @{
 */

    typedef enum NH_THREAD {  
        NH_THREAD_UNDEFINED,  
        NH_THREAD_CORE,       
        NH_THREAD_WINDOW,     
        NH_THREAD_TAB, 
    } NH_THREAD;

/** @} */

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_Thread 
    {
        NH_THREAD type;                
        NH_RESULT result; 
        void *Data_p;                  
        int id;                         
    
    #ifdef __unix__
        pthread_t unixId;              
    #elif defined(_WIN32) || defined (WIN32)
        DWORD windowsId;              
    #endif

        struct {
            int idleDelay; 
            int runDelay;  
            int haltDelay; 
            int waitDelay; 
            double turnAround;
        } Timing;

        struct {
            bool run;
        } Flags;

        struct {
            bool isVirtual; 
            bool active;    
            int owner;     
            int current;   
            bool atomic;
        } Info;

        struct {
    #ifdef __unix__
            struct {
                int writeFD_p[2];
                int readFD_p[2];
            } TTY;
    #endif
        } IPC;

    } Nh_Thread;

    typedef struct Nh_ThreadPool 
    {
        int activeThreads;         
        int activeKernelThreads; 
        int maxKernelThreads;    
        int totalThreads;          
        Nh_Thread *Threads_p;      

    #ifdef __unix__
        threadpool Pool;           
    #elif defined(_WIN32) || defined(WIN32)
    #endif
    
    } Nh_ThreadPool;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    NH_RESULT Nh_createThreadPool(
    );
    void Nh_freeThreadPool(
    );

    void Nh_setThreadPool(
        Nh_ThreadPool *Pool_p
    );
    Nh_ThreadPool *Nh_getThreadPool(
    );
   
    Nh_Thread *Nh_startThread(
        NH_THREAD type, NH_RESULT (*function_p)(void*), void *args_p
    );
    
    NH_RESULT Nh_activateThread(
        NH_THREAD type, bool isVirtual
    );
    NH_RESULT Nh_deactivateThread(
        Nh_Thread *Thread_p
    );
    
    NH_RESULT Nh_runVirtualThreads(
    );
    bool Nh_isVirtualThread(
    );
   
    Nh_Thread* Nh_getThread(
    );
    Nh_Thread *Nh_getKernelThread(
    );
    Nh_Thread* Nh_getCoreThread(
    );
    Nh_Thread* Nh_getThreadFromData(
        void *Data_p
    );
    const char *Nh_stringifyThreadType(
        NH_THREAD type
    );

    int Nh_switchThread(
        Nh_Thread *Thread_p
    );
    void Nh_switchThreadFromContext(
        int context
    );

    NH_RESULT NH_sleepMs(
        int milliseconds
    );

/** @} */

#endif 
