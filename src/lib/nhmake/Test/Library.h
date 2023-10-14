#ifndef NH_MAKE_LIBRARY_H
#define NH_MAKE_LIBRARY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stddef.h>

#endif

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    void *nh_make_openLibrary(
        NH_BYTE *path_p
    );
    
    void nh_make_closeLibrary(
        void *lib_p
    );
    
    NH_MAKE_RESULT nh_make_getExeDir(
        NH_BYTE *buffer_p, size_t size
    );
    
    void *nh_make_loadSymbol(
        void *lib_p, const NH_BYTE *name_p
    );

/** @} */

#endif
