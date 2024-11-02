// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "RuleList.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_css_initializeCSSRuleList(
    nh_webidl_Object *RuleList_p)
{
    RuleList_p->internal_p = nh_core_allocate(sizeof(nh_core_List));
    NH_CORE_CHECK_MEM(RuleList_p->internal_p)

    *((nh_core_List*)RuleList_p->internal_p) = nh_core_initList(16);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_css_appendToRuleList(
    nh_css_RuleListObject *RuleList_p, nh_webidl_Object *Object_p)
{
    if (nh_webidl_getObject(Object_p, "CSS", "CSSRule") == NULL) {
        return NH_API_ERROR_BAD_STATE;
    }

    nh_core_appendToList(((nh_webidl_Object*)RuleList_p)->internal_p, Object_p);

    return NH_API_SUCCESS;
}

nh_core_List *nh_css_getRuleListData(
    nh_webidl_Object *RuleList_p)
{
    return RuleList_p->internal_p;
}
