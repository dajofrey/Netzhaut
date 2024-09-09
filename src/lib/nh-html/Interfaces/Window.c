// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Document.h"

#include "../Common/Macros.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

// INITIALIZE ======================================================================================

NH_HTML_RESULT nh_html_initializeDocument(
    nh_webidl_Object *Document_p)
{
NH_HTML_BEGIN()

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// API =============================================================================================

nh_webidl_Object *nh_html_createDocument()
{
NH_HTML_BEGIN()

    nh_webidl_Object *Document_p = nh_webidl_createCompositeObject("HTML", "Document");

    // The node document of a document is that document itself. All nodes have a node document at all times. 
    nh_dom_setNodeDocument(nh_dom_getNode(Document_p), Document_p);

NH_HTML_END(Document_p)
}

