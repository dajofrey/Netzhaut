// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Document.h"

#include "../../nh-webidl/Runtime/String.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define MODE ((nh_dom_Document*)Document_p->internal_p)->Mode_p
#define DOCTYPE Document_p->Attributes.pp[9]

typedef struct nh_dom_Document {
    nh_webidl_DOMString *Mode_p;
} nh_dom_Document;

// DATA ============================================================================================

nh_webidl_DOMString NH_DOM_DOCUMENT_MODE_NO_QUIRKS;
nh_webidl_DOMString NH_DOM_DOCUMENT_MODE_QUIRKS;
nh_webidl_DOMString NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_dom_initializeDocument(
    nh_webidl_Object *Document_p)
{
    Document_p->internal_p = nh_core_allocate(sizeof(nh_dom_Document));
    NH_CORE_CHECK_MEM(Document_p->internal_p)

    MODE = &NH_DOM_DOCUMENT_MODE_NO_QUIRKS;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_initDocumentModes()
{
    NH_DOM_DOCUMENT_MODE_NO_QUIRKS = nh_webidl_initDOMString(16);
    NH_DOM_DOCUMENT_MODE_QUIRKS = nh_webidl_initDOMString(16);
    NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS = nh_webidl_initDOMString(16);

    nh_webidl_appendToDOMString(&NH_DOM_DOCUMENT_MODE_NO_QUIRKS, "no-quirks", 9);
    nh_webidl_appendToDOMString(&NH_DOM_DOCUMENT_MODE_QUIRKS, "quirks", 6);
    nh_webidl_appendToDOMString(&NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS, "limited-quirks", 14);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_freeDocumentModes()
{
    nh_webidl_freeDOMString(&NH_DOM_DOCUMENT_MODE_NO_QUIRKS);
    nh_webidl_freeDOMString(&NH_DOM_DOCUMENT_MODE_QUIRKS);
    nh_webidl_freeDOMString(&NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_setDocumentMode(
    nh_webidl_Object *Document_p, nh_webidl_DOMString *Mode_p)
{
    MODE = Mode_p;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_setDocumentType(
    nh_webidl_Object *Document_p, nh_webidl_Object *DocumentType_p)
{
    DOCTYPE = DocumentType_p;

    return NH_API_SUCCESS;
}

