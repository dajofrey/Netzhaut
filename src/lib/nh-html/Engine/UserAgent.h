#ifndef NH_HTML_USER_AGENT_H
#define NH_HTML_USER_AGENT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

typedef struct nh_html_UserAgent {
    nh_core_List TopLevelTraversables;
    // https://html.spec.whatwg.org/#browsing-context-group-set
    nh_core_List BrowsingContextGroups;
} nh_html_UserAgent;

#endif
