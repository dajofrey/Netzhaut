#ifndef NH_ECMASCRIPT_OBJECT_H
#define NH_ECMASCRIPT_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Engine/Realm.h"

#endif

/** @addtogroup ECMAScriptFunctions Functions
 *  \ingroup ECMAScript 
 *  @{
 */

    NH_API_RESULT nh_ecmascript_createIntrinsicObject(
        nh_ecmascript_IntrinsicObject *Object_p, nh_ecmascript_Realm *Realm_p
    );

/** @} */

#endif
