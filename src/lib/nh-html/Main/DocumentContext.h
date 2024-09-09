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
        nh_List Documents;
        nh_webidl_Object *Document_p;
        NH_BOOL scripting;
        NH_BOOL browsingContext;
        nh_css_LayoutEngine *LayoutEngine_p;
    } nh_html_DocumentContext;

/** @} */

/** @addtogroup lib_nh-html_vars
 *  @{
 */

    extern nh_List NH_HTML_DOCUMENT_CONTEXTS;

/** @} */

/** @addtogroup lib_nh-html_typedefs
 *  @{
 */

    typedef nh_html_DocumentContext *(*nh_html_createDocumentContext_f)(
        NH_BOOL browsingContext
    );

    typedef NH_HTML_RESULT (*nh_html_loadBytes_f)(
        nh_html_DocumentContext *Context_p, NH_BYTE *bytes_p, unsigned long long size
    );

/** @} */

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    nh_html_DocumentContext *nh_html_createDocumentContext(
        NH_BOOL browsingContext
    );

    NH_HTML_RESULT nh_html_loadBytes(
        nh_html_DocumentContext *Context_p, NH_BYTE *bytes_p, unsigned long long size
    );

/** @} */

#endif
