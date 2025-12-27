// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "OrdinaryFunctionObject.h"
#include "OrdinaryObject.h"
#include "TestAndCompare.h"
#include "ExecutionContext.h"

#include "../StaticSemantics/ParameterLists.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>

// ORDINARY FUNCTION OBJECT INTERNAL METHODS =======================================================

nh_ecmascript_Completion nh_ecmascript_ordinaryCall(
    nh_ecmascript_Object *This_p, nh_ecmascript_Any ThisArgument, nh_core_List ArgumentsList)
{
    return nh_ecmascript_normalEmptyCompletion();
}

// DATA ============================================================================================

static const int lookup_p[] = {0, 1, 2, -1, -1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

static nh_ecmascript_InternalMethods InternalMethods = {
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
    .call_f              = nh_ecmascript_ordinaryCall,
//    .construct_f         = NULL,
};

// OTHER ===========================================================================================

// https://tc39.es/ecma262/#sec-ordinaryfunctioncreate
nh_ecmascript_Object *nh_ecmascript_ordinaryFunctionCreate(
    nh_ecmascript_Object *Prototype_p, void *SourceText_p, nh_ecmascript_ParseNode *ParameterList_p, 
    nh_ecmascript_ParseNode *Body_p, NH_ECMASCRIPT_THIS_MODE thisMode, nh_ecmascript_Environment *Scope_p)
{
    nh_ecmascript_Object *Function_p = nh_ecmascript_ordinaryObjectCreate(Prototype_p, lookup_p, 16);
    NH_CORE_CHECK_NULL_2(NULL, Function_p)

//    Function_p->InternalMethods_p = &InternalMethods;
//
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_SOURCE_TEXT, NULL);
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_FORMAL_PARAMETERS, ParameterList_p);
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_ECMASCRIPT_CODE, Body_p);
//
//    // TODO If the source text matching Body is strict mode code, let Strict be true; else let Strict be false.
//    bool strict = false;
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_STRICT, (void*)strict);
//
//    if (thisMode == NH_ECMASCRIPT_THIS_MODE_LEXICAL) {
//        nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_THIS_MODE, (void*)NH_ECMASCRIPT_THIS_MODE_LEXICAL);
//    } else if (strict) {
//        nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_THIS_MODE, (void*)NH_ECMASCRIPT_THIS_MODE_STRICT);
//    } else {
//        nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_THIS_MODE, (void*)NH_ECMASCRIPT_THIS_MODE_GLOBAL);
//    }
//
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_IS_CLASS_CONSTRUCTOR, (void*)false);
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_ENVIRONMENT, Scope_p);
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_SCRIPT_OR_MODULE, nh_ecmascript_getActiveScriptOrModule());
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_REALM, nh_ecmascript_getRunningExecutionContext()->Realm_p);
//    nh_ecmascript_setInternalSlot(&Function_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_HOME_OBJECT, NULL);
//
//    int len = nh_ecmascript_getExpectedArgumentCount(ParameterList_p);
//    nh_ecmascript_setFunctionLength(Function_p, len);

    return Function_p;
}

// https://tc39.es/ecma262/#sec-makeconstructor
nh_ecmascript_Completion nh_ecmascript_makeConstructor(
    nh_ecmascript_Object *Function_p, bool writablePrototype, nh_ecmascript_Object *Prototype_p)
{
    // TODO

    return nh_ecmascript_normalEmptyCompletion();
}

// https://tc39.es/ecma262/#sec-setfunctionname
nh_ecmascript_Completion nh_ecmascript_setFunctionName(
    nh_ecmascript_Object *Function_p, nh_ecmascript_Any Name, char *prefix_p)
{
    char *propertyName_p = "name";

    nh_ecmascript_String PropertyName;
    PropertyName.p = propertyName_p;
    PropertyName.length  = strlen(propertyName_p);

    if (Name.type == NH_ECMASCRIPT_TYPE_SYMBOL) {
        // TODO
    }

//    if (Function_p->InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_INITIAL_NAME] >= 0) {
//        // TODO 
//    }
//
//    if (prefix_p != NULL) {
//        // TODO
//    }
//
    nh_ecmascript_PropertyDescriptor Descriptor;
    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
    Descriptor.enumerable   = false;
    Descriptor.configurable = true;
    Descriptor.Fields.Data.writable = false;
    Descriptor.Fields.Data.Value    = Name; 

    return nh_ecmascript_abstractDefinePropertyOrThrow(Function_p, nh_ecmascript_wrapString(&PropertyName), Descriptor);
}

// https://tc39.es/ecma262/#sec-setfunctionlength
nh_ecmascript_Completion nh_ecmascript_setFunctionLength(
    nh_ecmascript_Object *Function_p, NH_ECMASCRIPT_NUMBER len)
{
    char *propertyName_p = "length";

    nh_ecmascript_String PropertyName;
    PropertyName.p = propertyName_p;
    PropertyName.length  = strlen(propertyName_p);

    nh_ecmascript_PropertyDescriptor Descriptor;
    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
    Descriptor.enumerable   = false;
    Descriptor.configurable = false;
    Descriptor.Fields.Data.writable = false;
    Descriptor.Fields.Data.Value    = nh_ecmascript_wrapNumber(&len); 

    return nh_ecmascript_abstractDefinePropertyOrThrow(Function_p, nh_ecmascript_wrapString(&PropertyName), Descriptor);
}

