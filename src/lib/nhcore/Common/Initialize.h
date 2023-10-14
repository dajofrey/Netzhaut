#ifndef NH_CORE_INITIALIZE_H
#define NH_CORE_INITIALIZE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#endif

/** @addtogroup lib_nhcore_typedefs
 *  @{
 */

    typedef nh_Loader *(*nh_core_initialize_f)(
        NH_LOADER_SCOPE_E scope, NH_BYTE *path_p, NH_BYTE *config_p, int length
    );

/** @} */

#endif
