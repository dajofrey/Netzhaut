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
#include "../../nh-html/Main/DocumentContext.h"

#endif

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_Layout {
        bool initialized;
        nh_List Canvases;
    } nh_css_Layout;

    typedef struct nh_css_LayoutEngine {
        nh_Array CanvasTypes;
        nh_html_DocumentContext *DocumentContext_p;
        nh_css_Layout *Layout_p;
    } nh_css_LayoutEngine;

/** @} */

/** @addtogroup lib_nh-css_typedefs
 *  @{
 */

    typedef nh_css_LayoutEngine *(*nh_css_createLayoutEngine_f)(
        nh_html_DocumentContext *Context_p
    );

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_LayoutEngine *nh_css_createLayoutEngine(
        nh_html_DocumentContext *Context_p
    );

/** @} */

#endif
