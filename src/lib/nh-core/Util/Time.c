// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Time.h"

#include <string.h>
#include <time.h>
#include <stdarg.h>

// FUNCTIONS =======================================================================================

nh_GeneralTime nh_humanizeSeconds(
    long seconds)
{
    nh_GeneralTime Time;
    Time.weeks   = seconds / 604800; 
    Time.days    = (seconds / 86400) % 7; 
    Time.hours   = (seconds / 3600) % 24;
    Time.minutes = (seconds / 60) % 60;
    Time.seconds = seconds % 60;

    return Time;
}

nh_LocalTime nh_core_getLocalTime()
{
    nh_LocalTime Time;

#ifdef __unix__ 	
    time_t t = time(NULL);
    struct tm *p = localtime(&t);
    Time.seconds = p->tm_sec;  
    Time.minutes = p->tm_min;  
    Time.hours   = p->tm_hour; 
    Time.days    = p->tm_mday;
    Time.months  = p->tm_mon + 1;  
    Time.years   = p->tm_year + 1900; 
    Time.weekday = p->tm_wday;
#elif defined(_WIN32) || defined(WIN32)
    SYSTEMTIME WTime;
    GetLocalTime(&WTime);
    // TODO
#endif

    return Time;
}

nh_core_SystemTime nh_core_getSystemTime()
{
    nh_core_SystemTime Time;

#ifdef __unix__ 	
    struct timeval LTime;
    gettimeofday(&LTime, NULL);
    Time.seconds      = LTime.tv_sec;  
    Time.milliseconds = LTime.tv_usec / 1000;  
    Time.microseconds = LTime.tv_usec;  
#elif defined(_WIN32) || defined(WIN32)
    SYSTEMTIME WTime;
    GetSystemTime(&WTime);
#endif

    return Time;
}

double nh_core_getSystemTimeDiffInSeconds(
    nh_core_SystemTime Time1, nh_core_SystemTime Time2)
{
    double diff;
#ifdef __unix__
    diff  = (double) (Time2.microseconds - Time1.microseconds) / 1000000 +
            (double) (Time2.seconds - Time1.seconds);
#elif defined(_WIN32) || defined(WIN32)
    diff = (double) (Time2.wSecond - Time1.wSecond) +
           (double) ((Time2.wMilliseconds - Time1.wMilliseconds) / 1000.0);
#endif

    return diff;
}
