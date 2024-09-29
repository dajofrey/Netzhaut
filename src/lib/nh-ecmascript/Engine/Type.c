// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Type.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/String.h"

#include <string.h>

// MISC ============================================================================================

nh_ecmascript_Any nh_ecmascript_initAny(
    NH_ECMASCRIPT_TYPE type, void *handle_p)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_Any Any;
    Any.type = type;
    Any.handle_p = handle_p;

NH_ECMASCRIPT_END(Any)
}

nh_ecmascript_Any nh_ecmascript_wrapUndefined()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
}

nh_ecmascript_Any nh_ecmascript_wrapNull()
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_NULL, NULL))
}

nh_ecmascript_Any nh_ecmascript_wrapBoolean(
    NH_ECMASCRIPT_BOOLEAN boolean)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_BOOLEAN, (void*)boolean))
}

nh_ecmascript_Any nh_ecmascript_wrapString(
    nh_ecmascript_String *String_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_STRING, String_p))
}

nh_ecmascript_Any nh_ecmascript_wrapObject(
    nh_ecmascript_Object *Object_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_OBJECT, Object_p))
}

nh_ecmascript_String *nh_ecmascript_allocateString(
    size_t chunkSize)
{
NH_ECMASCRIPT_BEGIN()

    nh_ecmascript_String *String_p = nh_core_allocate(sizeof(nh_ecmascript_String));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, String_p)

    *String_p = nh_core_initString(chunkSize);

NH_ECMASCRIPT_END(String_p)
}

NH_API_RESULT nh_ecmascript_appendFormatToString(
    nh_ecmascript_String *String_p, char *p, size_t len)
{
NH_ECMASCRIPT_BEGIN()

    NH_API_RESULT result = nh_core_appendFormatToString(String_p, p, len);

NH_ECMASCRIPT_END(NH_API_SUCCESS)
}

nh_ecmascript_Any nh_ecmascript_wrapNumber(
    NH_ECMASCRIPT_NUMBER *number_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(nh_ecmascript_initAny(NH_ECMASCRIPT_TYPE_NUMBER, number_p))
}

NH_ECMASCRIPT_NUMBER *nh_ecmascript_allocateNumber(
    NH_ECMASCRIPT_NUMBER number)
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_NUMBER *number_p = nh_core_allocate(sizeof(NH_ECMASCRIPT_NUMBER));
    NH_ECMASCRIPT_CHECK_MEM_2(NULL, number_p)

    *number_p = number;

NH_ECMASCRIPT_END(number_p)
}

