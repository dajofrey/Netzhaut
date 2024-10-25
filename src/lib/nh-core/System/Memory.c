// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Memory.h"

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
    return realloc(ptr, size);
}

void nh_core_free(
    void *ptr)
{
    free(ptr);
}

NH_API_RESULT nh_core_initMemory()
{
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_initThreadMemory()
{
    return NH_API_SUCCESS;
}
