// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "BrowsingContext.h"

#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

static void nh_html_initBrowsingContext(
    nh_html_BrowsingContext *Context_p)
{
    nh_html_BrowsingContext BrowsingContext;
    memset(&BrowsingContext, 0, sizeof(nh_html_BrowsingContext));
    BrowsingContext.SessionHistory = nh_core_initList(16);
    return BrowsingContext;
}

NH_API_RESULT nh_html_createNewBrowsingContextAndDocument(
    nh_ecmascript_Object *Creator_p, nh_ecmascript_Object *Embedder_p, nh_html_BrowsingContextGroup *Group_p, 
    nh_html_BrowsingContext **Context_pp, nh_ecmascript_Object **Document_pp)
{
    // let browsing context be a new browsing context
    nh_html_BrowsingContext *BrowsingContext_p = nh_core_allocate(sizeof(nh_html_BrowsingContext));
    NH_CORE_CHECK_MEM(NULL, BrowsingContext_p)
    *BrowsingContext_p = nh_html_initBrowsingContext();

    // let unsafeContextCreationTime be the unsafe shared current time
    // TODO

    nh_html_Origin *CreatorOrigin_p = NULL;
    nh_url_URL CreatorBaseURL_p = NULL;
    
    if (Creator_p) {
        puts("sddsadfad");
        exit(0);
    }

    int sandboxFlags = nh_html_determineTheCreationSandboxingFlags(BrowsingContext_p, Embedder_p);

    nh_url_URL AboutBlank = nh_url_newURL("about:blank");
    Origin_p = nh_html_determineTheOrigin(AboutBlank, sandboxFlags, CreatorOrigin_p);
    nh_html_PermissionsPolicy PermissionsPolicy = nh_html_createPermissionsPolicy(Embedder_p, Origin_p);

    nh_html_Agent *Agent_p = nh_html_obtainSimilarOriginWindowAgent(Origin_p, Group_p, false);
    nh_ecmascript_Realm *RealmExecutionContext_p = nh_html_createRealm(Agent_p);
 
//    Window_p = nh_html_createWindow(ExecutionContext_p->Realm_p);
//    WindowProxy_p = nh_html_createWindowProxy(Window_p);

    nh_ecmascript_ExecutionContext *ExecutionContext_p = nh_html_createJavaScriptRealm(Agent_p, Window_p, WindowProxy_p);

    return NewContext_p;
}

static nh_html_BrowsingContextGroup nh_html_initBrowsingContextGroup()
{
    nh_html_BrowsingContextGroup Group;
    Group.BrowsingContexts = nh_core_initList(128);
    Group.AgentClusterMap = nh_core_initList(128);
    Group.HistoricalAgentClusterKeyMap = nh_core_initList(128);
    Group.crossOriginIsolationMode = NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_NONE;
    return Group;
}

// https://html.spec.whatwg.org/#creating-a-new-browsing-context-group-and-document
NH_API_RESULT nh_html_createNewBrowsingContextGroupAndDocument(
    nh_html_UserAgent *UserAgent_p, nh_html_BrowsingContextGroup **BrowsingContextGroup_pp,
    nh_ecmascript_Object **Document_pp)
{
    nh_html_BrowsingContextGroup *Group_p = nh_core_allocate(sizeof(nh_html_BrowsingContextGroup));
    NH_CORE_CHECK_MEM_2(NULL, Group_p)

    *Group_p = nh_html_initBrowsingContextGroup();
    nh_core_appendToList(&UserAgent_p->BrowsingContextGroups, Group_p);

    nh_html_BrowsingContext *BrowsingContext_p = NULL;
    NH_CORE_CHECK(nh_html_createNewBrowsingContextAndDocument(NULL, NULL, Group_p, &BrowsingContext_p, Document_pp))

    nh_core_appendToList(&Group_p->BrowsingContexts, BrowsingContext_p);

    *BrowsingContextGroup_pp = Group_p;

    return NH_API_SUCCESS;
}

// https://html.spec.whatwg.org/#creating-a-new-top-level-browsing-context
NH_API_RESULT nh_html_createNewTopLevelBrowsingContextAndDocument(
    nh_html_UserAgent *UserAgent_p, nh_core_List **BrowsingContexts_pp, nh_ecmascript_Object **Document_pp)
{
    nh_html_BrowsingContextGroup *BrowsingContextGroup_p = NULL;
    NH_CORE_CHECK(nh_html_createNewBrowsingContextGroupAndDocument(UserAgent_p, &BrowsingContextGroup_p, Document_pp))
    *BrowsingContexts_pp = BrowsingContextGroup_p->BrowsingContexts.pp[0];
    return NH_API_SUCCESS;
}
