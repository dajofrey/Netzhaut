// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_URL_RESULTS_PP[] = 
{
    "NH_URL_SUCCESS",                    
    "NH_URL_ERROR_NULL_POINTER",         
    "NH_URL_ERROR_BAD_STATE", 
    "NH_URL_ERROR_MEMORY_ALLOCATION",
};

unsigned int NH_URL_RESULTS_PP_COUNT = sizeof(NH_URL_RESULTS_PP)/sizeof(NH_URL_RESULTS_PP[0]);

