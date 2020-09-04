#ifndef NH_BUILD_EXTERNALS_H
#define NH_BUILD_EXTERNALS_H

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

    NH_RESULT Nh_Bld_buildExternals(
    );

    NH_RESULT Nh_Bld_copyExternalHeader(
    );

    NH_RESULT Nh_Bld_installHelperLibrary(
    );

/** @} */

#endif
