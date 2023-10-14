#ifndef NH_CSS_DOCUMENT_H
#define NH_CSS_DOCUMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Main/LayoutEngine.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    nh_css_DocumentObject *nh_css_getDocument(
        nh_webidl_Object *Object_p
    );
    
    nh_css_Layout *nh_css_getLayout(
        nh_css_DocumentObject *Document_p
    );

    nh_css_StyleSheetListObject *nh_css_getStyleSheetList(
        nh_css_DocumentObject *Document_p
    );

/** @} */

#endif
