// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "IndexMap.h"

#include "../Engine/CST.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_ECMASCRIPT_INDEXMAP_E {
    NH_ECMASCRIPT_INDEXMAP_PARSE_NODE_NAMES = 0,
    NH_ECMASCRIPT_INDEXMAP_RESERVED_WORDS = 1,
    NH_ECMASCRIPT_INDEXMAP_COUNT,
} NH_ECMASCRIPT_INDEXMAP_E;

// DATA ============================================================================================

nh_ecmascript_IndexMap NH_ECMASCRIPT_INDEXMAP;
static unsigned int *indices_pp[NH_ECMASCRIPT_INDEXMAP_COUNT] = {NULL};
static bool allocated = false;

// CREATE/DESTROY ==================================================================================

static NH_API_RESULT nh_ecmascript_getNames(
    NH_ECMASCRIPT_INDEXMAP_E type, char ***array_ppp, int *count_p)
{
    switch (type)
    {
        case NH_ECMASCRIPT_INDEXMAP_PARSE_NODE_NAMES :
        {
            *array_ppp = (char**) NH_ECMASCRIPT_PARSE_NODE_NAMES_PP; 
            *count_p = NH_ECMASCRIPT_PARSE_NODE_NAMES_PP_COUNT; 
            break;
        }
        case NH_ECMASCRIPT_INDEXMAP_RESERVED_WORDS :
        {
            *array_ppp = (char**) NH_ECMASCRIPT_RESERVED_WORDS_PP; 
            *count_p = NH_ECMASCRIPT_RESERVED_WORDS_PP_COUNT; 
            break;
        }
        default : return NH_API_ERROR_BAD_STATE;
    }

    if (*array_ppp == NULL) {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_ecmascript_createSingleIndexMap(
    NH_ECMASCRIPT_INDEXMAP_E type, nh_core_HashMap *map_p)
{
    int count = 0; char **names_pp = NULL;
    NH_CORE_CHECK(nh_ecmascript_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_ecmascript_createIndexMap()
{
    if (allocated) {return NH_API_SUCCESS;}

    for (int type = 0; type < NH_ECMASCRIPT_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const char **names_pp = NULL;
        if (nh_ecmascript_getNames(type, (char***)&names_pp, &count) != NH_API_SUCCESS) {
            return NH_API_ERROR_BAD_STATE;
        }

        indices_pp[type] = (unsigned int*)nh_core_allocate(sizeof(unsigned int) * count);
        NH_CORE_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_CORE_CHECK(nh_ecmascript_createSingleIndexMap(NH_ECMASCRIPT_INDEXMAP_PARSE_NODE_NAMES, &NH_ECMASCRIPT_INDEXMAP.ParseNodeNames))
    NH_CORE_CHECK(nh_ecmascript_createSingleIndexMap(NH_ECMASCRIPT_INDEXMAP_RESERVED_WORDS, &NH_ECMASCRIPT_INDEXMAP.ReservedWords))

    allocated = true;

    return NH_API_SUCCESS;
}

void nh_ecmascript_freeIndexMap()
{
    if (allocated) {return ;}

    for (int type = 0; type < NH_ECMASCRIPT_INDEXMAP_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_ECMASCRIPT_INDEXMAP.ParseNodeNames);
    nh_core_freeHashMap(NH_ECMASCRIPT_INDEXMAP.ReservedWords);

    allocated = false;

    return;
}

