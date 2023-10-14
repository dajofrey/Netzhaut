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

NH_HTML_RESULT nh_html_terminate()
{
NH_HTML_BEGIN()

    nh_html_freeIndexMap();

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

