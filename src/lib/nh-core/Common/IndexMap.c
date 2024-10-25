// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "IndexMap.h"

#include "../System/Memory.h"

#include "../Util/Array.h"
#include "../Util/MediaType.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_INDEXMAP_E {
    NH_INDEXMAP_MEDIA_TYPE = 0,
    NH_INDEXMAP_COUNT,
} NH_INDEXMAP_E;

// DATA ============================================================================================

nh_IndexMap NH_INDEXMAP;
static unsigned int *indices_pp[NH_INDEXMAP_COUNT] = {NULL};

// CREATE/DESTROY ==================================================================================

static NH_API_RESULT nh_core_getNames(
    NH_INDEXMAP_E type, char ***array_ppp, int *count_p)
{
    switch (type)
    {
        case NH_INDEXMAP_MEDIA_TYPE :
        {
            *array_ppp = (char**) NH_MEDIA_TYPE_TEMPLATES_PP; 
            *count_p = NH_MEDIA_TYPE_TEMPLATES_PP_COUNT; 
            break;
        }
        default : return NH_API_ERROR_BAD_STATE;
    }

    if (*array_ppp == NULL) {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_createSingleIndexMap(
    NH_INDEXMAP_E type, nh_core_HashMap *map_p)
{
    int count = 0; char **names_pp = NULL;
    NH_CORE_CHECK(nh_core_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_CORE_CHECK(nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_createIndexMap()
{
    for (int type = 0; type < NH_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const char **names_pp = NULL;
        if (nh_core_getNames(type, (char***)&names_pp, &count) != NH_API_SUCCESS) {
            return NH_API_ERROR_BAD_STATE;
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_CORE_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_CORE_CHECK(nh_core_createSingleIndexMap(NH_INDEXMAP_MEDIA_TYPE, &NH_INDEXMAP.MediaTypes))

    return NH_API_SUCCESS;
}

void nh_core_freeIndexMap()
{
    for (int type = 0; type < NH_INDEXMAP_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_INDEXMAP.MediaTypes);

    return;
}
