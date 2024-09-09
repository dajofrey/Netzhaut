#ifndef NH_ECMASCRIPT_WEB_BINDING_H
#define NH_ECMASCRIPT_WEB_BINDING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Intrinsics.h"

#endif

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_TemplateMapEntry {
        void *Site_p;
        nh_ecmascript_Object Array;
    } nh_ecmascript_TemplateMapEntry;

    // https://tc39.es/ecma262/#sec-code-realms
    typedef struct nh_ecmascript_Realm {
        nh_ecmascript_Intrinsics Intrinsics;
        nh_ecmascript_Object *GlobalObject_p;
        nh_ecmascript_Environment *GlobalEnvironment_p;
    } nh_ecmascript_Realm;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_ecmascript_Realm *nh_ecmascript_createRealm(
    );

/** @} */

#endif
