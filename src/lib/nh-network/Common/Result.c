// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_NETWORK_RESULTS_PP[] = 
{
    "NH_NETWORK_SUCCESS",                    
    "NH_NETWORK_ERROR_NULL_POINTER",         
    "NH_NETWORK_ERROR_BAD_STATE", 
    "NH_NETWORK_ERROR_MEMORY_ALLOCATION",
};

unsigned int NH_NETWORK_RESULTS_PP_COUNT = sizeof(NH_NETWORK_RESULTS_PP) / sizeof(NH_NETWORK_RESULTS_PP[0]);

