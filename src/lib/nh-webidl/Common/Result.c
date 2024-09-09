// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_WEBIDL_RESULTS_PP[] = 
{
    "NH_WEBIDL_SUCCESS",                    
    "NH_WEBIDL_ERROR_NULL_POINTER",         
    "NH_WEBIDL_ERROR_BAD_STATE", 
    "NH_WEBIDL_ERROR_MEMORY_ALLOCATION",
};

unsigned int NH_WEBIDL_RESULTS_PP_COUNT = sizeof(NH_WEBIDL_RESULTS_PP) / sizeof(NH_WEBIDL_RESULTS_PP[0]);

