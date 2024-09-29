// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "ObjectType.h"
#include "TestAndCompare.h"
#include "OrdinaryObject.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>

// ABSTRACT ========================================================================================

nh_ecmascript_Object *nh_ecmascript_abstractMakeBasicObject(
    const int *lookup_p, int lookupLength)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Object *Obj_p = nh_core_allocate(sizeof(nh_ecmascript_Object));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, Obj_p)

    Obj_p->Properties = nh_ecmascript_initProperties(8);
    Obj_p->InternalSlots = nh_ecmascript_initInternalSlots(lookup_p, lookupLength);
    Obj_p->InternalMethods_p = &NH_ECMASCRIPT_ORDINARY_OBJECT_INTERNAL_METHODS; 

    nh_ecmascript_setInternalSlot(&Obj_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE, (void*) true);

NH_ECMASCRIPT_END(Obj_p)
}

static NH_API_RESULT nh_ecmascript_allocatePropertyKeyAndValue(
    nh_ecmascript_Any *PropertyKey_p, nh_ecmascript_PropertyDescriptor *PropertyDescriptor_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (PropertyKey_p->type)
    {
        case NH_ECMASCRIPT_TYPE_STRING : ;

            nh_ecmascript_String *Name_p = PropertyKey_p->handle_p;
            nh_ecmascript_String *AllocatedName_p = nh_ecmascript_allocateString(Name_p->length);
            NH_ECMASCRIPT_CHECK_MEM(AllocatedName_p);
            NH_ECMASCRIPT_CHECK(nh_ecmascript_appendFormatToString(AllocatedName_p, Name_p->p, Name_p->length))
            *PropertyKey_p = nh_ecmascript_wrapString(AllocatedName_p);
            break;

        default :
            NH_ECMASCRIPT_END(NH_API_ERROR_BAD_STATE)
    }

    if (PropertyDescriptor_p->type == NH_ECMASCRIPT_PROPERTY_DATA)
    {
        switch (PropertyDescriptor_p->Fields.Data.Value.type)
        {
            case NH_ECMASCRIPT_TYPE_NUMBER : ;

                NH_ECMASCRIPT_NUMBER *number_p = nh_ecmascript_allocateNumber(
                    *((NH_ECMASCRIPT_NUMBER*)PropertyDescriptor_p->Fields.Data.Value.handle_p)
                );
                NH_ECMASCRIPT_CHECK_MEM(number_p)
                PropertyDescriptor_p->Fields.Data.Value.handle_p = number_p;    
                break;

            case NH_ECMASCRIPT_TYPE_OBJECT :
                break;

            default :
                NH_ECMASCRIPT_END(NH_API_ERROR_BAD_STATE)
        }
    }
    else {
        NH_ECMASCRIPT_END(NH_API_ERROR_BAD_STATE)
    }

NH_ECMASCRIPT_END(NH_API_SUCCESS)
}

nh_ecmascript_Completion nh_ecmascript_abstractDefinePropertyOrThrow(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_PropertyDescriptor PropertyDescriptor)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_allocatePropertyKeyAndValue(&PropertyKey, &PropertyDescriptor);

    NH_ECMASCRIPT_BOOLEAN success = Object_p->InternalMethods_p->defineOwnProperty_f(
        Object_p, PropertyKey, PropertyDescriptor
    );

    if (success == false) {NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())}

NH_ECMASCRIPT_END(nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(success)))
}

bool nh_ecmascript_abstractHasProperty(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Any PropertyKey)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Object_p->InternalMethods_p->hasProperty_f(Object_p, PropertyKey))
}

bool nh_ecmascript_abstractHasOwnProperty(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Any PropertyKey)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_PropertyDescriptor Descriptor = Object_p->InternalMethods_p->getOwnProperty_f(Object_p, PropertyKey);
    if (Descriptor.type == -1) {NH_ECMASCRIPT_END(false)}

NH_ECMASCRIPT_END(true)
}

bool nh_ecmascript_abstractIsExtensible(
    nh_ecmascript_Object *Object_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Object_p->InternalMethods_p->isExtensible_f(Object_p))
}

nh_ecmascript_Completion nh_ecmascript_abstractSet(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Any Key, nh_ecmascript_Any Value, bool _throw)
{
NH_ECMASCRIPT_BEGIN()

    bool success = Object_p->InternalMethods_p->set_f(Object_p, Key, Value, nh_ecmascript_wrapObject(Object_p));
    if (!success && _throw) {NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())}

NH_ECMASCRIPT_END(nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(success)))
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_abstractCreateDataProperty(
    nh_ecmascript_Object *Object_p, nh_ecmascript_Any PropertyKey, nh_ecmascript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_PropertyDescriptor Descriptor;
    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
    Descriptor.enumerable = true;
    Descriptor.configurable = true;
    Descriptor.Fields.Data.Value = Value;
    Descriptor.Fields.Data.writable = true; 

NH_ECMASCRIPT_END(Object_p->InternalMethods_p->defineOwnProperty_f(Object_p, PropertyKey, Descriptor))
}

nh_ecmascript_Completion nh_ecmascript_abstractCall(
    nh_ecmascript_Any F, nh_ecmascript_Any Value, nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

    if (!nh_ecmascript_isCallable(F)) {NH_ECMASCRIPT_END(nh_ecmascript_throwTypeError())}

NH_ECMASCRIPT_END(((nh_ecmascript_Object*)Value.handle_p)->InternalMethods_p->call_f(Value.handle_p, Value, Arguments))
}

