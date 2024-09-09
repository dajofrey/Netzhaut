// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_DOM_RESULTS_PP[] = 
{
    "NH_DOM_SUCCESS",                    
    "NH_DOM_ERROR_NULL_POINTER",         
    "NH_DOM_ERROR_BAD_STATE", 
    "NH_DOM_ERROR_MEMORY_ALLOCATION",
};

unsigned int NH_DOM_RESULTS_PP_COUNT = sizeof(NH_DOM_RESULTS_PP) / sizeof(NH_DOM_RESULTS_PP[0]);

