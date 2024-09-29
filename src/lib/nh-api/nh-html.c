// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-html.h"

#include "../nh-core/Loader/Loader.h"
#include "../nh-html/Main/DocumentContext.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

nh_api_DocumentContext *nh_api_createDocumentContext(
    bool browsingContext)
{
    nh_html_createDocumentContext_f createDocumentContext_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_HTML, 0, "nh_html_createDocumentContext");
    return createDocumentContext_f ? createDocumentContext_f(browsingContext) : NULL;
}

// LOAD ============================================================================================

NH_API_RESULT nh_api_loadBytes(
    nh_api_DocumentContext *Context_p, char *bytes_p, unsigned long long size)
{
    nh_html_loadBytes_f loadBytes_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_HTML, 0, "nh_html_loadBytes");
    return loadBytes_f ? loadBytes_f(Context_p, bytes_p, size) : NH_API_ERROR_BAD_STATE;
}

