// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Document.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhdom/Interfaces/Node.h"
#include "../../nhwebidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_HTML_RESULT nh_html_initializeDocument(
    nh_webidl_Object *Document_p)
{
NH_HTML_BEGIN()
NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// API =============================================================================================

nh_webidl_Object *nh_html_createDocument(
    nh_html_DocumentContext *Context_p)
{
NH_HTML_BEGIN()

    nh_webidl_Object *Document_p = nh_webidl_createCompositeObject("HTML", "Document");
    NH_HTML_CHECK_MEM_2(NULL, Document_p)

    // The node document of a document is that document itself. All nodes have a node document at all times. 
    nh_dom_setNodeDocument(nh_dom_getNode(Document_p), Document_p);

NH_HTML_END(Document_p)
}

nh_webidl_Object *nh_html_getDocument(
    nh_webidl_Object *Object_p)
{
NH_HTML_BEGIN()
NH_HTML_END(nh_webidl_getObject(Object_p, "HTML", "Document"))
}

