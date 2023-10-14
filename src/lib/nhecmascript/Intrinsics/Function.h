#ifndef NH_ECMASCRIPT_FUNCTION_H
#define NH_ECMASCRIPT_FUNCTION_H

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

    NH_ECMASCRIPT_RESULT nh_ecmascript_createIntrinsicFunction(
        nh_ecmascript_IntrinsicFunction *Function_p, nh_ecmascript_Realm *Realm_p
    );
    
    NH_ECMASCRIPT_RESULT nh_ecmascript_freeIntrinsicFunction(
        nh_ecmascript_IntrinsicFunction *Function_p
    );

/** @} */

#endif
