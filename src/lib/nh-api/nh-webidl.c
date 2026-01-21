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

NH_API_RESULT nh_api_installInterfaces(
    nh_api_Realm *Realm_p)
{
    typedef NH_API_RESULT (*nh_api_installInterfaces_f)(nh_api_Realm *Realm_p); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_api_installInterfaces_f installInterfaces_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_WEBIDL, 0, "nh_api_installInterfaces");
    return installInterfaces_f ? installInterfaces_f(Realm_p) : NH_API_ERROR_BAD_STATE;
}
