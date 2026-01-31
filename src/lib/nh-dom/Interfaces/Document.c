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
    nh_encoding_UTF8String *Mode_p;
} nh_dom_Document;

// DATA ============================================================================================

nh_encoding_UTF8String NH_DOM_DOCUMENT_MODE_NO_QUIRKS;
nh_encoding_UTF8String NH_DOM_DOCUMENT_MODE_QUIRKS;
nh_encoding_UTF8String NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_dom_initializeDocument(
    nh_ecmascript_Object *Document_p)
{
    Document_p->internal_p = nh_core_allocate(sizeof(nh_dom_Document));
    NH_CORE_CHECK_MEM(Document_p->internal_p)

    MODE = &NH_DOM_DOCUMENT_MODE_NO_QUIRKS;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_initDocumentModes()
{
    NH_DOM_DOCUMENT_MODE_NO_QUIRKS = nh_encoding_initUTF8(16);
    NH_DOM_DOCUMENT_MODE_QUIRKS = nh_encoding_initUTF8(16);
    NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS = nh_encoding_initUTF8(16);

    nh_core_appendToString(&NH_DOM_DOCUMENT_MODE_NO_QUIRKS, "no-quirks", 9);
    nh_core_appendToString(&NH_DOM_DOCUMENT_MODE_QUIRKS, "quirks", 6);
    nh_core_appendToString(&NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS, "limited-quirks", 14);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_freeDocumentModes()
{
    nh_encoding_freeUTF8(&NH_DOM_DOCUMENT_MODE_NO_QUIRKS);
    nh_encoding_freeUTF8(&NH_DOM_DOCUMENT_MODE_QUIRKS);
    nh_encoding_freeUTF8(&NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_setDocumentMode(
    nh_ecmascript_Object *Document_p, nh_encoding_UTF8String *Mode_p)
{
    MODE = Mode_p;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_setDocumentType(
    nh_ecmascript_Object *Document_p, nh_ecmascript_Object *DocumentType_p)
{
    DOCTYPE = DocumentType_p;

    return NH_API_SUCCESS;
}

