#ifndef NH_CORE_MEMORY_H
#define NH_CORE_MEMORY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stddef.h>

#endif

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    void *nh_core_allocate(
        size_t size
    );

    void *nh_core_reallocate(
        void *ptr, size_t size
    );

    void nh_core_free(
        void *ptr
    );
    
    NH_CORE_RESULT nh_core_initMemory(
    );
    
    NH_CORE_RESULT nh_core_initThreadMemory(
    );

/** @} */

#endif
