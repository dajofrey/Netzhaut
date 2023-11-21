// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#define _GNU_SOURCE

#include "Loader.h"
#include "Reload.h"
#include "Library.h"

#include "../System/Memory.h"
#include "../Common/Log.h"
#include "../Common/Config.h"
#include "../Common/Macros.h"

#include <link.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// DATA ============================================================================================

nh_Loader NH_LOADER;

// MODULE ==========================================================================================

/**
 * Must match NH_MODULE enum. 
 */
const NH_BYTE *NH_MODULE_NAMES_PP[] = {
    "nhcore",
    "nhwsi",
    "nhtty",
    "nhnetwork",
    "nhecmascript",
    "nhhtml",
    "nhwebidl",
    "nhencoding",
    "nhdom",
    "nhgfx",
    "nhcss",
    "nhterminal",
    "nhrenderer",
    "nhurl",
    "nhmake",
};

int nh_core_getModuleIndex(
    NH_BYTE *name_p)
{
NH_CORE_BEGIN()

    for (int i = 0; i < NH_MODULE_E_COUNT; ++i) {
        if (!strcmp(name_p, NH_MODULE_NAMES_PP[i])) {NH_CORE_END(i)}
    }

NH_CORE_END(-1)
}

static nh_Module nh_core_initModule(
    NH_MODULE_E type)
{
NH_CORE_BEGIN()

    nh_Module Module;
    memset(&Module, 0, sizeof(nh_Module));
    Module.type = type;

NH_CORE_END(Module)
}

static nh_Module *nh_core_getModule(
    NH_MODULE_E module, int major)
{
NH_CORE_BEGIN()

    if (NH_LOADER.Modules_p[module].loaded == NH_FALSE) {
        NH_CORE_END(NULL)
    }
    if (major != -1 && NH_LOADER.Modules_p[module].major != major) {
        NH_CORE_END(NULL)
    }

NH_CORE_END(&NH_LOADER.Modules_p[module])
}

// LOAD MODULE =====================================================================================

typedef int (*initialize_f)();

static NH_CORE_RESULT nh_core_callDefaultInitializer(
    nh_Module *Module_p)
{
NH_CORE_BEGIN()

    NH_BYTE functionName_p[255] = {'\0'};
    sprintf(functionName_p, "nh_%s_initialize", NH_MODULE_NAMES_PP[Module_p->type] + 2);

    initialize_f initializer_f = nh_core_loadSymbolFromLibrary(Module_p->lib_p, functionName_p);
    if (initializer_f != NULL) {initializer_f();}

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

static NH_CORE_RESULT nh_core_loadDependencies(
    NH_MODULE_E _module, int major)
{
NH_CORE_BEGIN()

    switch (_module) 
    {
        case NH_MODULE_HTML : 
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_CORE, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_DOM, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_URL, 0))
            break;

        case NH_MODULE_WEBIDL : 
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_CORE, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_ENCODING, 0))
            break;

        case NH_MODULE_ECMASCRIPT : 
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_CORE, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_ENCODING, 0))
            break;

        case NH_MODULE_CSS :
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_CORE, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_ENCODING, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_DOM, 0))
            break;

        case NH_MODULE_DOM :
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_CORE, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_WEBIDL, 0))
            break;

        case NH_MODULE_RENDERER : 
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_CORE, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_GFX, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_CSS, 0))
            break;

        case NH_MODULE_URL :
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_CORE, 0))
            NH_CORE_CHECK(NH_LOADER.load_f(NH_MODULE_ENCODING, 0))
            break;

        case NH_MODULE_MAKE :
            break;
    }

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

static NH_CORE_RESULT nh_core_load(
    NH_MODULE_E _module, int major)
{
NH_CORE_BEGIN()

    if (nh_core_getModule(_module, major)) {NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)}
 
    NH_CORE_CHECK(nh_core_loadDependencies(_module, major))

    nh_Module *Module_p = &NH_LOADER.Modules_p[_module];

    Module_p->lib_p = nh_core_loadLibrary(_module, -1);
    NH_CORE_CHECK_NULL(Module_p->lib_p)

    Module_p->lastModified_p = nh_core_lastModified(Module_p->lib_p);
    NH_CORE_CHECK_NULL(Module_p->lastModified_p)

    nh_core_callDefaultInitializer(Module_p);

    Module_p->loaded = NH_TRUE;

    NH_CORE_CHECK(nh_core_logModules())

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

// UNLOAD LIBRARY ==================================================================================

typedef int (*terminate_f)();

static NH_CORE_RESULT nh_core_callDefaultTerminator(
    nh_Module *Module_p)
{
NH_CORE_BEGIN()

    NH_BYTE functionName_p[255] = {'\0'};
    sprintf(functionName_p, "nh_%s_terminate", NH_MODULE_NAMES_PP[Module_p->type] + 2);
    terminate_f terminator_f = NH_LOADER.loadSymbol_f(Module_p->type, Module_p->major, functionName_p);
    if (terminator_f != NULL) {terminator_f();}

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

static NH_BOOL nh_core_isUnloadAllowed(
    NH_MODULE_E _module)
{
NH_CORE_BEGIN()

    switch (_module) 
    {
        case NH_MODULE_HTML : 
            // No module has this module currently as a dependency, 
            // so we can unload it.
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_ENCODING :
            if (NH_LOADER.Modules_p[NH_MODULE_WEBIDL].loaded == NH_TRUE
            ||  NH_LOADER.Modules_p[NH_MODULE_ECMASCRIPT].loaded == NH_TRUE
            ||  NH_LOADER.Modules_p[NH_MODULE_CSS].loaded == NH_TRUE
            ||  NH_LOADER.Modules_p[NH_MODULE_URL].loaded == NH_TRUE) {
                NH_CORE_END(NH_FALSE)    
            }
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_WEBIDL : 
            if (NH_LOADER.Modules_p[NH_MODULE_DOM].loaded == NH_TRUE) {
                NH_CORE_END(NH_FALSE)    
            }
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_ECMASCRIPT : 
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_CSS :
            if (NH_LOADER.Modules_p[NH_MODULE_RENDERER].loaded == NH_TRUE) {
                NH_CORE_END(NH_FALSE)    
            }
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_DOM :
            if (NH_LOADER.Modules_p[NH_MODULE_HTML].loaded == NH_TRUE
            ||  NH_LOADER.Modules_p[NH_MODULE_CSS].loaded == NH_TRUE) {
                NH_CORE_END(NH_FALSE)    
            }
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_RENDERER : 
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_URL :
            if (NH_LOADER.Modules_p[NH_MODULE_HTML].loaded == NH_TRUE) {
                NH_CORE_END(NH_FALSE)    
            }
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_MAKE :
            NH_CORE_END(NH_TRUE)

        case NH_MODULE_GFX :
            if (NH_LOADER.Modules_p[NH_MODULE_RENDERER].loaded == NH_TRUE) {
                NH_CORE_END(NH_FALSE)    
            }
            NH_CORE_END(NH_TRUE)
    }

NH_CORE_END(NH_TRUE)
}

static NH_CORE_RESULT nh_core_unloadModule(
    NH_MODULE_E module)
{
NH_CORE_BEGIN()

    nh_Module *Module_p = &NH_LOADER.Modules_p[module];

    // nhcore is unloaded in the API.
    if (Module_p->loaded == NH_FALSE || module == NH_MODULE_CORE) {NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)}

    NH_CORE_CHECK(nh_core_callDefaultTerminator(Module_p))

    if (nh_core_getConfig().loaderUnload) {
        NH_CORE_CHECK(nh_unloadLibrary(Module_p->lib_p))
    }

    if (Module_p->lastModified_p != NULL) {free(Module_p->lastModified_p);}

    *Module_p = nh_core_initModule(module);

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

// LOAD SYMBOL =====================================================================================

void *nh_core_loadExistingSymbol(
    NH_MODULE_E type, int major, const NH_BYTE *name_p)
{
NH_CORE_BEGIN()

    nh_Module *Module_p = nh_core_getModule(type, major);
    if (Module_p == NULL) {NH_CORE_END(NULL)}

NH_CORE_END(nh_core_loadSymbolFromLibrary(Module_p->lib_p, name_p))
}

static void *nh_core_loadSymbol(
    NH_MODULE_E type, int major, const NH_BYTE *name_p)
{
NH_CORE_BEGIN()

    nh_Module *Module_p = nh_core_getModule(type, major);
    if (Module_p == NULL) {
        if (nh_core_load(type, major) == NH_CORE_SUCCESS) {
            Module_p = nh_core_getModule(type, major);
        }
    }

    if (Module_p == NULL) {NH_CORE_END(NULL)}

NH_CORE_END(nh_core_loadSymbolFromLibrary(Module_p->lib_p, name_p))
}

void *nh_core_loadSymbolUsingModuleName(
    NH_BYTE *module_p, NH_BYTE *symbol_p)
{
NH_CORE_BEGIN()

    NH_MODULE_E module = nh_core_getModuleIndex(module_p);
    if (module == -1) {NH_CORE_END(NULL)}

NH_CORE_END(nh_core_loadSymbol(module, -1, symbol_p))
}

// INIT/DESTROY ====================================================================================

nh_Loader *nh_core_initLoader(
    NH_LOADER_SCOPE_E scope, NH_BOOL fallback, NH_BOOL install)
{
NH_CORE_BEGIN()

    NH_LOADER.scope = scope;
    NH_LOADER.install = install;

    NH_LOADER.load_f = nh_core_load; 
    NH_LOADER.unload_f = nh_core_unloadModule;
    NH_LOADER.loadSymbol_f = nh_core_loadSymbol;

    for (int i = 0; i < NH_MODULE_E_COUNT; ++i) {
        NH_LOADER.Modules_p[i] = nh_core_initModule(i);
    }

    if (fallback && install) {
        puts("TODO replace fallback loader");
        exit(0);
    }

NH_CORE_END(&NH_LOADER)
}

NH_CORE_RESULT nh_core_freeLoader()
{
NH_CORE_BEGIN()

    while (1) {
        NH_BOOL unloadNeeded = NH_FALSE;
        // We skip the nhcore module since it's unloaded in the API.
        for (int i = 1; i < NH_MODULE_E_COUNT; ++i) {
            if (nh_core_isUnloadAllowed(i)) {
                NH_CORE_CHECK(nh_core_unloadModule(i))
            }
            if (NH_LOADER.Modules_p[i].loaded == NH_TRUE) {unloadNeeded = NH_TRUE;}
        }
        if (!unloadNeeded) {break;}
    }

NH_CORE_END(NH_CORE_SUCCESS)
}
