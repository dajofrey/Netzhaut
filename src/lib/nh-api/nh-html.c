// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-html.h"
#include "nh-core.h"

#include "../nh-core/Loader/Loader.h"
#include "../nh-html/Main/DocumentContext.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_api_DocumentContext *nh_api_createDocumentContext(
    bool browsingContext)
{
    typedef nh_api_DocumentContext *(*nh_html_createDocumentContext_f)(bool browsingContext);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_html_createDocumentContext_f createDocumentContext_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_HTML, 0, "nh_html_createDocumentContext");
    return createDocumentContext_f ? createDocumentContext_f(browsingContext) : NULL;
}

NH_API_RESULT nh_api_loadBytes(
    nh_api_DocumentContext *Context_p, char *bytes_p, unsigned long long size)
{
    typedef NH_API_RESULT (*nh_html_loadBytes_f)(nh_api_DocumentContext *Context_p, char *bytes_p, unsigned long long size);
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_html_loadBytes_f loadBytes_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_HTML, 0, "nh_html_loadBytes");
    return loadBytes_f ? loadBytes_f(Context_p, bytes_p, size) : NH_API_ERROR_BAD_STATE;
}
