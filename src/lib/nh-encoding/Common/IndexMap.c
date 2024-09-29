// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "IndexMap.h"
#include "Macros.h"

#include "../Base/Encodings.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_ENCODING_INDEXMAP_E {
    NH_ENCODING_INDEXMAP_LABELS = 0,
    NH_ENCODING_INDEXMAP_E_COUNT,
} NH_ENCODING_INDEXMAP_E;

// DATA ============================================================================================

nh_encoding_IndexMap NH_ENCODING_INDEXMAP;
static unsigned int *indices_pp[NH_ENCODING_INDEXMAP_E_COUNT] = {NULL};
static bool allocated = false;

// CREATE/DESTROY ==================================================================================

static NH_API_RESULT nh_encoding_getNames(
    NH_ENCODING_INDEXMAP_E type, char ***array_ppp, int *count_p)
{
NH_ENCODING_BEGIN()

    switch (type)
    {
        case NH_ENCODING_INDEXMAP_LABELS :
        {
            *array_ppp = (char**) NH_ENCODING_LABELS_PP; 
            *count_p = NH_ENCODING_LABELS_PP_COUNT; 
            break;
        }
        default : NH_ENCODING_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_ENCODING_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

NH_ENCODING_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_encoding_createSingleIndexMap(
    NH_ENCODING_INDEXMAP_E type, nh_HashMap *map_p)
{
NH_ENCODING_BEGIN()

    int count = 0; char **names_pp = NULL;
    NH_ENCODING_CHECK(nh_encoding_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]);
    }

NH_ENCODING_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_encoding_createIndexMap()
{
NH_ENCODING_BEGIN()

    if (allocated) {NH_ENCODING_DIAGNOSTIC_END(NH_API_SUCCESS)}

    for (int type = 0; type < NH_ENCODING_INDEXMAP_E_COUNT; ++type)  
    {
        int count = 0;
        const char **names_pp = NULL;
        if (nh_encoding_getNames(type, (char***)&names_pp, &count) != NH_API_SUCCESS) {
            NH_ENCODING_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_ENCODING_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_ENCODING_CHECK(nh_encoding_createSingleIndexMap(NH_ENCODING_INDEXMAP_LABELS, &NH_ENCODING_INDEXMAP.Labels))

    allocated = true;

NH_ENCODING_DIAGNOSTIC_END(NH_API_SUCCESS)
}

void nh_encoding_freeIndexMap()
{
NH_ENCODING_BEGIN()

    if (!allocated) {NH_ENCODING_SILENT_END()}

    for (int type = 0; type < NH_ENCODING_INDEXMAP_E_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_ENCODING_INDEXMAP.Labels);

    allocated = false;

NH_ENCODING_SILENT_END()
}

