#ifndef NH_CORE_SYSTEM_MEMORY_H
#define NH_CORE_SYSTEM_MEMORY_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
#include <stddef.h>

// FUNCTIONS =======================================================================================

void *nh_core_allocate(
    size_t size
);

void *nh_core_reallocate(
    void *ptr, size_t size
);

void nh_core_free(
    void *ptr
);

NH_API_RESULT nh_core_initMemory(
);

NH_API_RESULT nh_core_initThreadMemory(
);

#endif // NH_CORE_SYSTEM_MEMORY_H
