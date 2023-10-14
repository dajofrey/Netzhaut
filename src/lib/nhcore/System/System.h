#ifndef NH_CORE_SYSTEM_H
#define NH_CORE_SYSTEM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Thread.h"

#include "../Util/String.h"
#include "../Util/Time.h"

#include "../Common/Includes.h"

#include <stdlib.h>

#endif

/** @addtogroup lib_nhcore_macros
 *  @{
 */

    #define NH_HOSTNAME_LENGTH 255

/** @} */

/** @addtogroup lib_nhcore_structs
 *  @{
 */

    typedef struct nh_CPU {
        char name_p[255];
        int cores;       
        NH_BOOL ht;         
    } nh_CPU;
    
    typedef struct nh_RAM {
         unsigned long total;
         unsigned long free; 
    } nh_RAM;

    typedef struct nh_GPU {
        int API;
        void *Pointer;
    } nh_GPU;
    
    typedef struct nh_SystemUpdater {
        nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
    } nh_SystemUpdater;

    typedef struct nh_System {
        nh_SystemUpdater Updater;
        NH_BYTE hostname_p[NH_HOSTNAME_LENGTH];
        long uptime;
        nh_LocalTime LocalTime;
        NH_BOOL littleEndian;
        nh_RAM RAM;                     
        nh_CPU CPU; 
    } nh_System;

/** @} */

/** @addtogroup lib_nhcore_vars
 *  @{
 */

    extern nh_System NH_SYSTEM;
    
/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_initSystem(
    );
    
    NH_CORE_RESULT nh_core_freeSystem(
    );

    void *nh_core_initSystemUpdater(
        nh_core_Workload *Workload_p
    );

    NH_SIGNAL nh_core_runSystemUpdater(
        void *args_p
    );

/** @} */

#endif 
