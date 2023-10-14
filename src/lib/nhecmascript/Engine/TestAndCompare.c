// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "TestAndCompare.h"

#include "../Common/Macros.h"

// ABSTRACT ========================================================================================

// https://tc39.es/ecma262/#sec-isextensible-o
nh_ecmascript_Completion nh_ecmascript_isExtensible(
    nh_ecmascript_Object *Object_p)
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_BOOLEAN boolean = Object_p->InternalMethods_p->isExtensible_f(Object_p);
    nh_ecmascript_Completion Result = nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(boolean));

NH_ECMASCRIPT_END(Result)
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isCallable(
    nh_ecmascript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    if (Value.type != NH_ECMASCRIPT_TYPE_OBJECT) {NH_ECMASCRIPT_END(NH_FALSE)}

    if (Value.handle_p != NULL && ((nh_ecmascript_Object*)Value.handle_p)->InternalMethods_p->call_f != NULL) {
        NH_ECMASCRIPT_END(NH_TRUE)
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

