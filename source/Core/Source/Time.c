// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Time.h"
#include "../Header/Macros.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>
#include <time.h>
#include <stdarg.h>

// TIME ============================================================================================

Nh_Time Nh_getTime()
{
NH_BEGIN()

    Nh_Time Time;
#ifdef __unix__ 	
    gettimeofday(&Time.time, NULL);
#elif defined(_WIN32) || defined(WIN32)
    GetSystemTime(&Time.time);
#endif

NH_END(Time)
}

double Nh_getTimeDiffInSeconds(
    Nh_Time Time1, Nh_Time Time2)
{
NH_BEGIN()

    double diff;
#ifdef __unix__
    diff  = (double) (Time2.time.tv_usec - Time1.time.tv_usec) / 1000000 +
            (double) (Time2.time.tv_sec - Time1.time.tv_sec);
#elif defined(_WIN32) || defined(WIN32)
    diff = (double) (Time2.time.wSecond - Time1.time.wSecond) +
           (double) ((Time2.time.wMilliseconds - Time1.time.wMilliseconds) / 1000.0);
#endif

NH_END(diff)
}

