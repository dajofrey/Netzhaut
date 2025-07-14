// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "InternalSlots.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// LOOKUP ==========================================================================================

static int ordinaryLookup_p[] = {
    0, // [[Extensible]]
    1, // [[Prototype]]
};

// FUNCTIONS =======================================================================================

nh_ecmascript_InternalSlots nh_ecmascript_initInternalSlots(
    const int *lookup_p, int length)
{
    nh_ecmascript_InternalSlots InternalSlots;
    InternalSlots.lookup_p = NULL;
    InternalSlots.values_pp = NULL;

    if (lookup_p != NULL && length > 0) 
    {
        int valueCount = 0;

        for (int i = 0; i < length; ++i) {
            if (lookup_p[i] > -1) {valueCount++;}
        }

        if (valueCount > 0)
        {
            InternalSlots.maxLookup = length;
            InternalSlots.lookup_p  = lookup_p;
            InternalSlots.values_pp = (void**)nh_core_allocate(sizeof(void*) * valueCount);
        }
    }
    else {
        InternalSlots.maxLookup = sizeof(ordinaryLookup_p) / sizeof(ordinaryLookup_p[0]);
        InternalSlots.lookup_p  = ordinaryLookup_p;
        InternalSlots.values_pp = (void**)nh_core_allocate(sizeof(void*) * (sizeof(ordinaryLookup_p) / sizeof(ordinaryLookup_p[0])));
    }

    return InternalSlots;
}

void *nh_ecmascript_getInternalSlot(
    nh_ecmascript_InternalSlots *InternalSlots_p, NH_ECMASCRIPT_INTERNAL_SLOT slot)
{
    if (slot >= InternalSlots_p->maxLookup || slot < 0 || InternalSlots_p->lookup_p[slot] == -1) {
        return NULL;
    }

    return InternalSlots_p->values_pp[InternalSlots_p->lookup_p[slot]];
}

NH_API_RESULT nh_ecmascript_setInternalSlot(
    nh_ecmascript_InternalSlots *InternalSlots_p, NH_ECMASCRIPT_INTERNAL_SLOT slot, void *value_p)
{
    if (slot >= InternalSlots_p->maxLookup || slot < 0 || InternalSlots_p->lookup_p[slot] == -1) {
        return NH_API_ERROR_BAD_STATE;
    }

    InternalSlots_p->values_pp[InternalSlots_p->lookup_p[slot]] = value_p;

    return NH_API_SUCCESS;
}

