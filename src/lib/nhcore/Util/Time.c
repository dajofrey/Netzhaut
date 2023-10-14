// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Time.h"

#include "../Common/Macros.h"

#include <string.h>
#include <time.h>
#include <stdarg.h>

// HUMANIZE ========================================================================================

nh_GeneralTime nh_humanizeSeconds(
    long seconds)
{
NH_CORE_BEGIN()

    nh_GeneralTime Time;
    Time.weeks   = seconds / 604800; 
    Time.days    = (seconds / 86400) % 7; 
    Time.hours   = (seconds / 3600) % 24;
    Time.minutes = (seconds / 60) % 60;
    Time.seconds = seconds % 60;

NH_CORE_END(Time)
}

// TIME ============================================================================================

nh_LocalTime nh_core_getLocalTime()
{
NH_CORE_BEGIN()

    nh_LocalTime Time;

#ifdef __unix__ 	
    struct timeval LTime;
    gettimeofday(&LTime, NULL);
    struct tm *tm_p = localtime(&LTime.tv_sec);
    Time.seconds = tm_p->tm_sec;  
    Time.minutes = tm_p->tm_min;  
    Time.hours   = tm_p->tm_hour; 
    Time.days    = tm_p->tm_mday;
    Time.months  = tm_p->tm_mon;  
    Time.years   = tm_p->tm_year + 1900; 
    Time.weekday = tm_p->tm_wday;
#elif defined(_WIN32) || defined(WIN32)
    SYSTEMTIME WTime;
    GetLocalTime(&WTime);
    // TODO
#endif

NH_CORE_END(Time)
}

nh_SystemTime nh_core_getSystemTime()
{
NH_CORE_BEGIN()

    nh_SystemTime Time;

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

NH_CORE_END(Time)
}

double nh_core_getSystemTimeDiffInSeconds(
    nh_SystemTime Time1, nh_SystemTime Time2)
{
NH_CORE_BEGIN()

    double diff;
#ifdef __unix__
    diff  = (double) (Time2.microseconds - Time1.microseconds) / 1000000 +
            (double) (Time2.seconds - Time1.seconds);
#elif defined(_WIN32) || defined(WIN32)
    diff = (double) (Time2.wSecond - Time1.wSecond) +
           (double) ((Time2.wMilliseconds - Time1.wMilliseconds) / 1000.0);
#endif

NH_CORE_END(diff)
}

