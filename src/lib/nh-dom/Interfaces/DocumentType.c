// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "DocumentType.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-webidl/Runtime/Object.h"
#include "../../nh-webidl/Runtime/String.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DECLARE =========================================================================================

#define NAME DocumentType_p->Attributes.pp[0]
#define PUBLIC_ID DocumentType_p->Attributes.pp[1] 
#define SYSTEM_ID DocumentType_p->Attributes.pp[2]

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_dom_initializeDocumentType(
    nh_webidl_Object *DocumentType_p)
{
    NAME = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_CORE_CHECK_MEM(NAME)
    *((nh_webidl_DOMString*)NAME) = nh_webidl_initDOMString(32);

    PUBLIC_ID = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_CORE_CHECK_MEM(PUBLIC_ID)
    *((nh_webidl_DOMString*)PUBLIC_ID) = nh_webidl_initDOMString(32);

    SYSTEM_ID = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_CORE_CHECK_MEM(SYSTEM_ID)
    *((nh_webidl_DOMString*)SYSTEM_ID) = nh_webidl_initDOMString(32);

    return NH_API_SUCCESS;
}

nh_webidl_Object *nh_dom_createDocumentType(
    nh_webidl_DOMString *Name_p, nh_webidl_DOMString *PublicId_p, nh_webidl_DOMString *SystemId_p)
{
    nh_webidl_Object *DocumentType_p = nh_webidl_createObject("DOM", "DocumentType");
    NH_CORE_CHECK_MEM_2(NULL, DocumentType_p)

    if (Name_p != NULL) {
        nh_webidl_appendToDOMString(NAME, Name_p->p, Name_p->length);
    }
    if (PublicId_p != NULL) {
        nh_webidl_appendToDOMString(PUBLIC_ID, PublicId_p->p, PublicId_p->length);
    }
    if (SystemId_p != NULL) {
        nh_webidl_appendToDOMString(SYSTEM_ID, SystemId_p->p, SystemId_p->length);
    }

    return DocumentType_p;
}
