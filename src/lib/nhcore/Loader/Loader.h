#ifndef NH_CORE_LOADER_H
#define NH_CORE_LOADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include <stddef.h>

#endif

/** @addtogroup lib_nhcore_enums
 *  @{
 */

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
        NH_MODULE_E_COUNT,
    } NH_MODULE_E;

/** @} */

/** @addtogroup lib_nhcore_typedefs 
 *  @{
 */

    typedef nh_Loader *(*nh_core_initLoader_f)(
        NH_LOADER_SCOPE_E scope, NH_BOOL fallback, NH_BOOL install
    );

    typedef NH_CORE_RESULT (*nh_core_load_f)(
        NH_MODULE_E _module, int major
    );
    
    typedef NH_CORE_RESULT (*nh_unload_f)(
        NH_MODULE_E _module
    );

    typedef void *(*nh_core_loadSymbol_f)(
        NH_MODULE_E _module, int major, const NH_BYTE *functionName_p
    );

    typedef void *(*nh_core_loadExternalSymbol_f)(
        NH_BYTE *name_p, const NH_BYTE *functionName_p
    );

    typedef NH_CORE_RESULT (*nh_core_addModule_f)(
        const NH_BYTE *name_p, const NH_BYTE **dependencies_pp, size_t dependencies
    );

/** @} */

/** @addtogroup lib_nhcore_structs
 *  @{
 */

    typedef struct nh_Module {
        NH_MODULE_E type;
        int major;
        void *lib_p;
        NH_BYTE *lastModified_p;
        NH_BOOL loaded;
    } nh_Module;

    typedef struct nh_core_ExternalModule {
        nh_Module Data;
        NH_BYTE *name_p;
        NH_BYTE **dependencies_pp;
        size_t dependencies;
    } nh_core_ExternalModule;

    typedef struct nh_Loader {
        NH_LOADER_SCOPE_E scope;
        NH_BOOL install;
        NH_BOOL unload;
        nh_core_load_f load_f;
        nh_unload_f unload_f;
        nh_core_loadSymbol_f loadSymbol_f;
        nh_core_loadExternalSymbol_f loadExternalSymbol_f;
        nh_core_addModule_f addModule_f;
        nh_Module Modules_p[NH_MODULE_E_COUNT];
        nh_Array ExternalModules;
    } nh_Loader;

/** @} */

/** @addtogroup lib_nhcore_vars
 *  @{
 */

    extern nh_Loader NH_LOADER;
    extern const NH_BYTE *NH_MODULE_NAMES_PP[];

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    void *nh_core_loadExistingSymbol(
        NH_MODULE_E _module, int major, const NH_BYTE *functionName_p
    );

    void *nh_core_loadSymbolUsingModuleName(
        NH_BYTE *moduleName_p, NH_BYTE *symbolName_p
    );

    nh_Loader *nh_core_initLoader(
        NH_LOADER_SCOPE_E scope, NH_BOOL fallback, NH_BOOL install
    );

    NH_CORE_RESULT nh_core_freeLoader(
    );

    int nh_core_getModuleIndex(
        NH_BYTE *name_p
    );

/** @} */

#endif
