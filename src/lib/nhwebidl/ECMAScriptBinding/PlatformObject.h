#ifndef NH_WEBIDL_PLATFORM_OBJECT_H
#define NH_WEBIDL_PLATFORM_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhwebidl_functions
 *  @{
 */

    NH_ECMAScript_Object *nh_webidl_createObjectImplementingInterface(
        nh_webidl_Interface *Interface_p, nh_ecmascript_Realm *Realm_p
    );

/** @} */

#endif
