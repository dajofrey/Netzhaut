#ifndef NH_CORE_LOADER_LOADER_H
#define NH_CORE_LOADER_LOADER_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"
#include "../Util/Array.h"

#include <stddef.h>

// ENUMS ===========================================================================================

/**
 * !!! DONT FORGET TO EDIT COMMON/LOG.H WHEN ADDING A MODULE !!!
 */
typedef enum NH_MODULE_E {
    NH_MODULE_CORE = 0,
    NH_MODULE_WSI,
    NH_MODULE_NETWORK,
    NH_MODULE_ECMASCRIPT,
    NH_MODULE_HTML,
    NH_MODULE_WEBIDL,
    NH_MODULE_ENCODING,
    NH_MODULE_DOM,
    NH_MODULE_GFX,
    NH_MODULE_CSS,
    NH_MODULE_RENDERER,
    NH_MODULE_URL,
    NH_MODULE_MONITOR,
    NH_MODULE_E_COUNT,
} NH_MODULE_E;

// TYPES ===========================================================================================

typedef struct nh_core_Loader nh_core_Loader;

typedef nh_core_Loader *(*nh_core_initLoader_f)(
    bool fallback, bool install
);

typedef NH_API_RESULT (*nh_core_load_f)(
    NH_MODULE_E _module, char *path_p
);

typedef NH_API_RESULT (*nh_unload_f)(
    NH_MODULE_E _module
);

typedef void *(*nh_core_loadSymbol_f)(
    NH_MODULE_E _module, int major, const char *functionName_p
);

typedef void *(*nh_core_loadExternalSymbol_f)(
    char *name_p, const char *functionName_p
);

typedef NH_API_RESULT (*nh_core_addModule_f)(
    const char *name_p, const char *path_p, const char **dependencies_pp, size_t dependencies
);

// STRUCTS =========================================================================================

typedef struct nh_Module {
    NH_MODULE_E type;
    int major;
    void *lib_p;
    char *lastModified_p;
    bool loaded;
} nh_Module;

typedef struct nh_core_ExternalModule {
    nh_Module Data;
    char *name_p;
    char **dependencies_pp;
    size_t dependencies;
    char path_p[255];
} nh_core_ExternalModule;

typedef struct nh_core_Loader {
    bool install;
    bool unload;
    nh_core_load_f load_f;
    nh_unload_f unload_f;
    nh_core_loadSymbol_f loadSymbol_f;
    nh_core_loadExternalSymbol_f loadExternalSymbol_f;
    nh_core_addModule_f addModule_f;
    nh_Module Modules_p[NH_MODULE_E_COUNT];
    nh_core_Array ExternalModules;
} nh_core_Loader;

// EXTERN ==========================================================================================

extern nh_core_Loader NH_LOADER;
extern const char *NH_MODULE_NAMES_PP[];

// FUNCTIONS =======================================================================================

void *nh_core_loadExistingSymbol(
    NH_MODULE_E _module, int major, const char *functionName_p
);

void *nh_core_loadSymbol(
    NH_MODULE_E type, int major, const char *name_p
);

void *nh_core_loadSymbolUsingModuleName(
    char *moduleName_p, char *symbolName_p
);

nh_core_Loader *nh_core_initLoader(
    bool fallback, bool install
);

NH_API_RESULT nh_core_freeLoader(
);

int nh_core_getModuleIndex(
    char *name_p
);

#endif // NH_CORE_LOADER_LOADER_H

