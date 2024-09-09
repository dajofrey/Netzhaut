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

#include NH_ENCODING_FLOW
#include NH_ENCODING_DEFAULT_CHECK

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_ENCODING_RESULT nh_encoding_terminate()
{
NH_ENCODING_BEGIN()

    nh_encoding_freeIndexMap();

NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_SUCCESS)
}

