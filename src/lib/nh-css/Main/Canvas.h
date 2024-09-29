#ifndef NH_CSS_CANVAS_H
#define NH_CSS_CANVAS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "LayoutEngine.h"
#include "FragmentTree.h"

#endif

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_Canvas {
        nh_api_CanvasType Type;
        nh_css_SourceTree SourceTree;
        nh_css_BoxTree BoxTree;
        nh_css_FragmentTree FragmentTree;
        bool render;
        bool _float;
        nh_List Floats;
        nh_Array LineBoxes;
    } nh_css_Canvas;

/** @} */

/** @addtogroup lib_nh-css_typedefs
 *  @{
 */

    typedef nh_api_CanvasType (*nh_css_createCanvasType_f)(
        nh_api_PixelSize Size 
    );

    typedef NH_API_RESULT (*nh_css_addCanvasType_f)(
        nh_css_LayoutEngine *LayoutEngine_p, nh_api_CanvasType CanvasType
    );

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_Canvas *nh_css_addCanvas(
        nh_css_Layout *Layout_p, nh_api_CanvasType Type 
    );

    NH_API_RESULT nh_css_computeCanvas(
        nh_css_Canvas *Canvas_p, nh_webidl_Object *HTMLElement_p, nh_css_StyleSheetListObject *StyleSheets_p
    );

    nh_api_CanvasType nh_css_createCanvasType(
        nh_api_PixelSize Size
    );

    NH_API_RESULT nh_css_addCanvasType(
        nh_css_LayoutEngine *LayoutEngine_p, nh_api_CanvasType CanvasType 
    );

/** @} */

#endif
