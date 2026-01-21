// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "IndexMap.h"

#include "../Utils/Builtin.h"
#include "../Utils/Parser.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef enum NH_WEBIDL_INDEXMAP_E {
    NH_WEBIDL_INDEXMAP_FRAGMENT_NAMES = 0,
    NH_WEBIDL_INDEXMAP_FRAGMENT_FUNCTION_NAMES,
    NH_WEBIDL_INDEXMAP_PARSE_NODE_NAMES,
    NH_WEBIDL_INDEXMAP_COUNT,
} NH_WEBIDL_INDEXMAP_E;

// DATA ============================================================================================

static bool init = false;
nh_webidl_IndexMap NH_WEBIDL_INDEXMAP;
unsigned int *indices_pp[NH_WEBIDL_INDEXMAP_COUNT] = {NULL};

// CREATE/FREE =====================================================================================

static NH_API_RESULT nh_webidl_getNames(
    NH_WEBIDL_INDEXMAP_E type, char ***array_ppp, int *size_p)
{
    switch (type)
    {
        case NH_WEBIDL_INDEXMAP_FRAGMENT_NAMES :
        {
            *array_ppp = (char**) NH_WEBIDL_FRAGMENT_NAMES_PP; 
            *size_p = NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT; 
            break;
        }
        case NH_WEBIDL_INDEXMAP_FRAGMENT_FUNCTION_NAMES :
        {
//            *array_ppp = (char**) NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP; 
//            *size_p = NH_WEBIDL_FRAGMENT_FUNCTION_NAMES_PP_COUNT; 
            break;
        }
        case NH_WEBIDL_INDEXMAP_PARSE_NODE_NAMES :
        {
            *array_ppp = (char**) NH_WEBIDL_PARSE_NODE_NAMES_PP; 
            *size_p = NH_WEBIDL_PARSE_NODE_NAMES_PP_COUNT; 
            break;
        }
        default : return NH_API_ERROR_BAD_STATE;
    }

    if (array_ppp == NULL) {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_webidl_createSingleIndexMap(
    NH_WEBIDL_INDEXMAP_E type, nh_core_HashMap *map_p)
{
    int count = 0; char **names_pp = NULL;
    NH_CORE_CHECK(nh_webidl_getNames(type, &names_pp, &count))

    *map_p = nh_core_createHashMap();

    for (int i = 0; i < count; ++i) {
        NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_core_addToHashMap(map_p, names_pp[i], &indices_pp[type][i]))
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_webidl_createIndexMap()
{
    if (init) {return NH_API_SUCCESS;}

    for (int type = 0; type < NH_WEBIDL_INDEXMAP_COUNT; ++type)  
    {
        int count = 0;
        const char **names_pp = NULL;
        if (nh_webidl_getNames(type, (char***)&names_pp, &count) != NH_API_SUCCESS) {
            return NH_API_ERROR_BAD_STATE;
        }
        if (count == 0) {continue;}

        indices_pp[type] = (unsigned int*)nh_core_allocate(sizeof(unsigned int) * count);
        NH_CORE_CHECK_MEM(indices_pp[type])
        
        for (int i = 0; i < count; ++i) {indices_pp[type][i] = i;}
    }    

    NH_CORE_CHECK(nh_webidl_createSingleIndexMap(NH_WEBIDL_INDEXMAP_FRAGMENT_NAMES, &NH_WEBIDL_INDEXMAP.FragmentNames))
//    NH_CORE_CHECK(nh_webidl_createSingleIndexMap(NH_WEBIDL_INDEXMAP_FRAGMENT_FUNCTION_NAMES, &NH_WEBIDL_INDEXMAP.FragmentFunctionNames))
    NH_CORE_CHECK(nh_webidl_createSingleIndexMap(NH_WEBIDL_INDEXMAP_PARSE_NODE_NAMES, &NH_WEBIDL_INDEXMAP.ParseNodeNames))

    init = true;

    return NH_API_SUCCESS;
}

void nh_webidl_freeIndexMap()
{
    if (!init) {return;}

    for (int type = 0; type < NH_WEBIDL_INDEXMAP_COUNT; ++type)  
    {
         nh_core_free(indices_pp[type]);
         indices_pp[type] = NULL;
    }

    nh_core_freeHashMap(NH_WEBIDL_INDEXMAP.FragmentNames);
//    nh_core_freeHashMap(NH_WEBIDL_INDEXMAP.FragmentFunctionNames);
    nh_core_freeHashMap(NH_WEBIDL_INDEXMAP.ParseNodeNames);

    init = false;

    return;
}

