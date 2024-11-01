#ifndef NH_CSS_STYLE_DECLARATION_H
#define NH_CSS_STYLE_DECLARATION_H

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

    nh_webidl_Object *nh_css_createStyleDeclaration(
        nh_core_Array Declarations
    );

    nh_core_Array *nh_css_getStyleDeclarationData(
        nh_webidl_Object *Declaration_p
    );

/** @} */

#endif
