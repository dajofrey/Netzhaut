#ifndef NH_TIME_H
#define NH_TIME_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"
    
#ifdef __unix__
#include <sys/time.h>
#elif defined(_WIN32) || defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#endif

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_Time {
    #ifdef __unix__ 	
        struct timeval time;
    #elif defined(_WIN32) || defined(WIN32)
        SYSTEMTIME time;
    #endif
    } Nh_Time;
    
/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    Nh_Time Nh_getTime(
    );
    
    double Nh_getTimeDiffInSeconds(
        Nh_Time Time1, Nh_Time Time2
    );

/** @} */

#endif 
