// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "PropertyDescriptor.h"
#include <string.h>

// FUNCTIONS =======================================================================================

nh_ecmascript_PropertyDescriptor nh_ecmascript_undefinedPropertyDescriptor()
{
    nh_ecmascript_PropertyDescriptor Descriptor;
    return Descriptor;
}

// https://tc39.es/ecma262/#sec-isaccessordescriptor
bool nh_ecmascript_isAccessorDescriptor(
    nh_ecmascript_PropertyDescriptor Descriptor) 
{
    if (!Descriptor.flags.hasGet && !Descriptor.flags.hasSet) return false;
    return true;
}

// https://tc39.es/ecma262/#sec-isdatadescriptor
bool nh_ecmascript_isDataDescriptor(
    nh_ecmascript_PropertyDescriptor Descriptor) 
{
    if (!Descriptor.flags.hasValue && !Descriptor.flags.hasWritable) return false;
    return true;
}

// https://tc39.es/ecma262/#sec-isgenericdescriptor
bool nh_ecmascript_isGenericDescriptor(
    nh_ecmascript_PropertyDescriptor Descriptor) 
{
    if (nh_ecmascript_isAccessorDescriptor(Descriptor)) {return false;}
    if (nh_ecmascript_isDataDescriptor(Descriptor)) {return false;}
    return true;
}

// https://tc39.es/ecma262/#sec-on-property-descriptor-to-property
void nh_ecmascript_applyDescriptorToProperty(
    nh_ecmascript_PropertyDescriptor *Desc_p, 
    nh_ecmascript_Property *Prop_p) 
{
    if (Desc_p->flags.hasEnumerable)   Prop_p->enumerable = Desc_p->flags.enumerable;
    if (Desc_p->flags.hasConfigurable) Prop_p->configurable = Desc_p->flags.configurable;
    
    if (Prop_p->isAccessor) {
        if (Desc_p->flags.hasGet) Prop_p->accessor.get = Desc_p->Get;
        if (Desc_p->flags.hasSet) Prop_p->accessor.set = Desc_p->Set;
    } else {
        if (Desc_p->flags.hasValue)    Prop_p->data.value = Desc_p->Value;
        if (Desc_p->flags.hasWritable) Prop_p->data.writable = Desc_p->flags.writable;
    }
}

// https://tc39.es/ecma262/#sec-topropertydescriptor
nh_ecmascript_Completion nh_ecmascript_toPropertyDescriptor(
    nh_ecmascript_Value ObjVal, 
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. If Type(ObjVal) is not Object, throw a TypeError
    if (ObjVal.tag != NH_ECMASCRIPT_VALUE_OBJECT) {
        return nh_ecmascript_throwTypeError("Property description must be an object", Realm_p);
    }
    nh_ecmascript_Object *Obj_p = ObjVal.p.object;
    nh_ecmascript_PropertyDescriptor desc = {0}; // Initialize all 'has' fields to false

    // 2. Helper for checking and fetching fields
    // We check "enumerable", "configurable", "value", "writable", "get", "set"
    
    // Example: Handle "enumerable"
	    if (nh_ecmascript_hasProperty(Obj_p, "enumerable", Realm_p)) {
        desc.flags.hasEnumerable = true;
//        desc.flags.enumerable = nh_ecmascript_toBoolean(nh_ecmascript_get(Obj_p, "enumerable", Realm_p));
    }

    // Example: Handle "value"
    if (nh_ecmascript_hasProperty(Obj_p, "value", Realm_p)) {
        desc.flags.hasValue = true;
//        desc.Value = nh_ecmascript_get(Obj_p, "value", Realm_p);
    }

    // 3. Validation: A descriptor cannot have both (Value/Writable) and (Get/Set)
//    if ((desc.hasValue || desc.hasWritable) && (desc.hasGet || desc.hasSet)) {
//        return nh_ecmascript_throwTypeError(
//            "Invalid property descriptor. Cannot both specify accessors and a value or writable attribute", 
//            Realm_p
//        );
//    }

    // Wrap the C struct in a Completion (You might need a pointer or a specialized Value tag)
    // Usually, you return this as a specialized internal type.
    return nh_ecmascript_normalCompletion(nh_ecmascript_makeInternalPointer(&desc));
}
