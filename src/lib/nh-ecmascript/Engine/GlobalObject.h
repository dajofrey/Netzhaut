#ifndef NH_ECMASCRIPT_GLOBAL_OBJECT_H
#define NH_ECMASCRIPT_GLOBAL_OBJECT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Engine/Realm.h"

/** @addtogroup ECMAScriptFunctions Functions
 *  \ingroup ECMAScript 
 *  @{
 */

    nh_ecmascript_Object *nh_ecmascript_setDefaultGlobalBindings(
        nh_ecmascript_Realm *Realm_p
    );
    
    NH_API_RESULT nh_ecmascript_freeDefaultGlobalBindings(
        nh_ecmascript_Object *Object_p
    );
    
    NH_API_RESULT nh_ecmascript_installHostGlobals(
        nh_ecmascript_Realm *Realm_p, nh_api_HostGlobals *HostGlobals_p
    );

/** @} */

#endif
