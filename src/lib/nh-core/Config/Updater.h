#ifndef NH_CORE_CONFIG_UPDATER_H
#define NH_CORE_CONFIG_UPDATER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Loader/Loader.h"

#endif

/** @addtogroup lib_nh-core_typedefs
 *  @{
 */

    typedef NH_API_RESULT (*nh_core_registerConfig_f)(
        char *absolutePath_p, int length
    );
    
    typedef NH_API_RESULT (*nh_core_loadConfig_f)(
        char *data_p, int length
    );

/** @ } */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    NH_API_RESULT nh_core_startConfigUpdater(
    );

/** @} */

#endif // NH_CORE_CONFIG_UPDATER_H
