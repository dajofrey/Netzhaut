// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "../Interfaces/Document.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_dom_terminate()
{
    NH_CORE_CHECK(nh_dom_freeDocumentModes())
    return NH_API_SUCCESS;
}

