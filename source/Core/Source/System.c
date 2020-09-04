// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#ifdef __unix__
#define _GNU_SOURCE
#endif

#include "../Header/System.h"
#include "../Header/Memory.h"
#include "../Header/Macros.h"
#include "../Header/Config.h"

#include "../../Graphics/Vulkan/Header/Vulkan.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>
#include <stdio.h>

#ifdef __unix__
    #include <sys/sysinfo.h>
#endif

// DATA ===========================================================================================

Nh_System System;

// DECLARE ========================================================================================

static NH_RESULT Nh_getCPUInfo(
);
static NH_RESULT Nh_getRAMInfo(
);

static NH_RESULT getCPUAttr(
    char *src, char *dest
); 

static NH_RESULT Nh_initGPUs(
);

// SYSTEM ==========================================================================================

NH_RESULT Nh_initSystem()
{
NH_BEGIN()

    for (int i = 0; i < NH_API_COUNT; ++i) {System.available_p[i] = false;}

    NH_CHECK(Nh_initGPUs())   
    NH_CHECK(Nh_getCPUInfo())
    NH_CHECK(Nh_getRAMInfo())

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_freeSystem()
{
NH_BEGIN()

    Nh_Vk_freeVulkan();

NH_END(NH_SUCCESS)
}

bool Nh_available(
    NH_API API) 
{
NH_BEGIN()
NH_END(System.available_p[API])
}

Nh_System *Nh_getSystem()
{
NH_BEGIN()
NH_END(&System)
}

// CPU =============================================================================================

static NH_RESULT Nh_getCPUInfo()
{
NH_BEGIN()

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
        NH_CHECK(getCPUAttr(ptr, name_p))

    // cores
        char *ptr2 = strstr(arg2, "cpu cores");
        char tmp[8];
        NH_CHECK(getCPUAttr(ptr2, tmp))
        cores = atoi(tmp);

    // ht
        char *ptr3 = strstr(arg3, "flags");
        char flags[2048];
        
        NH_CHECK(getCPUAttr(ptr3, flags))
            
        ht = strstr(flags, "ht") != NULL;
    }
    free(arg);
    fclose(cpuinfo);

#elif defined(_WIN32) || defined (WIN32)

#endif

    System.CPU.cores = cores;
    System.CPU.ht    = ht;
    strcpy(System.CPU.name_p, name_p);

NH_END(NH_SUCCESS)
}

static NH_RESULT getCPUAttr(
    char *src, char *dest) 
{
NH_BEGIN()

    if (src != NULL) {
        char delim[] = ":";
        strtok(src, delim);
        char *src2 = strtok(NULL, delim);
        char delim2[] = "\n";
        char *src3 = strtok(src2, delim2); 
        strcpy(dest, src3 + 1);
    }
    else {NH_END(NH_ERROR_BAD_STATE)}

NH_END(NH_SUCCESS)
}

// RAM =============================================================================================

static NH_RESULT Nh_getRAMInfo()
{
NH_BEGIN()

    unsigned long total = 0;
    unsigned long free  = 0;

#ifdef __unix__

    struct sysinfo info;
    sysinfo(&info);
    total = info.totalram;
    free  = info.freeram;

#elif defined(_WIN32) || defined (WIN32)

#endif

    System.RAM.total = total;
    System.RAM.free  = free;

NH_END(NH_SUCCESS)
}

// GPU =============================================================================================

static NH_RESULT Nh_initGPUs()
{
NH_BEGIN()

    int count = 0;
    if (Nh_Vk_initVulkan() == NH_SUCCESS) {
        System.available_p[NH_API_VULKAN] = true; count++;
    }

    if (count <= 0) {NH_END(NH_INIT_ERROR_NO_GRAPHICS_API_AVAILABLE)}
   
NH_END(NH_SUCCESS)
}

