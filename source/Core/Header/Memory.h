#ifndef NH_MEMORY_H
#define NH_MEMORY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"

#include <stddef.h>

#endif

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    void* Nh_allocate(
        size_t size
    );
    
    void Nh_free(
        void *ptr
    );
    
    NH_RESULT Nh_initializeMemory(
    );
    
    NH_RESULT Nh_initializeThreadMemory(
    );

/** @} */

#endif
