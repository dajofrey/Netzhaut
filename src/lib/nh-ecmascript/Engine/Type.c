// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Type.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/String.h"

#include <string.h>

// FUNCTIONS =======================================================================================

nh_ecmascript_Any nh_ecmascript_initAny(
    NH_ECMASCRIPT_TYPE type, void *handle_p)
{
    nh_ecmascript_Any Any;
    Any.type = type;
    Any.handle_p = handle_p;

    return Any;
}

nh_ecmascript_Any nh_ecmascript_wrapUndefined()
{
    return nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL);
}

nh_ecmascript_Any nh_ecmascript_wrapNull()
{
    return nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_NULL, NULL);
}

nh_ecmascript_Any nh_ecmascript_wrapBoolean(
    NH_ECMASCRIPT_BOOLEAN boolean)
{
    return nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_BOOLEAN, (void*)boolean);
}

nh_ecmascript_Any nh_ecmascript_wrapString(
    nh_ecmascript_String *String_p)
{
    return nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_STRING, String_p);
}

nh_ecmascript_Any nh_ecmascript_wrapObject(
    nh_ecmascript_Object *Object_p)
{
    return nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_OBJECT, Object_p);
}

nh_ecmascript_String *nh_ecmascript_allocateString(
    size_t chunkSize)
{
    nh_ecmascript_String *String_p = nh_core_allocate(sizeof(nh_ecmascript_String));
    NH_CORE_CHECK_MEM_2(NULL, String_p)

    *String_p = nh_core_initString(chunkSize);

    return String_p;
}

NH_API_RESULT nh_ecmascript_appendFormatToString(
    nh_ecmascript_String *String_p, char *p, size_t len)
{
    NH_API_RESULT result = nh_core_appendFormatToString(String_p, p, len);
    return NH_API_SUCCESS;
}

nh_ecmascript_Any nh_ecmascript_wrapNumber(
    NH_ECMASCRIPT_NUMBER *number_p)
{
    return nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_NUMBER, number_p);
}

NH_ECMASCRIPT_NUMBER *nh_ecmascript_allocateNumber(
    NH_ECMASCRIPT_NUMBER number)
{
    NH_ECMASCRIPT_NUMBER *number_p = nh_core_allocate(sizeof(NH_ECMASCRIPT_NUMBER));
    NH_CORE_CHECK_MEM_2(NULL, number_p)

    *number_p = number;

    return number_p;
}

