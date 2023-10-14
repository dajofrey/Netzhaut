// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_WSI_RESULT_ES_PP[] = 
{
    "NH_WSI_SUCCESS",                    
    "NH_WSI_ERROR_NULL_POINTER",         
    "NH_WSI_ERROR_BAD_STATE", 
    "NH_WSI_ERROR_MEMORY_ALLOCATION",
};

unsigned int NH_WSI_RESULT_ES_PP_COUNT = sizeof(NH_WSI_RESULT_ES_PP) / sizeof(NH_WSI_RESULT_ES_PP[0]);

