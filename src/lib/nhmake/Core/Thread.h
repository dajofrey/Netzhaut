#ifndef NH_MAKE_THREAD_H
#define NH_MAKE_THREAD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Core/Runtime.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
    
#if defined(__linux__) || defined(__APPLE__)
#include <pthread.h>
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

#endif

/** @addtogroup lib_nhmake_macros
 *  @{
 */

    #define NH_MAKE_MAX_THREADS 64

/** @} */

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef struct nh_make_Thread {
        int depth;
        NH_MAKE_BOOL running;
        nh_make_Runtime *Runtime_p;
    #if defined(__linux__) || defined(__APPLE__)
        pthread_t id;              
    #elif defined(_WIN32) || defined (WIN32)
        DWORD id;              
    #endif
    } nh_make_Thread;

/** @} */

/** @addtogroup lib_nhmake_typedefs
 *  @{
 */

    typedef int (*nh_make_isRunning_f)(
    );

    typedef NH_MAKE_RESULT (*nh_make_sleepMs_f)(
        int milliseconds
    );

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    void nh_make_initThreadPool(
    );
    
    NH_MAKE_RESULT nh_make_runThread(
        nh_make_Runtime *Runtime_p, NH_BYTE **args_pp, int args
    );

    nh_make_Thread *_nh_make_getThread(
    );
    
/** @} */

#endif 
