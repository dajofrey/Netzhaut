#ifndef NH_BUILD_UTILS_H
#define NH_BUILD_UTILS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"

#include <stddef.h>

#endif

/** @addtogroup BuildData Data
 *  \ingroup Build
 *  @{
 */

    extern int NH_ERROR;

/** @} */

/** @addtogroup BuildFunctions Functions
 *  \ingroup Build
 *  @{
 */

    NH_RESULT Nh_Bld_getTopDirectory(
        char *buffer_p, size_t size
    );

    NH_BOOL Nh_Bld_canRunCommand(
        const char *cmd
    );

    NH_BOOL Nh_Bld_canFindSharedLib(
        const char *lib_p
    );

    NH_RESULT Nh_Bld_createDirectory(
        const char *path_p
    );

    NH_RESULT Nh_Bld_createPICObjectFileUsingGCC(
        const char *in_p, const char *out_p, char *extra_p
    );

    NH_RESULT Nh_Bld_copy(
        char *in_p, char *out_p, NH_BOOL recursive, NH_BOOL toAbsolute, NH_BOOL sudo
    );

    const char *Nh_Bld_getHomeDirectory(
    );

/** @} */

#endif
