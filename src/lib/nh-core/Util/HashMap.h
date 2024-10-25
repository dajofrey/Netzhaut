#ifndef NH_CORE_HASHMAP_H
#define NH_CORE_HASHMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Array.h"

#include "../Common/Includes.h"
#include "../External/c_hashmap.h"

#endif

/** @addtogroup lib_nh-core_macros
 *  @{
 */

    #define nh_core_HashMap map_t 

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

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

/** @} */

#endif 
