// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "IndexMap.h"
#include "Macros.h"

#include "../Parser/Elements.h"
#include "../../nhcore/System/Memory.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_HTML_INDEXMAP_E {
    NH_HTML_INDEXMAP_TAGS = 0,
    NH_HTML_INDEXMAP_COUNT,
} NH_HTML_INDEXMAP_E;

// DATA ============================================================================================

nh_html_IndexMap NH_HTML_INDEXMAP;
static unsigned int *indices_pp[NH_HTML_INDEXMAP_COUNT] = {NULL};
static NH_BOOL allocated = NH_FALSE;

// CREATE/DESTROY ==================================================================================

static NH_HTML_RESULT nh_html_getNames(
    NH_HTML_INDEXMAP_E type, NH_BYTE ***array_ppp, int *count_p)
{
NH_HTML_BEGIN()

    switch (type)
    {
        case NH_HTML_INDEXMAP_TAGS :
        {
            *array_ppp = (NH_BYTE**)NH_HTML_TAG_NAMES_PP; 
            *count_p = NH_HTML_TAG_NAMES_PP_COUNT; 
            break;
        }
        default : NH_HTML_DIAGNOSTIC_END(NH_HTML_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_HTML_DIAGNOSTIC_END(NH_HTML_ERROR_BAD_STATE)}

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT nh_html_createSingleIndexMap(
    NH_HTML_INDEXMAP_E type, nh_HashMap *map_p)
{
NH_HTML_BEGIN()

    int count = 0; NH_BYTE **names_pp = NULL;
    NH_HTML_CHECK(nh_html_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

NH_HTML_RESULT nh_html_createIndexMap()
{
NH_HTML_BEGIN()

    if (allocated) {NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)}

    for (int type = 0; type < NH_HTML_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const NH_BYTE **names_pp = NULL;
        if (nh_html_getNames(type, (NH_BYTE***)&names_pp, &count) != NH_HTML_SUCCESS) {
            NH_HTML_DIAGNOSTIC_END(NH_HTML_ERROR_BAD_STATE)
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_HTML_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_HTML_CHECK(nh_html_createSingleIndexMap(NH_HTML_INDEXMAP_TAGS, &NH_HTML_INDEXMAP.Tags))

    allocated = NH_TRUE;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

void nh_html_freeIndexMap()
{
NH_HTML_BEGIN()

    if (allocated) {NH_HTML_SILENT_END()}

    for (int type = 0; type < NH_HTML_INDEXMAP_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_HTML_INDEXMAP.Tags);

    allocated = NH_FALSE;

NH_HTML_SILENT_END()
}

