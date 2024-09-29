// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "IndexMap.h"
#include "Macros.h"

#include "../Properties/Properties.h"
#include "../Properties/Color.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_CSS_INDEXMAP_E {
    NH_CSS_INDEXMAP_COLORS = 0,
    NH_CSS_INDEXMAP_PROPERTIES,
    NH_CSS_INDEXMAP_COUNT,
} NH_CSS_INDEXMAP_E;

// DATA ============================================================================================

nh_css_IndexMap NH_CSS_INDEXMAP;
static unsigned int *indices_pp[NH_CSS_INDEXMAP_COUNT] = {NULL};
static bool allocated = false;

// CREATE/DESTROY ==================================================================================

static NH_API_RESULT nh_css_getNames(
    NH_CSS_INDEXMAP_E type, char ***array_ppp, int *count_p)
{
NH_CSS_BEGIN()

    switch (type)
    {
        case NH_CSS_INDEXMAP_COLORS :
        {
            *array_ppp = (char**)NH_CSS_COLORS_PP; 
            *count_p = NH_CSS_COLORS_PP_COUNT; 
            break;
        }
        case NH_CSS_INDEXMAP_PROPERTIES :
        {
            *array_ppp = (char**)NH_CSS_PROPERTY_NAMES_PP; 
            *count_p = NH_CSS_PROPERTY_COUNT; 
            break;
        }

        default : NH_CSS_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_CSS_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

NH_CSS_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_css_createSingleIndexMap(
    NH_CSS_INDEXMAP_E type, nh_HashMap *map_p)
{
NH_CSS_BEGIN()

    int count = 0; char **names_pp = NULL;
    NH_CSS_CHECK(nh_css_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]);
    }

NH_CSS_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_css_createIndexMap()
{
NH_CSS_BEGIN()

    if (allocated) {NH_CSS_DIAGNOSTIC_END(NH_API_SUCCESS)}

    for (int type = 0; type < NH_CSS_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const char **names_pp = NULL;
        if (nh_css_getNames(type, (char***)&names_pp, &count) != NH_API_SUCCESS) {
            NH_CSS_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_CSS_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_CSS_CHECK(nh_css_createSingleIndexMap(NH_CSS_INDEXMAP_COLORS, &NH_CSS_INDEXMAP.Colors))
    NH_CSS_CHECK(nh_css_createSingleIndexMap(NH_CSS_INDEXMAP_PROPERTIES, &NH_CSS_INDEXMAP.Properties))

    allocated = true;

NH_CSS_DIAGNOSTIC_END(NH_API_SUCCESS)
}

void nh_css_freeIndexMap()
{
NH_CSS_BEGIN()

    if (allocated) {NH_CSS_SILENT_END()}

    for (int type = 0; type < NH_CSS_INDEXMAP_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_CSS_INDEXMAP.Colors);
    nh_core_freeHashMap(NH_CSS_INDEXMAP.Properties);

    allocated = false;

NH_CSS_SILENT_END()
}

