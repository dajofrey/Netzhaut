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

#include "Data/default.css.inc"

#include "../Parser/Parser.h"
#include "../Properties/Properties.h"

#include "../../nhcore/System/Logger.h"
#include "../../nhcore/System/Memory.h"

#include "../../nhencoding/Encodings/UTF8.h"
#include "../../nhencoding/Encodings/UTF32.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_CSS_RESULT nh_css_initialize()
{
NH_CSS_BEGIN()

    NH_CSS_CHECK(nh_css_createIndexMap())
    NH_CSS_CHECK(nh_css_createInitialPropertyValues())

    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(default_css_inc, default_css_inc_len, NULL);

    NH_CSS_DEFAULT_STYLE_SHEET_P = nh_css_parseStyleSheetFromUTF32(&String, NULL);
    NH_CSS_CHECK_NULL(NH_CSS_DEFAULT_STYLE_SHEET_P)

    nh_encoding_freeUTF32(&String);

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

