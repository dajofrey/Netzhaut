// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "StyleDeclaration.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

nh_webidl_Object *nh_css_createStyleDeclaration(
    nh_core_Array Declarations)
{
    nh_webidl_Object *StyleDeclaration_p = nh_webidl_createObject("CSS", "CSSStyleDeclaration");
    NH_CORE_CHECK_MEM_2(NULL, StyleDeclaration_p)
    
    StyleDeclaration_p->internal_p = nh_core_allocate(sizeof(nh_core_Array));
    NH_CORE_CHECK_MEM_2(NULL, StyleDeclaration_p->internal_p)

    *((nh_core_Array*)StyleDeclaration_p->internal_p) = Declarations;

    return StyleDeclaration_p;
}

nh_core_Array *nh_css_getStyleDeclarationData(
    nh_webidl_Object *CSSStyleDeclaration_p)
{
    return CSSStyleDeclaration_p->internal_p;
}
