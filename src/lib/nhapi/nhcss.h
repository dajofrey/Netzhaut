#ifndef NH_CSS_H
#define NH_CSS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhhtml.h"

#endif

/** @defgroup api_nhcss nhcss
 *  @brief Create CSS based layout engines for rendering HTML documents.
 */

/** @addtogroup api_nhcss 
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_CSS_RESULT
    {
        NH_CSS_SUCCESS, /**<Indicates that something worked as planned.*/
        NH_CSS_ERROR_NULL_POINTER,
        NH_CSS_ERROR_BAD_STATE,
        NH_CSS_ERROR_MEMORY_ALLOCATION,
        NH_CSS_SIGNAL_SPLIT_INLINE_BOX,

    } NH_CSS_RESULT;

// TYPEDEFS ========================================================================================

    typedef struct nh_css_LayoutEngine nh_css_LayoutEngine;

// STRUCTS =========================================================================================

    typedef struct nh_css_CanvasType {
        nh_PixelSize Size;
    } nh_css_CanvasType;

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_css_LayoutEngine *nh_css_createLayoutEngine(
        nh_html_DocumentContext *DocumentContext_p
    );

    /**
     * Todo.
     */
    nh_css_CanvasType nh_css_createCanvasType(
        nh_PixelSize Size 
    );

    /**
     * Todo.
     */
    NH_CSS_RESULT nh_css_addCanvasType(
        nh_css_LayoutEngine *LayoutEngine_p, nh_css_CanvasType CanvasType
    );

/** @} */

#endif // NH_CSS_H
