// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Memory.h"
#include "../Header/Macros.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <stdlib.h>

// IMPLEMENT ======================================================================================

void* Nh_allocate(
    size_t size)
{
NH_BEGIN()

NH_END(malloc(size))
}

void Nh_free(
    void *ptr)
{
NH_BEGIN()

    free(ptr);

NH_SILENT_END()
}

NH_RESULT Nh_initializeMemory()
{
NH_BEGIN()

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_initializeThreadMemory()
{
NH_BEGIN()

NH_END(NH_SUCCESS)
}

