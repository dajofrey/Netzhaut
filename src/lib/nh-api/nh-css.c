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

nh_api_LayoutEngine *nh_api_createLayoutEngine(
    nh_api_DocumentContext *DocumentContext_p)
{
    nh_css_createLayoutEngine_f createLayoutEngine_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_createLayoutEngine");
    return createLayoutEngine_f ? createLayoutEngine_f(DocumentContext_p) : NULL;
}

nh_api_CanvasType nh_api_createCanvasType(
    nh_api_PixelSize Size)
{
    nh_css_createCanvasType_f createCanvasType_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_createCanvasType");
    nh_api_CanvasType CanvasType;
    memset(&CanvasType, 0, sizeof(nh_api_CanvasType));
    return createCanvasType_f ? createCanvasType_f(Size) : CanvasType;
}

NH_API_RESULT nh_api_addCanvasType(
    nh_api_LayoutEngine *LayoutEngine_p, nh_api_CanvasType CanvasType)
{
    nh_css_addCanvasType_f addCanvasType_f = !NH_LOADER_P || !LayoutEngine_p ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_addCanvasType");
    return addCanvasType_f ? addCanvasType_f(LayoutEngine_p, CanvasType) : NH_API_ERROR_BAD_STATE;
}

