#ifndef NH_CSS_STYLE_SHEET_LIST_H
#define NH_CSS_STYLE_SHEET_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    NH_API_RESULT nh_css_appendToStyleSheetList(
        nh_ecmascript_Object *StyleSheetList_p, nh_ecmascript_Object *StyleSheet_p
    );

    nh_core_List *nh_css_getStyleSheetListData(
        nh_ecmascript_Object *StyleSheetList_p
    );

/** @} */

#endif
