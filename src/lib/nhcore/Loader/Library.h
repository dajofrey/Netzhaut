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

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_CORE_RESULT nh_installLibrary(
        NH_MODULE_E _module
    );
    
    void *nh_core_loadLibrary(
        NH_MODULE_E _module, int major
    );
    
    void *nh_core_loadExternalLibrary(
        NH_BYTE *name_p
    );

    void *nh_core_loadSymbolFromLibrary(
        void *lib_p, const NH_BYTE *name_p
    );

    NH_CORE_RESULT nh_unloadLibrary(
        void *lib_p
    );

/** @} */

#endif
