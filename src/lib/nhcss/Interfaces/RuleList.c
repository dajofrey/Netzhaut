// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "RuleList.h"

#include "../Common/Macros.h"

#include "../../nhcore/Util/List.h"
#include "../../nhcore/System/Memory.h"

#include "../../nhwebidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_CSS_RESULT nh_css_initializeCSSRuleList(
    nh_webidl_Object *RuleList_p)
{
NH_CSS_BEGIN()

    RuleList_p->internal_p = nh_core_allocate(sizeof(nh_List));
    NH_CSS_CHECK_MEM(RuleList_p->internal_p)

    *((nh_List*)RuleList_p->internal_p) = nh_core_initList(16);

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

// API =============================================================================================

NH_CSS_RESULT nh_css_appendToRuleList(
    nh_css_RuleListObject *RuleList_p, nh_webidl_Object *Object_p)
{
NH_CSS_BEGIN()

    if (nh_webidl_getObject(Object_p, "CSS", "CSSRule") == NULL) {
        NH_CSS_END(NH_CSS_ERROR_BAD_STATE)
    }

    nh_core_appendToList(((nh_webidl_Object*)RuleList_p)->internal_p, Object_p);

NH_CSS_END(NH_CSS_SUCCESS)
}

nh_List *nh_css_getRuleListData(
    nh_css_RuleListObject *RuleList_p)
{
NH_CSS_BEGIN()
NH_CSS_END(((nh_webidl_Object*)RuleList_p)->internal_p)
}

