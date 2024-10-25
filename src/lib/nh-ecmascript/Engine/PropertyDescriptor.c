// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "PropertyDescriptor.h"
#include "Properties.h"

#include <string.h>

// FUNCTIONS =======================================================================================

nh_ecmascript_PropertyDescriptor nh_ecmascript_undefinedPropertyDescriptor()
{
    nh_ecmascript_PropertyDescriptor Descriptor;
    Descriptor.type = -1;

    return Descriptor;
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isAccessorDescriptor(
    nh_ecmascript_PropertyDescriptor Descriptor)
{
    return Descriptor.type == NH_ECMASCRIPT_PROPERTY_ACCESSOR;
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isDataDescriptor (
    nh_ecmascript_PropertyDescriptor Descriptor)
{
    return Descriptor.type == NH_ECMASCRIPT_PROPERTY_DATA;
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isGenericDescriptor(
    nh_ecmascript_PropertyDescriptor Descriptor)
{
    if (Descriptor.type == -1) {
        return false;
    }

    if (!nh_ecmascript_isAccessorDescriptor(Descriptor) && !nh_ecmascript_isDataDescriptor(Descriptor)) {
        return true;
    }

    return false;
}
