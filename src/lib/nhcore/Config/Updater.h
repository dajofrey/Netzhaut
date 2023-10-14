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

/** @addtogroup lib_nhcore_typedefs
 *  @{
 */

    typedef NH_CORE_RESULT (*nh_core_registerConfig_f)(
        NH_BYTE *absolutePath_p, int length
    );
    
    typedef NH_CORE_RESULT (*nh_core_loadConfig_f)(
        NH_BYTE *data_p, int length
    );

/** @ } */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    NH_CORE_RESULT nh_core_startConfigUpdater(
    );

/** @} */

#endif // NH_CORE_CONFIG_UPDATER_H
