// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Document.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_API_RESULT nh_html_initializeDocument(
    nh_webidl_Object *Document_p)
{
    return NH_API_SUCCESS;
}

// API =============================================================================================

nh_webidl_Object *nh_html_createDocument(
    nh_html_DocumentContext *Context_p)
{
    nh_webidl_Object *Document_p = nh_webidl_createCompositeObject("HTML", "Document");
    NH_CORE_CHECK_MEM_2(NULL, Document_p)

    // The node document of a document is that document itself. 
    // All nodes have a node document at all times. 
    nh_dom_setNodeDocument(NH_WEBIDL_GET_DOM_NODE(Document_p), Document_p);

    return Document_p;
}
