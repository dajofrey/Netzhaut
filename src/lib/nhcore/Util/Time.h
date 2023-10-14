#ifndef NH_CORE_TIME_H
#define NH_CORE_TIME_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
    
#ifdef __unix__
    #include <sys/time.h>
#elif defined(_WIN32) || defined(WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#endif

/** @addtogroup lib_nhcore_structs
 *  @{
 */

    typedef struct nh_LocalTime {
        long weekday;
        long years;
        long months;
        long weeks;
        long days;
        long hours;
        long minutes;
        long seconds;
    } nh_LocalTime;

    typedef struct nh_GeneralTime {
        long weeks;
        long days;
        long hours;
        long minutes;
        long seconds;
    } nh_GeneralTime;

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    nh_GeneralTime nh_humanizeSeconds(
        long seconds
    );

    nh_SystemTime nh_core_getSystemTime(
    );

    nh_LocalTime nh_core_getLocalTime(
    );

    double nh_core_getSystemTimeDiffInSeconds(
        nh_SystemTime Time1, nh_SystemTime Time2
    );

/** @} */

#endif 
