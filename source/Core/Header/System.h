#ifndef NH_SYSTEM_H
#define NH_SYSTEM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Thread.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct Nh_Config Nh_Config;

#endif

/** @addtogroup CoreEnums Enums
 *  \ingroup Core
 *  @{
 */

    typedef enum NH_API {
        NH_API_VULKAN,
        NH_API_OPENGL,
        NH_API_COUNT,
    } NH_API;
    
/** @} */

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_CPU {
        char name_p[255];
        int cores;       
        bool ht;         
    } Nh_CPU;
    
    typedef struct Nh_RAM {
        unsigned long total;
        unsigned long free; 
    } Nh_RAM;

    typedef struct Nh_GPU {
        NH_API API;
        void *Pointer;
    } Nh_GPU;
    
    typedef struct Nh_System {
        bool available_p[NH_API_COUNT];
        Nh_RAM RAM;                     
        Nh_CPU CPU; 
    } Nh_System;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    NH_RESULT Nh_initSystem(
    );
    
    NH_RESULT Nh_freeSystem(
    );

    bool Nh_available(
        NH_API API
    );
   
    Nh_System *Nh_getSystem(
    );

/** @} */

#endif 
