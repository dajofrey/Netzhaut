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

// CREATE ==========================================================================================

nh_html_UserAgent *nh_html_createUserAgent()
{
    nh_html_UserAgent *UserAgent_p = nh_core_allocate(sizeof(nh_html_UserAgent));
    NH_CORE_CHECK_MEM_2(NULL, UserAgent_p)

    nh_html_UserAgent UserAgent;
    memset(&UserAgent, 0, sizeof(nh_html_UserAgent));
    UserAgent.TopLevelTraversables = nh_core_initList(64);
    UserAgent.BrowsingContextGroups = nh_core_initList(64);

    return UserAgent_p;
}

// LOAD ============================================================================================

nh_html_Traversable *nh_html_loadURL(
    nh_html_UserAgent *UserAgent_p, char *url_p, unsigned long long size)
{
    return nh_html_createFreshTopLevelTraversable(UserAgent_p, URL);
}
