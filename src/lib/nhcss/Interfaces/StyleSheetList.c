// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "StyleSheetList.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/List.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_CSS_RESULT nh_css_initializeStyleSheetList(
    nh_webidl_Object *StyleSheetList_p)
{
NH_CSS_BEGIN()

    StyleSheetList_p->internal_p = nh_core_allocate(sizeof(nh_List));
    NH_CSS_CHECK_MEM(StyleSheetList_p->internal_p)

    *((nh_List*)StyleSheetList_p->internal_p) = nh_core_initList(8);

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

// API =============================================================================================

NH_CSS_RESULT nh_css_appendToStyleSheetList(
    nh_css_StyleSheetListObject *StyleSheetList_p, nh_css_StyleSheetObject *StyleSheet_p)
{
NH_CSS_BEGIN()

    nh_core_appendToList(((nh_webidl_Object*)StyleSheetList_p)->internal_p, StyleSheet_p);

NH_CSS_END(NH_CSS_SUCCESS)
}

nh_List *nh_css_getStyleSheetListData(
    nh_css_StyleSheetListObject *StyleSheetList_p)
{
NH_CSS_BEGIN()
NH_CSS_END(((nh_webidl_Object*)StyleSheetList_p)->internal_p)
}

