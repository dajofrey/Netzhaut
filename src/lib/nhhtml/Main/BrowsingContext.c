// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "BrowsingContext.h"

#include "../Common/Macros.h"

#include "../../nhdom/Interfaces/Node.h"
#include "../../nhwebidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DATA ============================================================================================

nh_List NH_HTML_BROWSING_CONTEXT_GROUPS;

// BROWSING CONTEXT ================================================================================

static void nh_html_initBrowsingContext(
    nh_html_BrowsingContext *Context_p)
{
NH_HTML_BEGIN()

    nh_html_BrowsingContext BrowsingContext;
    BrowsingContext.WindowProxy_p = NULL;
    BrowsingContext.OpenerBrowsingContext_p = NULL;
    BrowsingContext.disowned = NH_FALSE;
    BrowsingContext.isClosing = NH_FALSE;
    BrowsingContext.SessionHistory = nh_core_initList(16);
    BrowsingContext.virtualBrowsingContextGroupID = 0;
    BrowsingContext.InitialURL_p = NULL;
    BrowsingContext.OpenerOriginAtCreation_p = NULL;
    BrowsingContext.CreatorOrigin_p = NULL;
    BrowsingContext.CreatorURL_p = NULL;
    BrowsingContext.CreatorBaseURL_p = NULL;

NH_HTML_END(BrowsingContext)
}

nh_html_BrowsingContext *nh_html_createBrowsingContext(
    nh_webidl_Object *Creator_p, nh_webidl_Object *Embedder_p, nh_html_BrowsingContextGroup *Group_p)
{
NH_HTML_BEGIN()

    nh_html_BrowsingContext *NewContext_p = nh_core_allocate(sizeof(nh_html_BrowsingContext));
    NH_HTML_CHECK_MEM(NULL, NewContext_p)

    *NewContext_p = nh_html_initBrowsingContext();

    if (Creator_p) {
        NewContext_p->CreatorOrigin_p  = nh_html_getDocumentOrigin(nh_html_getDocument(Creator_p));
        NewContext_p->CreatorURL_p     = nh_html_getDocumentURL(nh_html_getDocument(Creator_p));
        NewContext_p->CreatorBaseURL_p = nh_html_getDocumentBaseURL(nh_html_getDocument(Creator_p));
    }

    nh_ecmascript_Agent *Agent_p = nh_html_obtainSimilarOriginWindowAgent();

//    Window_p = nh_html_createWindow(ExecutionContext_p->Realm_p);
//    WindowProxy_p = nh_html_createWindowProxy(Window_p);

    nh_ecmascript_ExecutionContext *ExecutionContext_p = nh_html_createJavaScriptRealm(Agent_p, Window_p, WindowProxy_p);

NH_HTML_END(NewContext_p)
}

// BROWSING CONTEXT GROUP ==========================================================================

static nh_html_BrowsingContextGroup nh_html_initBrowsingContextGroup()
{
NH_HTML_BEGIN()

    nh_html_BrowsingContextGroup Group;
    Group.BrowsingContexts = nh_core_initList(128);
    Group.AgentClusterMap = nh_core_initList(128);
    Group.HistoricalAgentClusterKeyMap = nh_core_initList(128);
    Group.crossOriginIsolationMode = NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_NONE;

NH_HTML_END(Group)
}

// https://html.spec.whatwg.org/multipage/browsers.html#creating-a-new-browsing-context-group
nh_html_BrowsingContextGroup *nh_html_createBrowsingContextGroup()
{
NH_HTML_BEGIN()

    nh_html_BrowsingContextGroup *NewGroup_p = nh_core_allocate(sizeof(nh_html_BrowsingContextGroup));
    NH_HTML_CHECK_MEM_2(NULL, NewGroup_p)

    *NewGroup_p = nh_html_initBrowsingContextGroup();

    nh_core_appendToList(&NH_HTML_BROWSING_CONTEXT_GROUPS, NewGroup_p);

    nh_html_BrowsingContext *NewContext_p = nh_html_createBrowsingContext(NULL, NULL, NewGroup_p);
    NH_HTML_CHECK_MEM_2(NULL, NewContext_p)

    nh_core_appendToList(&NewGroup_p->BrowsingContexts, NewContext_p);

NH_HTML_END(NewGroup_p)
}

