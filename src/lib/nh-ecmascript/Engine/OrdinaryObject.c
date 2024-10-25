// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "OrdinaryObject.h"
#include "TestAndCompare.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// ORDINARY INTERNAL METHODS =======================================================================

nh_ecmascript_Object *nh_ecmascript_ordinaryGetPrototypeOf(
    nh_ecmascript_Object *This_p)
{
    return nh_ecmascript_getInternalSlot(&This_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE);
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinarySetPrototypeOf(
    nh_ecmascript_Object *This_p)
{
    return false;
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryIsExtensible(
    nh_ecmascript_Object *This_p)
{
    return (bool)This_p->InternalSlots.values_pp[This_p->InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE]];
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryPreventExtensions(
    nh_ecmascript_Object *This_p)
{
    return false;
}

// https://tc39.es/ecma262/#sec-ordinarygetownproperty
nh_ecmascript_PropertyDescriptor nh_ecmascript_ordinaryGetOwnProperty(
    nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey)
{
    nh_ecmascript_Property *Property_p = NULL;

    for (int i = 0; i < This_p->Properties.size; ++i) 
    {
        nh_ecmascript_Property *CheckProperty_p = This_p->Properties.pp[i];
        if (nh_ecmascript_equalPropertyKeys(CheckProperty_p->Key, PropertyKey)) {
            Property_p = CheckProperty_p;
            break;
        }
    }

    if (Property_p == NULL) {
        return nh_ecmascript_undefinedPropertyDescriptor();
    }

    nh_ecmascript_PropertyDescriptor Descriptor;
    Descriptor.type = Property_p->type; 

    if (Property_p->type == NH_ECMASCRIPT_PROPERTY_DATA) {
        Descriptor.Fields.Data.Value = Property_p->Fields.Data.Value;
        Descriptor.Fields.Data.writable = Property_p->Fields.Data.writable;
    }
    else {
        Descriptor.Fields.Accessor.Get = Property_p->Fields.Accessor.Get;
        Descriptor.Fields.Accessor.Set = Property_p->Fields.Accessor.Set;
    }

    Descriptor.enumerable = Property_p->enumerable;
    Descriptor.configurable = Property_p->configurable;

    return Descriptor;
}

// https://tc39.es/ecma262/#sec-validateandapplypropertydescriptor
static NH_ECMASCRIPT_BOOLEAN nh_ecmascript_validateAndApplyPropertyDescriptor(
    nh_ecmascript_Object *O_p, nh_ecmascript_Any P, NH_ECMASCRIPT_BOOLEAN extensible, 
    nh_ecmascript_PropertyDescriptor Desc, nh_ecmascript_PropertyDescriptor Current)
{
    if (Current.type == -1) 
    {
        if (extensible == false) {return false;}
        if (nh_ecmascript_isGenericDescriptor(Desc) || nh_ecmascript_isDataDescriptor(Desc)) {
            if (O_p != NULL) {
                if (nh_ecmascript_newProperty(&O_p->Properties, P, Desc) == NULL) {
                    return false;
                }
            }
        } 
        else {
            if (O_p != NULL) {
                if (nh_ecmascript_newProperty(&O_p->Properties, P, Desc) == NULL) {
                    return false;
                }
            }
        }
        return true;
    }

    if (Current.configurable == false) {

    }

    // TODO validation

    return false;
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryDefineOwnProperty(
    nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_PropertyDescriptor Desc)
{
    nh_ecmascript_PropertyDescriptor Current = This_p->InternalMethods_p->getOwnProperty_f(This_p, PropertyKey);
    nh_ecmascript_Completion IsExtensibleCompletion = nh_ecmascript_isExtensible(This_p);

    return nh_ecmascript_validateAndApplyPropertyDescriptor(This_p, PropertyKey, (NH_ECMASCRIPT_BOOLEAN)IsExtensibleCompletion.Value.Payload.handle_p, Desc, Current);
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryHasProperty(
    nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey)
{
    nh_ecmascript_PropertyDescriptor Descriptor = This_p->InternalMethods_p->getOwnProperty_f(This_p, PropertyKey);
    if (Descriptor.type != -1) {return true;}

    nh_ecmascript_Object *Parent_p = This_p->InternalMethods_p->getPrototypeOf_f(This_p);
    if (Parent_p != NULL) {
        return Parent_p->InternalMethods_p->hasProperty_f(Parent_p, PropertyKey);
    }

    return false;
}

nh_ecmascript_Any nh_ecmascript_ordinaryGet(
    nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any Receiver)
{
    return nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL);
}

static NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinarySetWithOwnDescriptor(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any Value, nh_ecmascript_Any Receiver,
    nh_ecmascript_PropertyDescriptor OwnDescriptor)
{
    if (OwnDescriptor.type == -1) {
        nh_ecmascript_Object *Parent_p = Object_p->InternalMethods_p->getPrototypeOf_f(Object_p);
        if (Parent_p != NULL) {
            return Parent_p->InternalMethods_p->set_f(Parent_p, PropertyKey, Value, Receiver);
        }
        else {
            OwnDescriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
            OwnDescriptor.enumerable = true;
            OwnDescriptor.configurable = true;
            OwnDescriptor.Fields.Data.writable = true;
            OwnDescriptor.Fields.Data.Value = nh_ecmascript_wrapUndefined();
        }
    }

    if (nh_ecmascript_isDataDescriptor(OwnDescriptor)) 
    {
        if (!OwnDescriptor.Fields.Data.writable) {return false;}
        if (Receiver.type != NH_ECMASCRIPT_TYPE_OBJECT) {return false;}

        nh_ecmascript_PropertyDescriptor ExistingDescriptor = 
            ((nh_ecmascript_Object*)Receiver.handle_p)->InternalMethods_p->getOwnProperty_f(Receiver.handle_p, PropertyKey);

        if (ExistingDescriptor.type != -1) 
        {
            if (nh_ecmascript_isAccessorDescriptor(ExistingDescriptor)) {return false;}
            if (!ExistingDescriptor.Fields.Data.writable) {return false;}

            nh_ecmascript_PropertyDescriptor ValueDescriptor;
            ValueDescriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
            ValueDescriptor.Fields.Data.Value = Value;

            return ((nh_ecmascript_Object*)Receiver.handle_p)->InternalMethods_p->defineOwnProperty_f(Receiver.handle_p, PropertyKey, ValueDescriptor);
        }
        else {
            return nh_ecmascript_abstractCreateDataProperty(Receiver.handle_p, PropertyKey, Value);
        }
    }

    nh_ecmascript_Any Setter = OwnDescriptor.Fields.Accessor.Set;
    if (Setter.type == NH_ECMASCRIPT_TYPE_UNDEFINED) {return false;}

    nh_core_List Arguments = nh_core_initList(1);
    nh_core_appendToList(&Arguments, &Value);

    nh_ecmascript_abstractCall(Setter, Receiver, Arguments);

    nh_core_freeList(&Arguments, false);

    return true;
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinarySet(
    nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any V, nh_ecmascript_Any Receiver)
{
    nh_ecmascript_PropertyDescriptor Descriptor = This_p->InternalMethods_p->getOwnProperty_f(This_p, PropertyKey);

    return nh_ecmascript_ordinarySetWithOwnDescriptor(This_p, PropertyKey, V, Receiver, Descriptor);
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_ordinaryDelete(
    nh_ecmascript_Object *This_p, nh_ecmascript_Any PropertyKey)
{
    return false;
}

nh_core_List nh_ecmascript_ordinaryOwnPropertyKeys(
    nh_ecmascript_Object *This_p)
{
    return nh_core_initList(8);
}

// DATA ============================================================================================

const int NH_ECMASCRIPT_ORDINARY_OBJECT_LOOKUP_P[] = {0, 1};

nh_ecmascript_InternalMethods NH_ECMASCRIPT_ORDINARY_OBJECT_INTERNAL_METHODS = {
    .getPrototypeOf_f    = nh_ecmascript_ordinaryGetPrototypeOf,
    .setPrototypeOf_f    = nh_ecmascript_ordinarySetPrototypeOf,
    .isExtensible_f      = nh_ecmascript_ordinaryIsExtensible,
    .preventExtensions_f = nh_ecmascript_ordinaryPreventExtensions,
    .getOwnProperty_f    = nh_ecmascript_ordinaryGetOwnProperty,
    .defineOwnProperty_f = nh_ecmascript_ordinaryDefineOwnProperty,
    .hasProperty_f       = nh_ecmascript_ordinaryHasProperty,
    .get_f               = nh_ecmascript_ordinaryGet,
    .set_f               = nh_ecmascript_ordinarySet,
    .delete_f            = nh_ecmascript_ordinaryDelete,
    .ownPropertyKeys_f   = nh_ecmascript_ordinaryOwnPropertyKeys,
    .call_f              = NULL,
    .construct_f         = NULL,
};

// OTHER ===========================================================================================

// https://tc39.es/ecma262/#sec-ordinaryobjectcreate
nh_ecmascript_Object *nh_ecmascript_ordinaryObjectCreate(
    nh_ecmascript_Object *Proto_p, const int *lookup_p, int lookupLength)
{
    nh_ecmascript_Object *NewObject_p = nh_ecmascript_abstractMakeBasicObject(
        lookup_p == NULL ? NH_ECMASCRIPT_ORDINARY_OBJECT_LOOKUP_P : lookup_p, lookup_p == NULL ? 2 : lookupLength
    );

    nh_ecmascript_setInternalSlot(&NewObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE, Proto_p);

    return NewObject_p;
}

