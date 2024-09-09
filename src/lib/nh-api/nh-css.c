// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-css.h"

#include "../nh-core/Loader/Loader.h"
#include "../nh-css/Main/LayoutEngine.h"
#include "../nh-css/Main/Canvas.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

nh_css_LayoutEngine *nh_api_createLayoutEngine(
    nh_html_DocumentContext *DocumentContext_p)
{
    nh_css_createLayoutEngine_f createLayoutEngine_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_createLayoutEngine");
    return createLayoutEngine_f ? createLayoutEngine_f(DocumentContext_p) : NULL;
}

nh_css_CanvasType nh_api_createCanvasType(
    nh_PixelSize Size)
{
    nh_css_createCanvasType_f createCanvasType_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_createCanvasType");
    nh_css_CanvasType CanvasType;
    memset(&CanvasType, 0, sizeof(nh_css_CanvasType));
    return createCanvasType_f ? createCanvasType_f(Size) : CanvasType;
}

NH_CSS_RESULT nh_api_addCanvasType(
    nh_css_LayoutEngine *LayoutEngine_p, nh_css_CanvasType CanvasType)
{
    nh_css_addCanvasType_f addCanvasType_f = !NH_LOADER_P || !LayoutEngine_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_addCanvasType");
    return addCanvasType_f ? addCanvasType_f(LayoutEngine_p, CanvasType) : NH_CSS_ERROR_BAD_STATE;
}

