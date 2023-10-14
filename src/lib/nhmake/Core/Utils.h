#ifndef NH_MAKE_UTILS_H
#define NH_MAKE_UTILS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stddef.h>

#endif

/** @addtogroup lib_nhmake_typedefs
 *  @{
 */

    typedef NH_BYTE *(*nh_make_getProcessDirectory_f)(
    );
    
    typedef NH_BYTE *(*nh_make_getWorkDirectory_f)(
    );

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    const NH_BYTE *nh_make_getHomeDir(
    );

    NH_MAKE_RESULT nh_make_createDir(
        NH_BYTE *path_p
    );
    
    NH_MAKE_RESULT nh_make_removeDir(
        NH_BYTE *path_p
    );

    NH_MAKE_BOOL nh_make_canRunCommand(
        const NH_BYTE *cmd
    );

    NH_BYTE *nh_make_getWorkDirectory(
    );

/** @} */

#endif
