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

#include "../Common/Log.h"
#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdio.h>

// REFERENCE =======================================================================================

nh_ecmascript_Reference nh_ecmascript_initReference()
{
    nh_ecmascript_Reference Reference;
    Reference.Base.unresolvable = true;
    Reference.ReferencedName = nh_ecmascript_wrapUndefined();
    Reference.strict = false;
    Reference.ThisValue.empty = true;

    return Reference;
}

nh_ecmascript_Reference nh_ecmascript_getIdentifierReference(
    nh_ecmascript_Environment *Environment_p, nh_encoding_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN strict)
{
    nh_ecmascript_Reference Reference;

    if (Environment_p == NULL) {
        Reference.Base.unresolvable = true;
        Reference.ReferencedName = nh_ecmascript_wrapString(Name_p);
        Reference.strict = strict;
        Reference.ThisValue.empty = true;
        return Reference;
    }

    bool exists = nh_ecmascript_hasBinding(Environment_p, Name_p);

    if (exists) {
        Reference.Base.unresolvable = false;
        Reference.Base.Environment_p = Environment_p;
        Reference.ReferencedName = nh_ecmascript_wrapString(Name_p);
        Reference.strict = strict;
        Reference.ThisValue.empty = true;
        return Reference;
    }

    nh_ecmascript_Environment *OuterEnvironment_p = Environment_p->Outer_p;

    return nh_ecmascript_getIdentifierReference(OuterEnvironment_p, Name_p, strict);
}

NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isPropertyReference(
    nh_ecmascript_Reference Reference)
{
    if (Reference.Base.unresolvable) {return false;}
    if (Reference.Base.Environment_p != NULL) {return false;}

    return true;
}

nh_ecmascript_Completion nh_ecmascript_getValue(
    nh_ecmascript_Completion Completion, nh_ecmascript_Reference Reference)
{
    if (!Completion.Value.empty) {return Completion;}
    if (Reference.Base.unresolvable) {return nh_ecmascript_throwReferenceError();}

    if (nh_ecmascript_isPropertyReference(Reference)) {

    }
    else {

    }

    return nh_ecmascript_normalEmptyCompletion();
}

nh_ecmascript_Completion nh_ecmascript_putValue(
    nh_ecmascript_Reference Reference, nh_ecmascript_Any Value)
{
    if (Reference.Base.unresolvable) {
        if (Reference.strict) {return nh_ecmascript_throwReferenceError();}
        nh_ecmascript_Object *GlobalObject_p = nh_ecmascript_getGlobalObject();
        return nh_ecmascript_abstractSet(GlobalObject_p, Reference.ReferencedName, Value, false);
    }
    else if (nh_ecmascript_isPropertyReference(Reference)) {
    }

    return nh_ecmascript_normalEmptyCompletion();
}
