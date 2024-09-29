// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Terminate.h"
#include "Macros.h"

#include "../Interfaces/Document.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_dom_terminate()
{
NH_DOM_BEGIN()

    NH_DOM_CHECK(nh_dom_freeDocumentModes())

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

