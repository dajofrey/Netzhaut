// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "Macros.h"
#include "IndexMap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_TERMINAL_RESULT nh_terminal_terminate()
{
NH_TERMINAL_BEGIN()

    nh_terminal_freeIndexMap();

NH_TERMINAL_DIAGNOSTIC_END(NH_TERMINAL_SUCCESS)
}

