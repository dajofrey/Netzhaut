#ifndef NH_CSS_INTERFACES_DOCUMENT_H
#define NH_CSS_INTERFACES_DOCUMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Main/LayoutEngine.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_ecmascript_Object *nh_css_getDocument(
        nh_ecmascript_Object *Object_p
    );
    
    nh_css_Layout *nh_css_getLayout(
        nh_ecmascript_Object *Document_p
    );

    nh_ecmascript_Object *nh_css_getStyleSheetList(
        nh_ecmascript_Object *Document_p
    );

/** @} */

#endif
