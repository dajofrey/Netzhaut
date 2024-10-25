// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "IndexMap.h"
#include "Log.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_encoding_initialize()
{
    NH_CORE_CHECK(nh_encoding_logUnicodeCodepointDescriptions())
    NH_CORE_CHECK(nh_encoding_createIndexMap())
    return NH_API_SUCCESS;
}

