// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Memory.h"

#include "../Common/Macros.h"

#include <stdlib.h>
#include <stdio.h>

// IMPLEMENT ======================================================================================

void* nh_core_allocate(
    size_t size)
{
    return malloc(size);
}

void* nh_core_reallocate(
    void *ptr, size_t size)
{
NH_CORE_BEGIN()
NH_CORE_END(realloc(ptr, size))
}

void nh_core_free(
    void *ptr)
{
    free(ptr);
}

NH_CORE_RESULT nh_core_initMemory()
{
NH_CORE_BEGIN()
NH_CORE_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_initThreadMemory()
{
NH_CORE_BEGIN()
NH_CORE_END(NH_CORE_SUCCESS)
}
