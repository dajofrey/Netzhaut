// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "PropertyDescriptor.h"
#include "Properties.h"

#include "../Common/Macros.h"

#include <string.h>

// INIT ============================================================================================

nh_ecmascript_PropertyDescriptor nh_ecmascript_undefinedPropertyDescriptor()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_PropertyDescriptor Descriptor;
    Descriptor.type = -1;

NH_ECMASCRIPT_END(Descriptor)
}

// ABSTRACT OPERATIONS =============================================================================

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isAccessorDescriptor(
    nh_ecmascript_PropertyDescriptor Descriptor)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Descriptor.type == NH_ECMASCRIPT_PROPERTY_ACCESSOR)
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isDataDescriptor (
    nh_ecmascript_PropertyDescriptor Descriptor)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Descriptor.type == NH_ECMASCRIPT_PROPERTY_DATA)
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isGenericDescriptor(
    nh_ecmascript_PropertyDescriptor Descriptor)
{
NH_ECMASCRIPT_BEGIN()

    if (Descriptor.type == -1) {
        NH_ECMASCRIPT_END(false)
    }

    if (!nh_ecmascript_isAccessorDescriptor(Descriptor) && !nh_ecmascript_isDataDescriptor(Descriptor)) {
        NH_ECMASCRIPT_END(true)
    }

NH_ECMASCRIPT_END(false)
}

