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

typedef enum NH_TERMINAL_INDEXMAP_E {
    NH_TERMINAL_INDEXMAP_SETTING_NAMES = 0,
    NH_TERMINAL_INDEXMAP_E_COUNT,
} NH_TERMINAL_INDEXMAP_E;

// DATA ============================================================================================

nh_terminal_IndexMap NH_TERMINAL_INDEXMAP;
static unsigned int *indices_pp[NH_TERMINAL_INDEXMAP_E_COUNT] = {NULL};

// CREATE/DESTROY ==================================================================================

static NH_TERMINAL_RESULT nh_terminal_getNames(
    NH_TERMINAL_INDEXMAP_E type, NH_BYTE ***array_ppp, int *count_p)
{
NH_TERMINAL_BEGIN()

    switch (type)
    {
        case NH_TERMINAL_INDEXMAP_SETTING_NAMES :
        {
            *array_ppp = (NH_BYTE**) NH_TERMINAL_SETTING_NAMES_PP; 
            *count_p = NH_TERMINAL_SETTING_NAMES_PP_COUNT; 
            break;
        }
        default : NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_ERROR_BAD_STATE)}

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_core_createSingleIndexMap(
    NH_TERMINAL_INDEXMAP_E type, nh_HashMap *map_p)
{
NH_TERMINAL_BEGIN()

    int count = 0; NH_BYTE **names_pp = NULL;
    NH_TERMINAL_CHECK(nh_terminal_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_TERMINAL_CHECK(nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_createIndexMap()
{
NH_TERMINAL_BEGIN()

    for (int type = 0; type < NH_TERMINAL_INDEXMAP_E_COUNT; ++type)  
    {
        int count = 0;
        const NH_BYTE **names_pp = NULL;
        if (nh_terminal_getNames(type, (NH_BYTE***)&names_pp, &count) != NH_TERMINAL_SUCCESS) {
            NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_ERROR_BAD_STATE)
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_TERMINAL_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_TERMINAL_CHECK(nh_core_createSingleIndexMap(NH_TERMINAL_INDEXMAP_SETTING_NAMES, &NH_TERMINAL_INDEXMAP.SettingNames))

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

void nh_terminal_freeIndexMap()
{
NH_TERMINAL_BEGIN()

    for (int type = 0; type < NH_TERMINAL_INDEXMAP_E_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_TERMINAL_INDEXMAP.SettingNames);

NH_TERMINAL_SILENT_END()
}

