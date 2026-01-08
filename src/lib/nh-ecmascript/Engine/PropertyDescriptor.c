// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "PropertyDescriptor.h"
#include "TestAndCompare.h"

#include <string.h>

// FUNCTIONS =======================================================================================

uint8_t nh_ecmascript_getAttributeFlags(
    nh_ecmascript_PropertyDescriptor *desc)
{
    uint8_t attrs = 0;
    // If not specified, the spec says default to false for new properties
    if (desc->flags.hasWritable && desc->flags.writable)         attrs |= 0x01;
    if (desc->flags.hasEnumerable && desc->flags.enumerable)     attrs |= 0x02;
    if (desc->flags.hasConfigurable && desc->flags.configurable) attrs |= 0x04;
    return attrs;
}

nh_ecmascript_PropertyDescriptor nh_ecmascript_getPropertyDescriptor(
    nh_ecmascript_Value value, 
    uint8_t attrs) 
{
    nh_ecmascript_PropertyDescriptor desc = {0};

    // 1. Data properties always have these present
    desc.flags.hasValue = true;
    desc.Value = value;

    desc.flags.hasWritable = true;
    desc.flags.writable = (attrs & 0x01) ? 1 : 0;

    desc.flags.hasEnumerable = true;
    desc.flags.enumerable = (attrs & 0x02) ? 1 : 0;

    desc.flags.hasConfigurable = true;
    desc.flags.configurable = (attrs & 0x04) ? 1 : 0;

    return desc;
}

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
//void nh_ecmascript_applyDescriptorToProperty(
//    nh_ecmascript_PropertyDescriptor *Desc_p, 
//    nh_ecmascript_Property *Prop_p) 
//{
//    if (Desc_p->flags.hasEnumerable)   Prop_p->enumerable = Desc_p->flags.enumerable;
//    if (Desc_p->flags.hasConfigurable) Prop_p->configurable = Desc_p->flags.configurable;
//    
//    if (Prop_p->isAccessor) {
//        if (Desc_p->flags.hasGet) Prop_p->accessor.get = Desc_p->Get;
//        if (Desc_p->flags.hasSet) Prop_p->accessor.set = Desc_p->Set;
//    } else {
//        if (Desc_p->flags.hasValue)    Prop_p->data.value = Desc_p->Value;
//        if (Desc_p->flags.hasWritable) Prop_p->data.writable = Desc_p->flags.writable;
//    }
//}

/**
 * https://tc39.es/ecma262/#sec-topropertydescriptor
 * Note: Desc_out should be allocated by the caller to avoid scope issues.
 */
nh_ecmascript_Completion nh_ecmascript_toPropertyDescriptor(
    nh_ecmascript_Value ObjVal, 
    nh_ecmascript_PropertyDescriptor *Desc_out,
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. Type Check
    if (!nh_ecmascript_isObject(ObjVal)) {
        return nh_ecmascript_throwTypeError();
    }
    nh_ecmascript_Object *Obj_p = nh_ecmascript_toObject(ObjVal, Realm_p).Value.p.object;

    // Initialize the descriptor (sets all 'has' flags to false)
    memset(Desc_out, 0, sizeof(nh_ecmascript_PropertyDescriptor));

    // 2. Helper Logic for each field
    char *fields[] = {"enumerable", "configurable", "value", "writable", "get", "set"};
    
    for (int i = 0; i < 6; i++) {
        // HasProperty?
        nh_ecmascript_Completion hasComp = Obj_p->Methods_p->hasProperty(Obj_p, fields[i]);
        if (hasComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return hasComp;

        if (nh_ecmascript_toBool(hasComp.Value)) {
            // Get the value
            nh_ecmascript_Completion getComp = Obj_p->Methods_p->get(Obj_p, fields[i], Obj_p);
            if (getComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return getComp;
            nh_ecmascript_Value val = getComp.Value;

            // Map to struct fields
            if (i == 0) { Desc_out->flags.hasEnumerable = true; Desc_out->flags.enumerable = nh_ecmascript_toBool(val); }
            else if (i == 1) { Desc_out->flags.hasConfigurable = true; Desc_out->flags.configurable = nh_ecmascript_toBool(val); }
            else if (i == 2) { Desc_out->flags.hasValue = true; Desc_out->Value = val; }
            else if (i == 3) { Desc_out->flags.hasWritable = true; Desc_out->flags.writable = nh_ecmascript_toBool(val); }
            else if (i == 4) { 
                if (!nh_ecmascript_isObject(val) && !nh_ecmascript_isUndefined(val)) return nh_ecmascript_throwTypeError();
                Desc_out->flags.hasGet = true; Desc_out->Set = nh_ecmascript_isObject(val) ? nh_ecmascript_toObject(val, Realm_p).Value.p.object : NULL;
            }
            else if (i == 5) { 
                if (!nh_ecmascript_isObject(val) && !nh_ecmascript_isUndefined(val)) return nh_ecmascript_throwTypeError();
                Desc_out->flags.hasSet = true; Desc_out->Set = nh_ecmascript_isObject(val) ? nh_ecmascript_toObject(val, Realm_p).Value.p.object : NULL;
            }
        }
    }

    // 3. Validation: Integrity Check
    if ((Desc_out->flags.hasValue || Desc_out->flags.hasWritable) && (Desc_out->flags.hasGet || Desc_out->flags.hasSet)) {
        return nh_ecmascript_throwTypeError();
    }

    return nh_ecmascript_normalEmptyCompletion();
}
