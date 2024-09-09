// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "StyleDeclaration.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// API =============================================================================================

nh_css_StyleDeclarationObject *nh_css_createStyleDeclaration(
    nh_Array Declarations)
{
NH_CSS_BEGIN()

    nh_webidl_Object *StyleDeclaration_p = nh_webidl_createObject("CSS", "CSSStyleDeclaration");
    NH_CSS_CHECK_MEM_2(NULL, StyleDeclaration_p)
    
    StyleDeclaration_p->internal_p = nh_core_allocate(sizeof(nh_Array));
    NH_CSS_CHECK_MEM_2(NULL, StyleDeclaration_p->internal_p)

    *((nh_Array*)StyleDeclaration_p->internal_p) = Declarations;

NH_CSS_END((nh_css_StyleDeclarationObject*)StyleDeclaration_p)
}

nh_Array *nh_css_getStyleDeclarationData(
    nh_css_StyleDeclarationObject *Declaration_p)
{
NH_CSS_BEGIN()
NH_CSS_END(((nh_webidl_Object*)Declaration_p)->internal_p)
}

