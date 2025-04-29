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

// FUNCTIONS =======================================================================================

nh_api_LayoutEngine *nh_api_createLayoutEngine(
    nh_api_DocumentContext *DocumentContext_p)
{
    typedef nh_api_LayoutEngine *(*nh_css_createLayoutEngine_f)(nh_api_DocumentContext *Context_p);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_css_createLayoutEngine_f createLayoutEngine_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_createLayoutEngine");
    return createLayoutEngine_f ? createLayoutEngine_f(DocumentContext_p) : NULL;
}

nh_api_CanvasType nh_api_createCanvasType(
    nh_api_PixelSize Size)
{
    nh_core_Loader *Loader_p = nh_api_getLoader();
    typedef nh_api_CanvasType (*nh_css_createCanvasType_f)(nh_api_PixelSize Size);
    nh_css_createCanvasType_f createCanvasType_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_createCanvasType");
    nh_api_CanvasType CanvasType;
    memset(&CanvasType, 0, sizeof(nh_api_CanvasType));
    return createCanvasType_f ? createCanvasType_f(Size) : CanvasType;
}

NH_API_RESULT nh_api_addCanvasType(
    nh_api_LayoutEngine *LayoutEngine_p, nh_api_CanvasType CanvasType)
{
    typedef NH_API_RESULT (*nh_css_addCanvasType_f)(nh_api_LayoutEngine *LayoutEngine_p, nh_api_CanvasType CanvasType);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_css_addCanvasType_f addCanvasType_f = !Loader_p || !LayoutEngine_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_addCanvasType");
    return addCanvasType_f ? addCanvasType_f(LayoutEngine_p, CanvasType) : NH_API_ERROR_BAD_STATE;
}

nh_api_StyleSheet *nh_api_parseStyleSheet(
    char *data_p, size_t length)
{
    typedef nh_api_StyleSheet *(*nh_css_parseStyleSheetFromUTF8_f)(char *data_p, size_t length, nh_webidl_Object *Document_pi); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_css_parseStyleSheetFromUTF8_f parseStyleSheet_f = !Loader_p || !data_p || length == 0 ? NULL : Loader_p->loadSymbol_f(NH_MODULE_CSS, 0, "nh_css_parseStyleSheetFromUTF8");
    return parseStyleSheet_f ? parseStyleSheet_f(data_p, length, NULL) : NULL;
}
