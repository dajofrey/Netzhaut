#ifndef NH_ECMASCRIPT_INTERNAL_SLOT_H
#define NH_ECMASCRIPT_INTERNAL_SLOT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-ecmascript_enums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_INTERNAL_SLOT {
        NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE,
        NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE,
        NH_ECMASCRIPT_INTERNAL_SLOT_REALM,
        NH_ECMASCRIPT_INTERNAL_SLOT_INITIAL_NAME,
        NH_ECMASCRIPT_INTERNAL_SLOT_BUILTIN,
        NH_ECMASCRIPT_INTERNAL_SLOT_ENVIRONMENT,
        NH_ECMASCRIPT_INTERNAL_SLOT_FORMAL_PARAMETERS,
        NH_ECMASCRIPT_INTERNAL_SLOT_ECMASCRIPT_CODE,
        NH_ECMASCRIPT_INTERNAL_SLOT_CONSTRUCTOR_KIND,
        NH_ECMASCRIPT_INTERNAL_SLOT_SCRIPT_OR_MODULE,
        NH_ECMASCRIPT_INTERNAL_SLOT_THIS_MODE,
        NH_ECMASCRIPT_INTERNAL_SLOT_STRICT,
        NH_ECMASCRIPT_INTERNAL_SLOT_HOME_OBJECT,
        NH_ECMASCRIPT_INTERNAL_SLOT_SOURCE_TEXT,
        NH_ECMASCRIPT_INTERNAL_SLOT_IS_CLASS_CONSTRUCTOR,
    } NH_ECMASCRIPT_INTERNAL_SLOT;

/** @} */

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_InternalSlots {
        int maxLookup;
        const int *lookup_p;
        void **values_pp;
    } nh_ecmascript_InternalSlots;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */ 

    nh_ecmascript_InternalSlots nh_ecmascript_initInternalSlots(
        const int *lookup_p, int length
    );

    void *nh_ecmascript_getInternalSlot(
        nh_ecmascript_InternalSlots *InternalSlots_p, NH_ECMASCRIPT_INTERNAL_SLOT slot
    );

    NH_ECMASCRIPT_RESULT nh_ecmascript_setInternalSlot(
        nh_ecmascript_InternalSlots *InternalSlots_p, NH_ECMASCRIPT_INTERNAL_SLOT slot, void *value_p
    );

/** @} */

#endif
