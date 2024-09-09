// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Document.h"

#include "../Common/Macros.h"

#include "../../nh-webidl/Runtime/String.h"
#include "../../nh-webidl/Common/Macros.h"

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

// INITIALIZE ======================================================================================

NH_DOM_RESULT nh_dom_initializeDocument(
    nh_webidl_Object *Document_p)
{
NH_DOM_BEGIN()

    Document_p->internal_p = nh_core_allocate(sizeof(nh_dom_Document));
    NH_DOM_CHECK_MEM(Document_p->internal_p)

    MODE = &NH_DOM_DOCUMENT_MODE_NO_QUIRKS;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT nh_dom_initDocumentModes()
{
NH_DOM_BEGIN()

    NH_DOM_DOCUMENT_MODE_NO_QUIRKS = nh_webidl_initDOMString(16);
    NH_DOM_DOCUMENT_MODE_QUIRKS = nh_webidl_initDOMString(16);
    NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS = nh_webidl_initDOMString(16);

    nh_webidl_appendToDOMString(&NH_DOM_DOCUMENT_MODE_NO_QUIRKS, "no-quirks", 9);
    nh_webidl_appendToDOMString(&NH_DOM_DOCUMENT_MODE_QUIRKS, "quirks", 6);
    nh_webidl_appendToDOMString(&NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS, "limited-quirks", 14);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT nh_dom_freeDocumentModes()
{
NH_DOM_BEGIN()

    nh_webidl_freeDOMString(&NH_DOM_DOCUMENT_MODE_NO_QUIRKS);
    nh_webidl_freeDOMString(&NH_DOM_DOCUMENT_MODE_QUIRKS);
    nh_webidl_freeDOMString(&NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT nh_dom_setDocumentMode(
    nh_webidl_Object *Document_p, nh_webidl_DOMString *Mode_p)
{
NH_DOM_BEGIN()

    MODE = Mode_p;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT nh_dom_setDocumentType(
    nh_webidl_Object *Document_p, nh_webidl_Object *DocumentType_p)
{
NH_DOM_BEGIN()

    DOCTYPE = DocumentType_p;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

