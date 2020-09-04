#ifndef NH_BUILD_LIBRARY_H
#define NH_BUILD_LIBRARY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"

#endif

/** @addtogroup BuildFunctions Functions
 *  \ingroup Build
 *  @{
 */

    NH_RESULT Nh_Bld_buildLibrary(
    );

    NH_RESULT Nh_Bld_copyPublicLibraryHeader(
    );

    NH_RESULT Nh_Bld_installLibrary(
    );

    NH_RESULT Nh_Bld_createLibraryIncludes(
    );

/** @} */

#endif
