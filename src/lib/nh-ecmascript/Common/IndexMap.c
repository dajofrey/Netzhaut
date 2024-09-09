// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "IndexMap.h"
#include "Macros.h"

#include "../Engine/Parser.h"
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
static NH_BOOL allocated = NH_FALSE;

// CREATE/DESTROY ==================================================================================

static NH_ECMASCRIPT_RESULT nh_ecmascript_getNames(
    NH_ECMASCRIPT_INDEXMAP_E type, NH_BYTE ***array_ppp, int *count_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (type)
    {
        case NH_ECMASCRIPT_INDEXMAP_PARSE_NODE_NAMES :
        {
            *array_ppp = (NH_BYTE**) NH_ECMASCRIPT_PARSE_NODE_NAMES_PP; 
            *count_p = NH_ECMASCRIPT_PARSE_NODE_NAMES_PP_COUNT; 
            break;
        }
        case NH_ECMASCRIPT_INDEXMAP_RESERVED_WORDS :
        {
            *array_ppp = (NH_BYTE**) NH_ECMASCRIPT_RESERVED_WORDS_PP; 
            *count_p = NH_ECMASCRIPT_RESERVED_WORDS_PP_COUNT; 
            break;
        }
        default : NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_ERROR_BAD_STATE)}

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

static NH_ECMASCRIPT_RESULT nh_ecmascript_createSingleIndexMap(
    NH_ECMASCRIPT_INDEXMAP_E type, nh_HashMap *map_p)
{
NH_ECMASCRIPT_BEGIN()

    int count = 0; NH_BYTE **names_pp = NULL;
    NH_ECMASCRIPT_CHECK(nh_ecmascript_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]);
    }

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

NH_ECMASCRIPT_RESULT nh_ecmascript_createIndexMap()
{
NH_ECMASCRIPT_BEGIN()

    if (allocated) {NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)}

    for (int type = 0; type < NH_ECMASCRIPT_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const NH_BYTE **names_pp = NULL;
        if (nh_ecmascript_getNames(type, (NH_BYTE***)&names_pp, &count) != NH_ECMASCRIPT_SUCCESS) {
            NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_ERROR_BAD_STATE)
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_ECMASCRIPT_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_ECMASCRIPT_CHECK(nh_ecmascript_createSingleIndexMap(NH_ECMASCRIPT_INDEXMAP_PARSE_NODE_NAMES, &NH_ECMASCRIPT_INDEXMAP.ParseNodeNames))
    NH_ECMASCRIPT_CHECK(nh_ecmascript_createSingleIndexMap(NH_ECMASCRIPT_INDEXMAP_RESERVED_WORDS, &NH_ECMASCRIPT_INDEXMAP.ReservedWords))

    allocated = NH_TRUE;

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

void nh_ecmascript_freeIndexMap()
{
NH_ECMASCRIPT_BEGIN()

    if (allocated) {NH_ECMASCRIPT_SILENT_END()}

    for (int type = 0; type < NH_ECMASCRIPT_INDEXMAP_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_ECMASCRIPT_INDEXMAP.ParseNodeNames);
    nh_core_freeHashMap(NH_ECMASCRIPT_INDEXMAP.ReservedWords);

    allocated = NH_FALSE;

NH_ECMASCRIPT_SILENT_END()
}

