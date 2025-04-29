// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "DocumentContext.h"

#include "../Parser/Parser.h"
#include "../Interfaces/Document.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-css/Main/LayoutEngine.h"
#include "../../nh-webidl/Runtime/Object.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DATA ============================================================================================

nh_core_List NH_HTML_DOCUMENT_CONTEXTS;

// CREATE ==========================================================================================

static nh_html_DocumentContext nh_html_initDocumentContext()
{
    nh_html_DocumentContext DocumentContext;
    memset(&DocumentContext, 0, sizeof(nh_html_DocumentContext));
    DocumentContext.Documents = nh_core_initList(64);
    return DocumentContext;
}

nh_html_DocumentContext *nh_html_createDocumentContext(
    bool browsingContext)
{
    nh_html_DocumentContext *DocumentContext_p = nh_core_allocate(sizeof(nh_html_DocumentContext));
    NH_CORE_CHECK_MEM_2(NULL, DocumentContext_p)

    *DocumentContext_p = nh_html_initDocumentContext();

//    if (browsingContext) {
//        DocumentContext_p->BrowsingContextGroup_p = nh_html_createBrowsingContextGroup();
//        NH_CORE_CHECK_MEM(NULL, DocumentContext_p->BrowsingContextGroup_p)
//    }

    nh_core_appendToList(&NH_HTML_DOCUMENT_CONTEXTS, DocumentContext_p);

    return DocumentContext_p;
}

// https://html.spec.whatwg.org/#creating-a-new-top-level-traversable
nh_html_Traversable *nh_html_createNewTopLevelTraversable(
    nh_html_UserAgent *UserAgent_p, nh_html_BrowsingContext *Opener_p, nh_encoding_UTF8 TargetName)
{
    nh_webidl_Object *Document_p = NULL;
    if (Opener_p == NULL) {
        nh_html_createNewTopLevelBrowsingContextAndDocument(&Document_p);
    } else {
        puts("TODO");
        exit(0);
    }

    nh_html_DocumentState DocumentState;
    DocumentState.Document_p = Document_p;
    ...

    nh_html_Traversable *Traversable_p = nh_core_allocate(sizeof(nh_core_Traversable));
    NH_CORE_CHECK_MEM_2(Traversable_p, NULL)

    NH_CORE_CHECK_2(NULL, nh_core_appendToList(UserAgent_p->TopLevelTraversables, Traversable_p))

    return Traversable_p;
}

nh_html_Traversable *nh_html_createFreshTopLevelTraversable(
    nh_html_UserAgent *UserAgent_p, nh_url_URL InitialNavigationURL)
{
    nh_encoding_UTF8 UTF8 = nh_encoding_initUTF8();
    nh_html_Traversable *Traversable_p = 
        nh_html_createNewTopLevelTraversable(UserAgent_p, NULL, UTF8);
    NH_CORE_CHECK_2(Traversable_p, NULL)
    nh_html_navigate(Traversable_p);
    return Traversable_p;
}
