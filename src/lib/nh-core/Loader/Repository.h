#ifndef NH_CORE_REPOSITORY_H
#define NH_CORE_REPOSITORY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_getExeDir(
        NH_BYTE *buffer_p, unsigned int size
    );

    NH_CORE_RESULT nh_downloadnetzhaut(
    );
    
    NH_CORE_RESULT nh_core_buildnhinstaller(
    );

/** @} */

#endif
