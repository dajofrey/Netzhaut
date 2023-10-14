// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =========================================================================================

#include "Result.h"

// RESULTS =========================================================================================

const char *NH_TTY_RESULTS_PP[] = 
{
    "NH_TTY_SUCCESS",                    
    "NH_TTY_ERROR_NULL_POINTER",         
    "NH_TTY_ERROR_BAD_STATE", 
    "NH_TTY_ERROR_MEMORY_ALLOCATION",
    "NH_TTY_ERROR_TERMINFO_DATA_CANNOT_BE_FOUND",
    "NH_TTY_ERROR_UNKNOWN_TERMINAL_TYPE",
    "NH_TTY_ERROR_TERMINAL_IS_HARDCOPY",
    "NH_TTY_ERROR_UNKNOWN_COMMAND",
    "NH_TTY_ERROR_INVALID_ARGUMENT",
};

unsigned int NH_TTY_RESULTS_PP_COUNT = sizeof(NH_TTY_RESULTS_PP) / sizeof(NH_TTY_RESULTS_PP[0]);

