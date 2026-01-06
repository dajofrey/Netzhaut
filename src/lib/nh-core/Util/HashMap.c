// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "HashMap.h"

#include "../System/Memory.h"

#pragma GCC diagnostic ignored "-Wendif-labels"

#include <stdio.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_core_HashMap nh_core_createHashMap()
{
    return hashmap_new();
}

NH_API_RESULT nh_core_addToHashMap(
    nh_core_HashMap *Map_p, char *key_p, void *value_p)
{
    if (hashmap_put(*Map_p, key_p, value_p) != MAP_OK) {
        return NH_API_ERROR_BAD_STATE;
    }

    return NH_API_SUCCESS;
}

void nh_core_freeHashMap(
    nh_core_HashMap Map)
{
    hashmap_free(Map);

    return;
}

void *nh_core_getFromHashMap(
    nh_core_HashMap *Map_p, char *key_p)
{
    void *handle_p = NULL;
    if (hashmap_get(*Map_p, key_p, (void**)(&handle_p)) != MAP_OK) {
        return NULL;
    }

    return handle_p;
}

unsigned int nh_core_getHashMapLength(
    nh_core_HashMap *Map_p)
{
    return hashmap_length(*Map_p);
}

void *nh_core_getValueFromHashMapUsingIndex(
    nh_core_HashMap *Map_p, unsigned int index)
{
    return hashmap_getFromIndex(*Map_p, index);
}

char *nh_core_getKeyFromHashMapUsingIndex(
    nh_core_HashMap *Map_p, unsigned int index)
{
    return hashmap_getKeyFromIndex(*Map_p, index);
}
