// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"

#include "../Base/UnicodeData.gen.h"
#include "../../nh-core/Util/Debug.h"

#include <stdio.h>
#include <string.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_encoding_logUnicodeCodepointDescriptions()
{
    for (int i = 0; i < NH_ENCODING_UNICODE_DATA_COUNT; ++i) {
        nh_core_log("nh-encoding:Unicode", NULL, (char*)NH_ENCODING_UNICODE_DATA_PP[i]);
    }
    return NH_API_SUCCESS;
}

