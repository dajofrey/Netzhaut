// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhecmascript.h"

#include "../nhcore/Loader/Loader.h" 
#include "../nhecmascript/Engine/Script.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// PARSE ===========================================================================================

nh_ecmascript_Script *nh_api_parseScript(
    NH_BYTE *sourceText_p, nh_ecmascript_Realm *Realm_p, int encoding)
{
    nh_ecmascript_parseScript_f parseScript_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_ECMASCRIPT, 0, "nh_ecmascript_parseScript");
    return parseScript_f ? parseScript_f(sourceText_p, Realm_p, encoding) : NULL;
}

