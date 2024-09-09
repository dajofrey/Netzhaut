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

#include "../Runtime/Runtime.h"
#include "../Runtime/String.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_WEBIDL_RESULT nh_webidl_initialize()
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK(nh_webidl_createIndexMap())
    NH_WEBIDL_CHECK(nh_webidl_initRuntime())
    NH_WEBIDL_CHECK(nh_webidl_initNamespaces())

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

