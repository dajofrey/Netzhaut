#ifndef NH_HTML_DOCUMENT_H
#define NH_HTML_DOCUMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Main/DocumentContext.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    nh_webidl_Object *nh_html_createDocument(
        nh_html_DocumentContext *Context_p
    );

    nh_webidl_Object *nh_html_getDocument(
        nh_webidl_Object *Object_p
    );

/** @} */

#endif
