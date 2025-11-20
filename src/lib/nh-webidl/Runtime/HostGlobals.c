// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HostGlobals.h"

#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

static nh_api_HostGlobal HostGlobals_p[1];

static char *specifications_pp[1] = {
    "System",
};
static char *interfaces_pp[1] = {
    "Console",
};

void nh_webidl_initHostGlobals()
{
    HostGlobals_p[0].specification_p = specifications_pp[0];
    HostGlobals_p[0].interface_p = interfaces_pp[0];
}

nh_api_HostGlobals nh_webidl_getHostGlobals()
{
    nh_api_HostGlobals HostGlobals;
    HostGlobals.size = 1;
    HostGlobals.p = HostGlobals_p;

    return HostGlobals;
}

nh_api_HostGlobals nh_webidl_getHostGlobalsForHTML()
{
    nh_api_HostGlobals HostGlobals;
    memset(&HostGlobals, 0, sizeof(nh_api_HostGlobals));

    return HostGlobals;
}
