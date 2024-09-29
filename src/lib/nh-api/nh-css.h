#ifndef NH_API_NH_CSS_H
#define NH_API_NH_CSS_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nh-html.h"

// TYPEDEFS ========================================================================================

    typedef struct nh_api_LayoutEngine nh_api_LayoutEngine;

// STRUCTS =========================================================================================

    typedef struct nh_api_CanvasType {
        nh_api_PixelSize Size;
    } nh_api_CanvasType;

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_api_LayoutEngine *nh_api_createLayoutEngine(
        nh_api_DocumentContext *DocumentContext_p
    );

    /**
     * Todo.
     */
    nh_api_CanvasType nh_api_createCanvasType(
        nh_api_PixelSize Size 
    );

    /**
     * Todo.
     */
    NH_API_RESULT nh_api_addCanvasType(
        nh_api_LayoutEngine *LayoutEngine_p, nh_api_CanvasType CanvasType
    );

#endif // NH_API_NH_CSS_H
