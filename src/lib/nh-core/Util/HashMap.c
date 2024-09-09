// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "HashMap.h"

#include "../System/Memory.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <string.h>

// CREATE ==========================================================================================

nh_HashMap nh_core_createHashMap()
{
NH_CORE_BEGIN()
NH_CORE_DIAGNOSTIC_END(hashmap_new())
}

// ADD =============================================================================================

NH_CORE_RESULT nh_core_addToHashMap(
    nh_HashMap *Map_p, NH_BYTE *key_p, void *value_p)
{
NH_CORE_BEGIN()

    if (hashmap_put(*Map_p, key_p, value_p) != MAP_OK) {
        NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)
    }

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

// FREE ============================================================================================

void nh_core_freeHashMap(
    nh_HashMap Map)
{
NH_CORE_BEGIN()

    hashmap_free(Map);

NH_CORE_SILENT_END()
}

// GET =============================================================================================

void *nh_core_getFromHashMap(
    nh_HashMap *Map_p, NH_BYTE *key_p)
{
NH_CORE_BEGIN()

    void *handle_p = NULL;
    if (hashmap_get(*Map_p, key_p, (void**)(&handle_p)) != MAP_OK) {
        NH_CORE_END(NULL)
    }

NH_CORE_END(handle_p)
}

