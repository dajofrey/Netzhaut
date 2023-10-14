#ifndef NH_ECMASCRIPT_BUILTIN_FUNCTION_OBJECT_H
#define NH_ECMASCRIPT_BUILTIN_FUNCTION_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "ObjectType.h"
#include "Realm.h"

#endif

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

    nh_ecmascript_Object *nh_ecmascript_createBuiltinFunctionObject(
        nh_ecmascript_Object *FunctionObject_p, nh_ecmascript_Any (*call_f)(nh_ecmascript_Object *This_p, nh_List Arguments),
        int *lookup_p, int lookupLength, nh_ecmascript_Realm *Realm_p, nh_ecmascript_Object *Prototype_p
    );

/** @} */

#endif
