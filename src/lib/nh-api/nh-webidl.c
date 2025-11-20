// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-webidl.h"
#include "nh-core.h"

#include "../nh-core/Loader/Loader.h" 

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_api_HostGlobals nh_api_getHostGlobals()
{
    typedef nh_api_HostGlobals (*nh_api_getHostGlobals_f)(); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_api_getHostGlobals_f getHostGlobals_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WEBIDL, 0, "nh_webidl_getHostGlobals");
    nh_api_HostGlobals HostGlobals;
    memset(&HostGlobals, 0, sizeof(nh_api_HostGlobals));
    return getHostGlobals_f ? getHostGlobals_f() : HostGlobals;
}

nh_api_HostGlobals nh_api_getHostGlobalsForHTML()
{
    typedef nh_api_HostGlobals (*nh_html_getHostGlobalsForHTML_f)();
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_html_getHostGlobalsForHTML_f getHostGlobalsForHTML_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WEBIDL, 0, "nh_webidl_getHostGlobalsForHTML");
    nh_api_HostGlobals HostGlobals;
    memset(&HostGlobals, 0, sizeof(nh_api_HostGlobals));
    return getHostGlobalsForHTML_f ? getHostGlobalsForHTML_f() : HostGlobals;
}
