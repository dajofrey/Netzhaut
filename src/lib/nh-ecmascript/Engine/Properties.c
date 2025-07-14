// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Properties.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// PROPERTIES ======================================================================================

nh_ecmascript_Properties nh_ecmascript_initProperties(
    int chunkSize)
{
    nh_ecmascript_Properties Properties = nh_core_initList(chunkSize);

    return Properties;
}

bool nh_ecmascript_equalPropertyKeys(
    nh_ecmascript_Any Key1, nh_ecmascript_Any Key2)
{
    if (Key1.type == NH_ECMASCRIPT_TYPE_STRING && Key2.type == NH_ECMASCRIPT_TYPE_STRING) {
        if (Key1.handle_p != NULL && Key2.handle_p != NULL && !strcmp(Key1.handle_p, Key2.handle_p)) {
            return true;
        }
    }

    return false;
}

nh_ecmascript_Property *nh_ecmascript_newProperty(
    nh_ecmascript_Properties *Properties_p, nh_ecmascript_Any Key, nh_ecmascript_PropertyDescriptor Descriptor)
{
    if (Descriptor.type == -1) {return NULL;}

    nh_ecmascript_Property *Property_p = (nh_ecmascript_Property*)nh_core_allocate(sizeof(nh_ecmascript_Property));
    NH_CORE_CHECK_MEM_2(NULL, Property_p)

    Property_p->type = Descriptor.type;
    Property_p->Key  = Key;
    Property_p->enumerable   = Descriptor.enumerable;
    Property_p->configurable = Descriptor.configurable;

    if (Property_p->type == NH_ECMASCRIPT_PROPERTY_DATA) {
        Property_p->Fields.Data.Value    = Descriptor.Fields.Data.Value;
        Property_p->Fields.Data.writable = Descriptor.Fields.Data.writable;
    }
    else {
        Property_p->Fields.Accessor.Get = Descriptor.Fields.Accessor.Get;
        Property_p->Fields.Accessor.Set = Descriptor.Fields.Accessor.Set;
    }

    return Property_p;
}
