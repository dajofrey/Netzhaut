// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Document.h"
#include "StyleSheetList.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_css_initializeDocument(
    nh_webidl_Object *Document_p)
{
    Document_p->Attributes.pp[0] = nh_webidl_createObject("CSS", "StyleSheetList");
    NH_CORE_CHECK_NULL(Document_p->Attributes.pp[0])
    return NH_API_SUCCESS;
}

nh_webidl_Object *nh_css_getDocument(
    nh_webidl_Object *Object_p)
{
    return nh_webidl_getObject(Object_p, "CSS", "Document");
}

nh_css_Layout *nh_css_getLayout(
    nh_webidl_Object *Document_p)
{
    return Document_p->internal_p;
}

nh_css_StyleSheetListObject *nh_css_getStyleSheetList(
    nh_webidl_Object *Document_p)
{
    return Document_p->Attributes.pp[0];
}
