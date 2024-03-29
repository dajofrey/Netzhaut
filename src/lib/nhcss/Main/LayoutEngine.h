#ifndef NH_CSS_LAYOUT_ENGINE_H
#define NH_CSS_LAYOUT_ENGINE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "BoxTree.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhcss_structs
 *  @{
 */

    typedef struct nh_css_Layout {
        NH_BOOL initialized;
        nh_List Canvases;
    } nh_css_Layout;

    typedef struct nh_css_LayoutEngine {
        nh_Array CanvasTypes;
        nh_html_DocumentContext *DocumentContext_p;
        nh_css_Layout *Layout_p;
    } nh_css_LayoutEngine;

/** @} */

/** @addtogroup lib_nhcss_typedefs
 *  @{
 */

    typedef nh_css_LayoutEngine *(*nh_css_createLayoutEngine_f)(
        nh_html_DocumentContext *Context_p
    );

/** @} */

/** @addtogroup lib_nhcss_functions
 *  @{
 */

    nh_css_LayoutEngine *nh_css_createLayoutEngine(
        nh_html_DocumentContext *Context_p
    );

/** @} */

#endif
