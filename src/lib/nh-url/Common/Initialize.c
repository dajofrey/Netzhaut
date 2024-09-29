// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "Log.h"
#include "Macros.h"

#include "../IDNA/IDNAMappingTable.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_url_initialize()
{
NH_URL_BEGIN()

    NH_URL_CHECK(nh_url_parseIDNAMappingTable())
    NH_URL_CHECK(nh_url_logIDNAMappingTable(&NH_URL_IDNA_MAPPING_TABLE))

NH_URL_DIAGNOSTIC_END(NH_API_SUCCESS)
}

