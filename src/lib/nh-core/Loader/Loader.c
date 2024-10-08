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
const char *NH_MODULE_NAMES_PP[] = {
    "nh-core",
    "nh-wsi",
    "nh-network",
    "nh-ecmascript",
    "nh-html",
    "nh-webidl",
    "nh-encoding",
    "nh-dom",
    "nh-gfx",
    "nh-css",
    "nh-renderer",
    "nh-url",
};

int nh_core_getModuleIndex(
    char *name_p)
{
NH_CORE_BEGIN()

    for (int i = 0; i < NH_MODULE_E_COUNT; ++i) {
        if (!strcmp(name_p, NH_MODULE_NAMES_PP[i])) {NH_CORE_END(i)}
    }

NH_CORE_END(-1)
}

static nh_Module nh_core_initModule(
    int type)
{
NH_CORE_BEGIN()

    nh_Module Module;
    memset(&Module, 0, sizeof(nh_Module));
    Module.type = type;

NH_CORE_END(Module)
}

static nh_Module *nh_core_getModule(
    NH_MODULE_E module)
{
NH_CORE_BEGIN()

    if (NH_LOADER.Modules_p[module].loaded == false) {
        NH_CORE_END(NULL)
    }

NH_CORE_END(&NH_LOADER.Modules_p[module])
}

// LOAD MODULE =====================================================================================

typedef int (*initialize_f)();

static NH_API_RESULT nh_core_callDefaultInitializer(
    nh_Module *Module_p)
{
NH_CORE_BEGIN()

    char functionName_p[255] = {'\0'};
    sprintf(functionName_p, "nh_%s_initialize", NH_MODULE_NAMES_PP[Module_p->type] + 3);

    initialize_f initializer_f = nh_core_loadSymbolFromLibrary(Module_p->lib_p, functionName_p);
    if (initializer_f != NULL) {initializer_f();}

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_core_loadDependencies(
    NH_MODULE_E _module, char *path_p)
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
    }

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_core_load(
    NH_MODULE_E type, char *path_p)
{
NH_CORE_BEGIN()

    if (nh_core_getModule(type)) {NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)}
    NH_CORE_CHECK(nh_core_loadDependencies(type, path_p))

    nh_Module *Module_p = &NH_LOADER.Modules_p[type];

    Module_p->lib_p = nh_core_loadLibrary(type, path_p);
    NH_CORE_CHECK_NULL(Module_p->lib_p)

    Module_p->lastModified_p = nh_core_lastModified(Module_p->lib_p);
    NH_CORE_CHECK_NULL(Module_p->lastModified_p)

    // This needs to be set before calling initalizer, 
    // because e.g. nh-css uses the loader in the initializer.
    Module_p->loaded = true;

    nh_core_callDefaultInitializer(Module_p);
//    NH_CORE_CHECK(nh_core_logModules())

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// EXTERNAL MODULES ================================================================================

static nh_core_ExternalModule *nh_core_getExternalModule(
    char *name_p)
{
NH_CORE_BEGIN()

    for (int i = 0; i < NH_LOADER.ExternalModules.length; ++i) {
        nh_core_ExternalModule *Module_p = ((nh_core_ExternalModule*)NH_LOADER.ExternalModules.p)+i;
        if (Module_p->name_p && !strcmp(Module_p->name_p, name_p)) {
            NH_CORE_END(Module_p)
        }
    }

NH_CORE_END(NULL)
}

static NH_API_RESULT nh_core_callExternalInitializer(
    nh_core_ExternalModule *Module_p)
{
NH_CORE_BEGIN()

    char name_p[255] = {'\0'};
    sprintf(name_p, "%s_initialize", Module_p->name_p);

    for (int i = 0; i < strlen(name_p); ++i) {
        if (name_p[i] == '-') {name_p[i] = '_';}
    }

    initialize_f initializer_f = nh_core_loadSymbolFromLibrary(Module_p->Data.lib_p, name_p);
    if (initializer_f != NULL) {initializer_f();}

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_core_loadExternal(
    nh_core_ExternalModule *Module_p
);

static NH_API_RESULT nh_core_loadExternalDependencies(
    nh_core_ExternalModule *Module_p)
{
NH_CORE_BEGIN()

    for (int i = 0; i < Module_p->dependencies; ++i) {
        char *name_p = Module_p->dependencies_pp[i];
        int index = nh_core_getModuleIndex(name_p);
        if (index >= 0) {
            NH_LOADER.load_f(index, 0);
        } else {
            nh_core_ExternalModule *Dependency_p = nh_core_getExternalModule(name_p);
            if (!Dependency_p) {NH_CORE_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}
            if (!Dependency_p->Data.loaded) {
                nh_core_loadExternal(Dependency_p);
            }
        }
    }

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static NH_API_RESULT nh_core_loadExternal(
    nh_core_ExternalModule *Module_p)
{
NH_CORE_BEGIN()

    NH_CORE_CHECK(nh_core_loadExternalDependencies(Module_p))

    Module_p->Data.lib_p = nh_core_loadExternalLibrary(Module_p->name_p, Module_p->path_p);
    NH_CORE_CHECK_NULL(Module_p->Data.lib_p)

    Module_p->Data.lastModified_p = nh_core_lastModified(Module_p->Data.lib_p);
    NH_CORE_CHECK_NULL(Module_p->Data.lastModified_p)

    nh_core_callExternalInitializer(Module_p);

    Module_p->Data.loaded = true;

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static void *nh_core_loadExternalSymbol(
    char *module_p, const char *name_p)
{
NH_CORE_BEGIN()

    nh_core_ExternalModule *Module_p = nh_core_getExternalModule(module_p);
    if (Module_p == NULL) {
        NH_CORE_END(NULL)
    }
    if (!Module_p->Data.loaded) {
        nh_core_loadExternal(Module_p);
    }

NH_CORE_END(nh_core_loadSymbolFromLibrary(Module_p->Data.lib_p, name_p))
}

NH_API_RESULT nh_core_addExternalModule(
    const char *name_p, const char *path_p, const char **dependencies_pp, size_t dependencies)
{
NH_CORE_BEGIN()
   
    nh_core_ExternalModule *Module_p = nh_core_incrementArray(&NH_LOADER.ExternalModules);
    NH_CORE_CHECK_NULL(Module_p)

    memset(Module_p, 0, sizeof(nh_core_ExternalModule));

    Module_p->Data = nh_core_initModule(-1);
    Module_p->name_p = name_p;
    Module_p->dependencies_pp = dependencies_pp;
    Module_p->dependencies = dependencies;
    
    if (path_p != NULL) {
        strcpy(Module_p->path_p, path_p);
    } 

NH_CORE_END(NH_API_SUCCESS)
}

// UNLOAD LIBRARY ==================================================================================

typedef int (*terminate_f)();

static NH_API_RESULT nh_core_callDefaultTerminator(
    nh_Module *Module_p)
{
NH_CORE_BEGIN()

    char functionName_p[255] = {'\0'};
    sprintf(functionName_p, "nh_%s_terminate", NH_MODULE_NAMES_PP[Module_p->type] + 3);
    terminate_f terminator_f = NH_LOADER.loadSymbol_f(Module_p->type, 0, functionName_p);
    if (terminator_f != NULL) {terminator_f();}

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

static bool nh_core_isUnloadAllowed(
    NH_MODULE_E _module)
{
NH_CORE_BEGIN()

    switch (_module) 
    {
        case NH_MODULE_HTML : 
            // No module has this module currently as a dependency, 
            // so we can unload it.
            NH_CORE_END(true)

        case NH_MODULE_ENCODING :
            if (NH_LOADER.Modules_p[NH_MODULE_WEBIDL].loaded == true
            ||  NH_LOADER.Modules_p[NH_MODULE_ECMASCRIPT].loaded == true
            ||  NH_LOADER.Modules_p[NH_MODULE_CSS].loaded == true
            ||  NH_LOADER.Modules_p[NH_MODULE_URL].loaded == true) {
                NH_CORE_END(false)    
            }
            NH_CORE_END(true)

        case NH_MODULE_WEBIDL : 
            if (NH_LOADER.Modules_p[NH_MODULE_DOM].loaded == true) {
                NH_CORE_END(false)    
            }
            NH_CORE_END(true)

        case NH_MODULE_ECMASCRIPT : 
            NH_CORE_END(true)

        case NH_MODULE_CSS :
            if (NH_LOADER.Modules_p[NH_MODULE_RENDERER].loaded == true) {
                NH_CORE_END(false)    
            }
            NH_CORE_END(true)

        case NH_MODULE_DOM :
            if (NH_LOADER.Modules_p[NH_MODULE_HTML].loaded == true
            ||  NH_LOADER.Modules_p[NH_MODULE_CSS].loaded == true) {
                NH_CORE_END(false)    
            }
            NH_CORE_END(true)

        case NH_MODULE_RENDERER : 
            NH_CORE_END(true)

        case NH_MODULE_URL :
            if (NH_LOADER.Modules_p[NH_MODULE_HTML].loaded == true) {
                NH_CORE_END(false)    
            }
            NH_CORE_END(true)

        case NH_MODULE_GFX :
            if (NH_LOADER.Modules_p[NH_MODULE_RENDERER].loaded == true) {
                NH_CORE_END(false)    
            }
            NH_CORE_END(true)
    }

NH_CORE_END(true)
}

static NH_API_RESULT nh_core_unloadModule(
    NH_MODULE_E module)
{
NH_CORE_BEGIN()

    nh_Module *Module_p = &NH_LOADER.Modules_p[module];

    // nh-core is unloaded in the API.
    if (Module_p->loaded == false || module == NH_MODULE_CORE) {NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)}

    NH_CORE_CHECK(nh_core_callDefaultTerminator(Module_p))

    if (nh_core_getConfig().loaderUnload) {
        NH_CORE_CHECK(nh_unloadLibrary(Module_p->lib_p))
    }

    if (Module_p->lastModified_p != NULL) {free(Module_p->lastModified_p);}

    *Module_p = nh_core_initModule(module);

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// LOAD SYMBOL =====================================================================================

void *nh_core_loadExistingSymbol(
    NH_MODULE_E type, int major, const char *name_p)
{
NH_CORE_BEGIN()

    nh_Module *Module_p = nh_core_getModule(type);
    if (Module_p == NULL) {NH_CORE_END(NULL)}

NH_CORE_END(nh_core_loadSymbolFromLibrary(Module_p->lib_p, name_p))
}

static void *nh_core_loadSymbol(
    NH_MODULE_E type, int major, const char *name_p)
{
NH_CORE_BEGIN()

    nh_Module *Module_p = nh_core_getModule(type);
    if (Module_p == NULL) {
        if (nh_core_load(type, 0) == NH_API_SUCCESS) {
            Module_p = nh_core_getModule(type);
        }
    }

    if (Module_p == NULL) {NH_CORE_END(NULL)}

NH_CORE_END(nh_core_loadSymbolFromLibrary(Module_p->lib_p, name_p))
}

void *nh_core_loadSymbolUsingModuleName(
    char *module_p, char *symbol_p)
{
NH_CORE_BEGIN()

    NH_MODULE_E index = nh_core_getModuleIndex(module_p);
    if (index == -1) {NH_CORE_END(NULL)}

NH_CORE_END(nh_core_loadSymbol(index, 0, symbol_p))
}

// INIT/DESTROY ====================================================================================

nh_Loader *nh_core_initLoader(
    bool fallback, bool install)
{
NH_CORE_BEGIN()

    NH_LOADER.install = install;
    NH_LOADER.load_f = nh_core_load; 
    NH_LOADER.unload_f = nh_core_unloadModule;
    NH_LOADER.loadSymbol_f = nh_core_loadSymbol;
    NH_LOADER.loadExternalSymbol_f = nh_core_loadExternalSymbol;
    NH_LOADER.addModule_f = nh_core_addExternalModule;

    for (int i = 0; i < NH_MODULE_E_COUNT; ++i) {
        NH_LOADER.Modules_p[i] = nh_core_initModule(i);
    }

    NH_LOADER.Modules_p[0].loaded = true;
    NH_LOADER.ExternalModules = nh_core_initArray(sizeof(nh_core_ExternalModule), 8);

    if (fallback && install) {
        puts("TODO replace fallback loader");
        exit(0);
    }

NH_CORE_END(&NH_LOADER)
}

NH_API_RESULT nh_core_freeLoader()
{
NH_CORE_BEGIN()

    while (1) {
        bool unloadNeeded = false;
        // We skip the nh-core module since it's unloaded in the API.
        for (int i = 1; i < NH_MODULE_E_COUNT; ++i) {
            if (nh_core_isUnloadAllowed(i)) {
                NH_CORE_CHECK(nh_core_unloadModule(i))
            }
            if (NH_LOADER.Modules_p[i].loaded == true) {unloadNeeded = true;}
        }
        if (!unloadNeeded) {break;}
    }

NH_CORE_END(NH_API_SUCCESS)
}
