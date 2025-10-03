#ifndef NH_API_NH_HTML_H
#define NH_API_NH_HTML_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-ecmascript.h"
#include "nh-gfx.h"

// STRUCTS =========================================================================================

/**
 * Opaque handle for a Document context.
 */
typedef struct nh_api_DocumentContext nh_api_DocumentContext;

// FUNCTIONS =======================================================================================

nh_api_HostGlobals nh_api_getHTMLGlobals(
);

NH_API_RESULT nh_api_freeHTMLGlobals(
    nh_api_HostGlobals *Globals_p
);

nh_api_DocumentContext *nh_api_createDocumentContext(
    bool browsingContext
);

NH_API_RESULT nh_api_loadBytes(
    nh_api_DocumentContext *Context_p, char *bytes_p, unsigned long long size
);

#endif // NH_API_NH_HTML_H
