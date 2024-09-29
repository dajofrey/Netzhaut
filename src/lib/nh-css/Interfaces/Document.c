// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Document.h"
#include "StyleSheetList.h"

#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_API_RESULT nh_css_initializeDocument(
    nh_css_DocumentObject *Document_p)
{
NH_CSS_BEGIN()

    ((nh_webidl_Object*)Document_p)->Attributes.pp[0] = nh_webidl_createObject("CSS", "StyleSheetList");
    NH_CSS_CHECK_NULL(((nh_webidl_Object*)Document_p)->Attributes.pp[0])

NH_CSS_END(NH_API_SUCCESS)
}

// API =============================================================================================

nh_css_DocumentObject *nh_css_getDocument(
    nh_webidl_Object *Object_p)
{
NH_CSS_BEGIN()
NH_CSS_END((nh_css_DocumentObject*)nh_webidl_getObject(Object_p, "CSS", "Document"))
}

nh_css_Layout *nh_css_getLayout(
    nh_css_DocumentObject *Document_p)
{
NH_CSS_BEGIN()
NH_CSS_END(((nh_webidl_Object*)Document_p)->internal_p)
}

nh_css_StyleSheetListObject *nh_css_getStyleSheetList(
    nh_css_DocumentObject *Document_p)
{
NH_CSS_BEGIN()
NH_CSS_END(((nh_webidl_Object*)Document_p)->Attributes.pp[0])
}

