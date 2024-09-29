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
#include "Log.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_encoding_initialize()
{
NH_ENCODING_BEGIN()

    NH_ENCODING_CHECK(nh_encoding_logUnicodeCodepointDescriptions())
    NH_ENCODING_CHECK(nh_encoding_createIndexMap())

NH_ENCODING_DIAGNOSTIC_END(NH_API_SUCCESS)
}

