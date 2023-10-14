#ifndef NH_HTML_H
#define NH_HTML_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhgfx.h"

#endif

/** @defgroup api_nhhtml nhhtml
 *  @brief Create HTML document contexts.
 */

/** @addtogroup api_nhhtml
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_HTML_RESULT
    {
        NH_HTML_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_HTML_ERROR_NULL_POINTER,
        NH_HTML_ERROR_BAD_STATE,
        NH_HTML_ERROR_MEMORY_ALLOCATION,

    } NH_HTML_RESULT;

// TYPEDEFS ========================================================================================

    typedef struct nh_html_DocumentContext nh_html_DocumentContext;
    typedef struct nh_html_HTMLElement nh_html_HTMLElement;
    typedef struct nh_html_HTMLImageElement nh_html_HTMLImageElement;

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_html_DocumentContext *nh_api_createDocumentContext(
        NH_BOOL browsingContext
    );

    /**
     * Todo.
     */
    NH_HTML_RESULT nh_api_loadBytes(
        nh_html_DocumentContext *Context_p, NH_BYTE *bytes_p, unsigned long long size
    );

/** @} */

#endif // NH_HTML_H
