// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Window.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

nh_api_HostGlobals nh_html_getHTMLGlobals()
{
    nh_api_HostGlobals Globals;
}

nh_ecmascript_Object *nh_html_createDocument()
{
    nh_ecmascript_Object *Document_p = nh_webidl_createCompositeObject("HTML", "Document");

    // The node document of a document is that document itself. All nodes have a node document at all times. 
    nh_dom_setNodeDocument(NH_WEBIDL_GET_DOM_NODE(Document_p), Document_p);

    return Document_p;
}

void *nh_html_openWindow(
    nh_ecmascript_Agent *Agent_p)
{
    
}
