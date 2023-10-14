// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_TERMINAL_RESULTS_PP[] = 
{
    "NH_TERMINAL_SUCCESS",                    
    "NH_TERMINAL_ERROR_NULL_POINTER",         
    "NH_TERMINAL_ERROR_BAD_STATE", 
    "NH_TERMINAL_ERROR_MEMORY_ALLOCATION",
    "NH_TERMINAL_ERROR_TERMINFO_DATA_CANNOT_BE_FOUND",
    "NH_TERMINAL_ERROR_UNKNOWN_TERMINAL_TYPE",
    "NH_TERMINAL_ERROR_TERMINAL_IS_HARDCOPY",
    "NH_TERMINAL_ERROR_UNKNOWN_COMMAND",
    "NH_TERMINAL_ERROR_INVALID_ARGUMENT",
};

unsigned int NH_TERMINAL_RESULTS_PP_COUNT = sizeof(NH_TERMINAL_RESULTS_PP) / sizeof(NH_TERMINAL_RESULTS_PP[0]);

