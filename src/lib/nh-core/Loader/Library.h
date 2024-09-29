#ifndef NH_CORE_LIBRARY_H
#define NH_CORE_LIBRARY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Loader.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_API_RESULT nh_installLibrary(
        NH_MODULE_E type 
    );
    
    void *nh_core_loadLibrary(
        NH_MODULE_E type, char *path_p
    );
    
    void *nh_core_loadExternalLibrary(
        char *name_p, char *path_p
    );

    void *nh_core_loadSymbolFromLibrary(
        void *lib_p, const char *name_p
    );

    NH_API_RESULT nh_unloadLibrary(
        void *lib_p
    );

/** @} */

#endif
