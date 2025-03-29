#ifndef NH_CORE_HASHMAP_H
#define NH_CORE_HASHMAP_H

// LICENSE NOTICE ===================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Array.h"
#include "../Common/Includes.h"

#pragma GCC diagnostic ignored "-Wendif-labels"
#include "../External/c_hashmap/hashmap.h"

// FUNCTIONS =======================================================================================

#define nh_core_HashMap map_t 

nh_core_HashMap nh_core_createHashMap(
);

NH_API_RESULT nh_core_addToHashMap(
    nh_core_HashMap *Map_p, char *key_p, void *value_p
);

void nh_core_freeHashMap(
    nh_core_HashMap Map
);

void *nh_core_getFromHashMap(
    nh_core_HashMap *Map_p, char *key_p
);

#endif 
