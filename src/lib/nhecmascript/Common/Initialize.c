// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "Macros.h"
#include "IndexMap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_ECMASCRIPT_RESULT nh_ecmascript_initialize()
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_CHECK(nh_ecmascript_createIndexMap())

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

