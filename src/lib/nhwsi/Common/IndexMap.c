// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "IndexMap.h"
#include "Macros.h"
#include "Config.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/Array.h"
#include "../../nhcore/Util/MediaType.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_WSI_INDEXMAP_E {
    NH_WSI_INDEXMAP_SETTING_NAMES = 0,
    NH_WSI_INDEXMAP_E_COUNT,
} NH_WSI_INDEXMAP_E;

// DATA ============================================================================================

nh_wsi_IndexMap NH_WSI_INDEXMAP;
static unsigned int *indices_pp[NH_WSI_INDEXMAP_E_COUNT] = {NULL};

// CREATE/DESTROY ==================================================================================

static NH_WSI_RESULT_E nh_wsi_getNames(
    NH_WSI_INDEXMAP_E type, NH_BYTE ***array_ppp, int *count_p)
{
NH_WSI_BEGIN()

    switch (type)
    {
        case NH_WSI_INDEXMAP_SETTING_NAMES :
        {
            *array_ppp = (NH_BYTE**) NH_WSI_SETTING_NAMES_PP; 
            *count_p = NH_WSI_SETTING_NAMES_PP_COUNT; 
            break;
        }
        default : NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)}

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

static NH_WSI_RESULT_E nh_core_createSingleIndexMap(
    NH_WSI_INDEXMAP_E type, nh_HashMap *map_p)
{
NH_WSI_BEGIN()

    int count = 0; NH_BYTE **names_pp = NULL;
    NH_WSI_CHECK(nh_wsi_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_WSI_CHECK(nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

NH_WSI_RESULT_E nh_wsi_createIndexMap()
{
NH_WSI_BEGIN()

    for (int type = 0; type < NH_WSI_INDEXMAP_E_COUNT; ++type)  
    {
        int count = 0;
        const NH_BYTE **names_pp = NULL;
        if (nh_wsi_getNames(type, (NH_BYTE***)&names_pp, &count) != NH_WSI_SUCCESS) {
            NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_WSI_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_WSI_CHECK(nh_core_createSingleIndexMap(NH_WSI_INDEXMAP_SETTING_NAMES, &NH_WSI_INDEXMAP.SettingNames))

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

void nh_wsi_freeIndexMap()
{
NH_WSI_BEGIN()

    for (int type = 0; type < NH_WSI_INDEXMAP_E_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_WSI_INDEXMAP.SettingNames);

NH_WSI_SILENT_END()
}

