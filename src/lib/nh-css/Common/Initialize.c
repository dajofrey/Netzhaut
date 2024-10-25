// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Initialize.h"
#include "IndexMap.h"

#include "Data/default.css.inc"

#include "../Parser/Parser.h"
#include "../Properties/Properties.h"

#include "../../nh-core/System/Logger.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-encoding/Encodings/UTF8.h"
#include "../../nh-encoding/Encodings/UTF32.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_API_RESULT nh_css_initialize()
{
    NH_CORE_CHECK(nh_css_createIndexMap())
    NH_CORE_CHECK(nh_css_createInitialPropertyValues())

    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(default_css_inc, default_css_inc_len, NULL);
    NH_CSS_DEFAULT_STYLE_SHEET_P = nh_css_parseStyleSheetFromUTF32(&String, NULL);

    NH_CORE_CHECK_NULL(NH_CSS_DEFAULT_STYLE_SHEET_P)

    nh_encoding_freeUTF32(&String);

    return NH_API_SUCCESS;
}
