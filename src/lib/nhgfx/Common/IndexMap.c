// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "IndexMap.h"
#include "Macros.h"

#include "../OpenGL/API.h"

#include "../../nhcore/System/Memory.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_GFX_INDEXMAP_E {
    NH_GFX_INDEXMAP_OPENGL_COMMAND_NAMES = 0,
    NH_GFX_INDEXMAP_COUNT,
} NH_GFX_INDEXMAP_E;

// DATA ============================================================================================

nh_gfx_IndexMap NH_GFX_INDEXMAP;
static unsigned int *indices_pp[NH_GFX_INDEXMAP_COUNT] = {NULL};

// CREATE/DESTROY ==================================================================================

static NH_GFX_RESULT nh_gfx_getNames(
    NH_GFX_INDEXMAP_E type, NH_BYTE ***array_ppp, int *count_p)
{
NH_GFX_BEGIN()

    switch (type)
    {
        case NH_GFX_INDEXMAP_OPENGL_COMMAND_NAMES :
        {
            *array_ppp = (NH_BYTE**) NH_GFX_OPENGL_COMMAND_NAMES_PP; 
            *count_p = NH_GFX_OPENGL_COMMAND_NAMES_PP_COUNT; 
            break;
        }
        default : NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_BAD_STATE)
    }

    if (*array_ppp == NULL) {NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_BAD_STATE)}

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

static NH_GFX_RESULT nh_gfx_createSingleIndexMap(
    NH_GFX_INDEXMAP_E type, nh_HashMap *map_p)
{
NH_GFX_BEGIN()

    int count = 0; NH_BYTE **names_pp = NULL;
    NH_GFX_CHECK(nh_gfx_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_GFX_CHECK(nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

NH_GFX_RESULT nh_gfx_createIndexMap()
{
NH_GFX_BEGIN()

    for (int type = 0; type < NH_GFX_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const NH_BYTE **names_pp = NULL;
        if (nh_gfx_getNames(type, (NH_BYTE***)&names_pp, &count) != NH_GFX_SUCCESS) {
            NH_GFX_DIAGNOSTIC_END(NH_GFX_ERROR_BAD_STATE)
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_GFX_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_GFX_CHECK(nh_gfx_createSingleIndexMap(NH_GFX_INDEXMAP_OPENGL_COMMAND_NAMES, &NH_GFX_INDEXMAP.OpenGLCommandNames))

NH_GFX_DIAGNOSTIC_END(NH_GFX_SUCCESS)
}

void nh_gfx_freeIndexMap()
{
NH_GFX_BEGIN()

    for (int type = 0; type < NH_GFX_INDEXMAP_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_GFX_INDEXMAP.OpenGLCommandNames);

NH_GFX_SILENT_END()
}

