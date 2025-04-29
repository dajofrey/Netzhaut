// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nh-ecmascript.h"
#include "nh-core.h"

#include "../nh-core/Loader/Loader.h" 
#include "../nh-ecmascript/Engine/Script.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS =======================================================================================

nh_api_Script *nh_api_parseScript(
    char *sourceText_p, nh_api_Realm *Realm_p, int encoding)
{
    typedef nh_api_Script *(*nh_ecmascript_parseScript_f)(char *sourceText_p, nh_api_Realm *Realm_p, int encoding); 
    nh_core_Loader *Loader_p = nh_api_getLoader();
    nh_ecmascript_parseScript_f parseScript_f = !Loader_p ? NULL : Loader_p->loadSymbol_f(NH_MODULE_ECMASCRIPT, 0, "nh_ecmascript_parseScript");
    return parseScript_f ? parseScript_f(sourceText_p, Realm_p, encoding) : NULL;
}
