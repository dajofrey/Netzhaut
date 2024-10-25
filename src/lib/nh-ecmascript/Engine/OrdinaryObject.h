#ifndef NH_ECMASCRIPT_ORDINARY_OBJECT_H
#define NH_ECMASCRIPT_ORDINARY_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Completion.h"
#include "InternalSlots.h"
#include "Properties.h"
#include "ObjectType.h"

#include "../../nh-core/Util/List.h"

#endif

/** @addtogroup lib_nh-ecmascript_vars
 *  @{
 */

    extern const int NH_ECMASCRIPT_ORDINARY_OBJECT_LOOKUP_P[];
    extern nh_ecmascript_InternalMethods NH_ECMASCRIPT_ORDINARY_OBJECT_INTERNAL_METHODS;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions Functions
 *  @{
 */

    nh_ecmascript_Object *nh_ecmascript_ordinaryGetPrototypeOf(
        nh_ecmascript_Object *This_p
    );
    
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinarySetPrototypeOf(
        nh_ecmascript_Object *This_p
    );
    
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryIsExtensible(
        nh_ecmascript_Object *This_p
    );
    
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryPreventExtensions(
        nh_ecmascript_Object *This_p
    );
    
    nh_ecmascript_PropertyDescriptor nh_ecmascript_ordinaryGetOwnProperty(
        nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey
    );
    
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryDefineOwnProperty(
        nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_PropertyDescriptor Desc
    );
    
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryHasProperty(
        nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey
    );
    
    nh_ecmascript_Any nh_ecmascript_ordinaryGet(
        nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any Receiver
    );
    
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinarySet(
        nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any V, nh_ecmascript_Any Receiver
    );
    
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryDelete(
        nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey
    );
    
    nh_core_List nh_ecmascript_ordinaryOwnPropertyKeys(
        nh_ecmascript_Object *This_p
    );

    // https://tc39.es/ecma262/#sec-ordinaryobjectcreate
    nh_ecmascript_Object *nh_ecmascript_ordinaryObjectCreate(
        nh_ecmascript_Object *Proto_p, const int *lookup_p, int lookupLength
    );

/** @} */

#endif
