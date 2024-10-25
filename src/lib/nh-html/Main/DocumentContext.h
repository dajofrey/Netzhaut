#ifndef NH_HTML_DOCUMENT_CONTEXT_H
#define NH_HTML_DOCUMENT_CONTEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-html_structs
 *  @{
 */

    typedef struct nh_html_DocumentContext {
        nh_core_List Documents;
        nh_webidl_Object *Document_p;
        bool scripting;
        bool browsingContext;
        nh_api_LayoutEngine *LayoutEngine_p;
    } nh_html_DocumentContext;

/** @} */

/** @addtogroup lib_nh-html_vars
 *  @{
 */

    extern nh_core_List NH_HTML_DOCUMENT_CONTEXTS;

/** @} */

/** @addtogroup lib_nh-html_typedefs
 *  @{
 */

    typedef nh_html_DocumentContext *(*nh_html_createDocumentContext_f)(
        bool browsingContext
    );

    typedef NH_API_RESULT (*nh_html_loadBytes_f)(
        nh_html_DocumentContext *Context_p, char *bytes_p, unsigned long long size
    );

/** @} */

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    nh_html_DocumentContext *nh_html_createDocumentContext(
        bool browsingContext
    );

    NH_API_RESULT nh_html_loadBytes(
        nh_html_DocumentContext *Context_p, char *bytes_p, unsigned long long size
    );

/** @} */

#endif
