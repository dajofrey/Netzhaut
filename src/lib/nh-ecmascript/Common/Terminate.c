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

NH_API_RESULT nh_ecmascript_terminate()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_freeIndexMap();

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_API_SUCCESS)
}

