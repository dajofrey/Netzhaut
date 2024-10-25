#ifndef NH_CORE_UTIL_TIME_H
#define NH_CORE_UTIL_TIME_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
    
#ifdef __unix__
    #include <sys/time.h>
#elif defined(_WIN32) || defined(WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

// STRUCTS =========================================================================================

typedef struct nh_core_SystemTime { 
    long seconds; 
    long milliseconds; 
    long microseconds; 
} nh_core_SystemTime; 

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

// FUNCTIONS =======================================================================================

nh_GeneralTime nh_humanizeSeconds(
    long seconds
);

nh_core_SystemTime nh_core_getSystemTime(
);

nh_LocalTime nh_core_getLocalTime(
);

double nh_core_getSystemTimeDiffInSeconds(
    nh_core_SystemTime Time1, nh_core_SystemTime Time2
);

#endif // NH_CORE_UTIL_TIME_H
