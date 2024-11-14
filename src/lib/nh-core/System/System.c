// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#ifdef __unix__
#define _GNU_SOURCE
#endif

#include "System.h"
#include "Memory.h"

#include "../Util/String.h"
#include "../Common/Log.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifdef __unix__
    #include <sys/sysinfo.h>
#endif

// DATA ============================================================================================

nh_System NH_SYSTEM;

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_core_getCPUAttr(
    char *src, char *dest) 
{
    if (src != NULL) {
        char delim[] = ":";
        strtok(src, delim);
        char *src2 = strtok(NULL, delim);
        char delim2[] = "\n";
        char *src3 = strtok(src2, delim2); 
        strcpy(dest, src3 + 1);
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_getCPUInfo()
{
    char name_p[255];
    int cores = 0;
    bool ht   = false;

#ifdef __unix__

    FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
    char *arg = 0;
    size_t size = 0;
    while(getdelim(&arg, &size, 0, cpuinfo) != -1) 
    {
        char arg2[size], arg3[size];
        strcpy(arg2, arg);
        strcpy(arg3, arg);
    
    // name
        char *ptr = strstr(arg, "model name");
        NH_CORE_CHECK(nh_core_getCPUAttr(ptr, name_p))

    // cores
        char *ptr2 = strstr(arg2, "cpu cores");
        char tmp[8];
        NH_CORE_CHECK(nh_core_getCPUAttr(ptr2, tmp))
        cores = atoi(tmp);

    // ht
        char *ptr3 = strstr(arg3, "flags");
        char flags[2048];
        
        NH_CORE_CHECK(nh_core_getCPUAttr(ptr3, flags))
            
        ht = strstr(flags, "ht") != NULL;
    }
    free(arg);
    fclose(cpuinfo);

#elif defined(_WIN32) || defined (WIN32)

#endif

    NH_SYSTEM.CPU.cores = cores;
    NH_SYSTEM.CPU.ht    = ht;
    strcpy(NH_SYSTEM.CPU.name_p, name_p);

    return NH_API_SUCCESS;
}

static bool nh_core_littleEndian()
{
    unsigned int x = 1;
    return (int)(((char *)&x)[0] == 1);
}

static NH_API_RESULT nh_core_getOtherInfo()
{
    unsigned long total = 0;
    unsigned long free  = 0;
    long uptime = 0;

#ifdef __unix__

    struct sysinfo info;
    sysinfo(&info);
    uptime = info.uptime;
    total  = info.totalram;
    free   = info.freeram;

#elif defined(_WIN32) || defined (WIN32)

#endif

    NH_SYSTEM.LocalTime    = nh_core_getLocalTime(); 
    NH_SYSTEM.uptime       = uptime;
    NH_SYSTEM.littleEndian = nh_core_littleEndian();
    NH_SYSTEM.RAM.total    = total;
    NH_SYSTEM.RAM.free     = free;

    return NH_API_SUCCESS;
}

static void nh_core_logSystem()
{
    nh_DataStorageUnits Total = nh_humanizeBytes(NH_SYSTEM.RAM.total);
    nh_DataStorageUnits Free = nh_humanizeBytes(NH_SYSTEM.RAM.free);
    nh_GeneralTime Uptime = nh_humanizeSeconds(NH_SYSTEM.uptime);

    char log_p[255] = {'\0'};
    sprintf(log_p, "LOCAL TIME %lu-%lu-%lu %lu:%lu:%lu", 
        NH_SYSTEM.LocalTime.years, NH_SYSTEM.LocalTime.months, NH_SYSTEM.LocalTime.days, NH_SYSTEM.LocalTime.hours, NH_SYSTEM.LocalTime.minutes, NH_SYSTEM.LocalTime.seconds
    );
    nh_core_logSystemInfo(log_p, 2);

    memset(log_p, 0, 255); 
    sprintf(log_p, "UPTIME     %lu weeks, %lu days, %lu hours, %lu minutes, %lu seconds", 
        Uptime.weeks, Uptime.days, Uptime.hours, Uptime.minutes, Uptime.seconds
    );
    nh_core_logSystemInfo(log_p, 3);

    memset(log_p, 0, 255); 
    sprintf(log_p, "RAM        total:%lugb %lumb, free:%lugb %lumb", 
        Total.gigabytes, Total.megabytes, Free.gigabytes, Free.megabytes
    );
    nh_core_logSystemInfo(log_p, 4);

    return;
}

static NH_API_RESULT nh_core_updateSystem()
{
    NH_CORE_CHECK(nh_core_getCPUInfo())
    NH_CORE_CHECK(nh_core_getOtherInfo())

    nh_core_logSystem();

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_initSystem()
{
    NH_CORE_CHECK(nh_core_updateSystem())

    NH_SYSTEM.Updater.updateIntervalInSeconds = 1;
    NH_SYSTEM.Updater.LastUpdate = nh_core_getSystemTime();

    memset(NH_SYSTEM.hostname_p, 0, NH_HOSTNAME_LENGTH);
#ifdef __unix__
    gethostname(NH_SYSTEM.hostname_p, NH_HOSTNAME_LENGTH);
#endif

    char log_p[255] = {'\0'};
    sprintf(log_p, "HOST NAME  %s", NH_SYSTEM.hostname_p);
    nh_core_logSystemInfo(log_p, 0);

    memset(log_p, 0, 255); 
    sprintf(log_p, NH_SYSTEM.littleEndian ? "ENDIANNESS Little-Endian (LE)" : "ENDIANNESS Big-Endian (BE)");
    nh_core_logSystemInfo(log_p, 1);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_freeSystem()
{
    return NH_API_SUCCESS;
}

static void *nh_core_initSystemWorkload(
    nh_core_Workload *Workload_p)
{
    static char *name_p = "System Workload";
    static char *path_p = "nh-core/System/System.c";

    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_CORE;

    return NH_SIGNAL_OK;
}

static NH_SIGNAL nh_core_runSystemWorkload(
    void *args_p)
{
    if (nh_core_getSystemTimeDiffInSeconds(NH_SYSTEM.Updater.LastUpdate, nh_core_getSystemTime()) < NH_SYSTEM.Updater.updateIntervalInSeconds) {
        return NH_SIGNAL_IDLE;
    }

    NH_CORE_CHECK(nh_core_updateSystem())
    NH_SYSTEM.Updater.LastUpdate = nh_core_getSystemTime();

    return NH_SIGNAL_OK;
}

NH_API_RESULT nh_core_startSystemWorkload() 
{
    nh_core_activateWorkload(nh_core_initSystemWorkload, nh_core_runSystemWorkload, NULL, NULL, NULL, false);
    return NH_API_SUCCESS;
}
