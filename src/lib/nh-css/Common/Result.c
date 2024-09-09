// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_CSS_RESULTS_PP[] = 
{
    "NH_CSS_SUCCESS",                    
    "NH_CSS_ERROR_NULL_POINTER",         
    "NH_CSS_ERROR_BAD_STATE", 
    "NH_CSS_ERROR_MEMORY_ALLOCATION",
    "NH_CSS_SIGNAL_SPLIT_INLINE_BOX",

};

unsigned int NH_CSS_RESULTS_PP_COUNT = sizeof(NH_CSS_RESULTS_PP) / sizeof(NH_CSS_RESULTS_PP[0]);

