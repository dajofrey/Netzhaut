// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Reference.h"
#include "Lexer.h"
#include "Parser.h"
#include "ExecutionContext.h"
#include "Instantiation.h"

#include "../../nhcore/System/Memory.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include <string.h>
#include <stdio.h>

// REFERENCE =======================================================================================

nh_ecmascript_Reference nh_ecmascript_initReference()
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Reference Reference;
    Reference.Base.unresolvable = NH_TRUE;
    Reference.ReferencedName = nh_ecmascript_wrapUndefined();
    Reference.strict = NH_FALSE;
    Reference.ThisValue.empty = NH_TRUE;

NH_ECMASCRIPT_END(Reference)
}

nh_ecmascript_Reference nh_ecmascript_getIdentifierReference(
    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN strict)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Reference Reference;

    if (Environment_p == NULL) {
        Reference.Base.unresolvable = NH_TRUE;
        Reference.ReferencedName = nh_ecmascript_wrapString(Name_p);
        Reference.strict = strict;
        Reference.ThisValue.empty = NH_TRUE;
        NH_ECMASCRIPT_END(Reference)
    }

    NH_BOOL exists = nh_ecmascript_hasBinding(Environment_p, Name_p);

    if (exists) {
        Reference.Base.unresolvable = NH_FALSE;
        Reference.Base.Environment_p = Environment_p;
        Reference.ReferencedName = nh_ecmascript_wrapString(Name_p);
        Reference.strict = strict;
        Reference.ThisValue.empty = NH_TRUE;
        NH_ECMASCRIPT_END(Reference)
    }

    nh_ecmascript_Environment *OuterEnvironment_p = Environment_p->Outer_p;

NH_ECMASCRIPT_END(nh_ecmascript_getIdentifierReference(OuterEnvironment_p, Name_p, strict))
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isPropertyReference(
    nh_ecmascript_Reference Reference)
{
NH_ECMASCRIPT_BEGIN()

    if (Reference.Base.unresolvable) {NH_ECMASCRIPT_END(NH_FALSE)}
    if (Reference.Base.Environment_p != NULL) {NH_ECMASCRIPT_END(NH_FALSE)}

NH_ECMASCRIPT_END(NH_TRUE)
}

nh_ecmascript_Completion nh_ecmascript_getValue(
    nh_ecmascript_Completion Completion, nh_ecmascript_Reference Reference)
{
NH_ECMASCRIPT_BEGIN()

    if (!Completion.Value.empty) {NH_ECMASCRIPT_END(Completion)}
    if (Reference.Base.unresolvable) {NH_ECMASCRIPT_END(nh_ecmascript_throwReferenceError())}

    if (nh_ecmascript_isPropertyReference(Reference)) {

    }
    else {

    }

NH_ECMASCRIPT_END(nh_ecmascript_normalEmptyCompletion())
}

nh_ecmascript_Completion nh_ecmascript_putValue(
    nh_ecmascript_Reference Reference, nh_ecmascript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    if (Reference.Base.unresolvable) {
        if (Reference.strict) {NH_ECMASCRIPT_END(nh_ecmascript_throwReferenceError())}
        nh_ecmascript_Object *GlobalObject_p = nh_ecmascript_getGlobalObject();
        NH_ECMASCRIPT_END(nh_ecmascript_abstractSet(GlobalObject_p, Reference.ReferencedName, Value, NH_FALSE))
    }
    else if (nh_ecmascript_isPropertyReference(Reference)) {
    }

NH_ECMASCRIPT_END(nh_ecmascript_normalEmptyCompletion())
}

