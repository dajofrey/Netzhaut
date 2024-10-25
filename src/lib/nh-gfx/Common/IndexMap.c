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

#include "../../nh-core/System/Memory.h"

#include <string.h>

// DECLARE =========================================================================================

typedef enum NH_GFX_INDEXMAP_E {
    NH_GFX_INDEXMAP_OPENGL_COMMAND_NAMES = 0,
    NH_GFX_INDEXMAP_COUNT,
} NH_GFX_INDEXMAP_E;

// DATA ============================================================================================

nh_gfx_IndexMap NH_GFX_INDEXMAP;
static unsigned int *indices_pp[NH_GFX_INDEXMAP_COUNT] = {NULL};

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_gfx_getNames(
    NH_GFX_INDEXMAP_E type, char ***array_ppp, int *count_p)
{
    switch (type)
    {
        case NH_GFX_INDEXMAP_OPENGL_COMMAND_NAMES :
        {
            *array_ppp = (char**) NH_GFX_OPENGL_COMMAND_NAMES_PP; 
            *count_p = NH_GFX_OPENGL_COMMAND_NAMES_PP_COUNT; 
            break;
        }
        default : return NH_API_ERROR_BAD_STATE;
    }

    if (*array_ppp == NULL) {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_gfx_createSingleIndexMap(
    NH_GFX_INDEXMAP_E type, nh_core_HashMap *map_p)
{
    int count = 0; char **names_pp = NULL;
    NH_CORE_CHECK(nh_gfx_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_CORE_CHECK(nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_gfx_createIndexMap()
{
    for (int type = 0; type < NH_GFX_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const char **names_pp = NULL;
        if (nh_gfx_getNames(type, (char***)&names_pp, &count) != NH_API_SUCCESS) {
            return NH_API_ERROR_BAD_STATE;
        }

        indices_pp[type] = nh_core_allocate(sizeof(unsigned int) * count);
        NH_CORE_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {
            indices_pp[type][i] = i;
        }
    }    

    NH_CORE_CHECK(nh_gfx_createSingleIndexMap(NH_GFX_INDEXMAP_OPENGL_COMMAND_NAMES, &NH_GFX_INDEXMAP.OpenGLCommandNames))

    return NH_API_SUCCESS;
}

void nh_gfx_freeIndexMap()
{
    for (int type = 0; type < NH_GFX_INDEXMAP_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_GFX_INDEXMAP.OpenGLCommandNames);

    return;
}

