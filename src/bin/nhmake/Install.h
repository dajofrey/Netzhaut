#ifndef NH_MAKE_LIBRARIES_H
#define NH_MAKE_LIBRARIES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

#include "../Parser/Functions.h"
#include "../Common/Result.h"
#include "../Common/Types.h"

#endif

/** @addtogroup bin_nhmake_functions
 *  @{
 */

    NH_MAKE_RESULT nh_make_prepareLibraryBuild(
    );
    
    NH_MAKE_RESULT nh_make_buildLibrary(
        nh_make_Library *Library_p, NH_BOOL installLibraries
    ); 

    NH_MAKE_RESULT nh_make_buildnhexternal(
        NH_BOOL install
    );

/** @} */

#endif
