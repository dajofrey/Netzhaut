// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "StyleSheetList.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_css_initializeStyleSheetList(
    nh_webidl_Object *StyleSheetList_p)
{
    StyleSheetList_p->internal_p = nh_core_allocate(sizeof(nh_core_List));
    NH_CORE_CHECK_MEM(StyleSheetList_p->internal_p)

    *((nh_core_List*)StyleSheetList_p->internal_p) = nh_core_initList(8);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_appendToStyleSheetList(
    nh_webidl_Object *StyleSheetList_p, nh_webidl_Object *StyleSheet_p)
{
    nh_core_appendToList(((nh_webidl_Object*)StyleSheetList_p)->internal_p, StyleSheet_p);

    return NH_API_SUCCESS;
}

nh_core_List *nh_css_getStyleSheetListData(
    nh_webidl_Object *StyleSheetList_p)
{
    return ((nh_webidl_Object*)StyleSheetList_p)->internal_p;
}

