// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "IndexMap.h"

#include "../Main/DocumentContext.h"
#include "../Parser/NamedCharacterReferences.h"

#include "../../nh-core/Util/List.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_html_initialize()
{
    NH_HTML_DOCUMENT_CONTEXTS = nh_core_initList(16);

    NH_CORE_CHECK(nh_html_createIndexMap())
    NH_CORE_CHECK(nh_html_initCharacterReferencesTrie())

    return NH_API_SUCCESS;
}

