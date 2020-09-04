// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Object.h"
#include "../Header/Function.h"
#include "../Header/Macros.h"

#include "../../Header/Data.h"
#include "../../Header/Interpreter.h"

#include "../../DOM/Header/Document.h"
#include "../../DOM/Header/HTMLElement.h"
#include "../../DOM/Header/EventListener.h"

#include "../../../Core/Header/Memory.h"
#include "../../../Core/Header/HashMap.h"
#include "../../../Core/Header/String.h"

#include <ctype.h>

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// DECLARE =========================================================================================

static NH_RESULT Nh_JS_createObjectRecursively(
    Nh_JS_Script *Script_p, NH_JS_OBJECT type, Nh_JS_Object *Object_p, Nh_JS_Object *Inherit_p, 
    Nh_JS_Object *Inheriting_p, int index
);
static NH_RESULT Nh_JS_createParentObject(
    Nh_JS_Script *Script_p, NH_JS_OBJECT type, Nh_JS_Object *Object_p
);

static void Nh_JS_initObject(
    Nh_JS_Object *Function_p, Nh_JS_Object *Inheriting_p, NH_JS_OBJECT type
);
static int Nh_JS_getFunctionIndex(
    Nh_HashMaps *Maps_p, NH_JS_OBJECT type, char *name_p
);

// DATA ============================================================================================

const char *types_pp[] =
{
    "EventTarget",
    "Node",
    "Element",
    "HTMLElement",
    "Location",
    "Document",
    "Console",
    "Window",
    "NhWindow",
    "NhTab",
    "HTMLCollection",
    "Event",
    "CanvasRenderingContext2D",
    "HTMLCanvasElement",
    "Path2D",
    "Math",
    "UIEvent",
    "MouseEvent",
    "WheelEvent",
    "EventListener",
    "Function",
    "Object",
};

// API =============================================================================================

Nh_JS_Result Nh_JS_Object_Object(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_assign(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_create(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_defineProperty(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_defineProperties(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_entries(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_freeze(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_fromEntries(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_getOwnPropertyDescriptor(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_getOwnPropertyDescriptors(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_getOwnPropertyNames(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_getOwnPropertySymbols(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_getPrototypeOf(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_is(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_isExtensible(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_isFrozen(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_isSealed(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_keys(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_preventExtensions(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_seal(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_setPrototypeOf(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Object_values(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

// INTERNAL ========================================================================================

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG

NH_RESULT Nh_JS_createObject(
    Nh_JS_Script *Script_p, NH_JS_OBJECT type, Nh_JS_Object **Object_pp)
{
NH_BEGIN()

    *Object_pp = Nh_allocate(sizeof(Nh_JS_Object));
    NH_CHECK_MEM(Object_pp)

    NH_CHECK(Nh_JS_createObjectRecursively(Script_p, type, *Object_pp, NULL, NULL, -1))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_createObjectRecursively(
    Nh_JS_Script *Script_p, NH_JS_OBJECT type, Nh_JS_Object *Object_p, Nh_JS_Object *Inherit_p, 
    Nh_JS_Object *Inheriting_p, int index)
{
NH_BEGIN()

    NH_CHECK_NULL(Object_p)
    Nh_JS_initObject(Object_p, Inheriting_p, type);

    Nh_JS_getFunctionNames(type, &Object_p->functionCount);

    if (type == NH_JS_OBJECT_FUNCTION) 
    {
        Object_p->data_p = Nh_allocate(sizeof(Nh_JS_Function));
        NH_CHECK_MEM(Object_p->data_p)
        NH_CHECK(Nh_JS_createFunctionData(Object_p->data_p, Inherit_p, index))

        Object_p->Inherit_p = Inherit_p;

        if (Inherit_p->type != NH_JS_OBJECT_FUNCTION)
        {
            Object_p->Functions_p = Nh_allocate(sizeof(Nh_JS_Object) * Object_p->functionCount);
            NH_CHECK_MEM(Object_p->Functions_p)
            for (size_t i = 0; i < Object_p->functionCount; ++i) {
                NH_CHECK(Nh_JS_createObjectRecursively(Script_p, NH_JS_OBJECT_FUNCTION, &Object_p->Functions_p[i], Object_p, NULL, i))
            }
        } else {Object_p->functionCount = 0;}
    }
    else 
    {
        Object_p->Functions_p = Nh_allocate(sizeof(Nh_JS_Object) * Object_p->functionCount);
        NH_CHECK_MEM(Object_p->Functions_p)
        for (size_t i = 0; i < Object_p->functionCount; ++i) {
            NH_CHECK(Nh_JS_createObjectRecursively(Script_p, NH_JS_OBJECT_FUNCTION, &Object_p->Functions_p[i], Object_p, NULL, i))
        }

        if (type != NH_JS_OBJECT_OBJECT) {
            Object_p->Inherit_p = Nh_allocate(sizeof(Nh_JS_Object));
            NH_CHECK_MEM(Object_p->Inherit_p);
        }

        NH_CHECK(Nh_JS_createParentObject(Script_p, type, Object_p))
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_createParentObject(
    Nh_JS_Script *Script_p, NH_JS_OBJECT type, Nh_JS_Object *Object_p)
{
NH_BEGIN()

#define CREATE_PARENT_OBJECT(t) NH_CHECK(Nh_JS_createObjectRecursively(Script_p, t, Object_p->Inherit_p, NULL, Object_p, -1))

    switch (type)
    {
        case NH_JS_OBJECT_EVENT_TARGET                : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_NODE                        : CREATE_PARENT_OBJECT(NH_JS_OBJECT_EVENT_TARGET) break;
        case NH_JS_OBJECT_ELEMENT                     : CREATE_PARENT_OBJECT(NH_JS_OBJECT_NODE) break;
        case NH_JS_OBJECT_HTML_ELEMENT                : CREATE_PARENT_OBJECT(NH_JS_OBJECT_ELEMENT) break;
        case NH_JS_OBJECT_LOCATION                    : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_DOCUMENT                    : CREATE_PARENT_OBJECT(NH_JS_OBJECT_NODE) break;
        case NH_JS_OBJECT_CONSOLE                     : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_WINDOW                      : CREATE_PARENT_OBJECT(NH_JS_OBJECT_EVENT_TARGET) break;
        case NH_JS_OBJECT_NH_WINDOW                   : CREATE_PARENT_OBJECT(NH_JS_OBJECT_EVENT_TARGET) break;
        case NH_JS_OBJECT_NH_TAB                      : CREATE_PARENT_OBJECT(NH_JS_OBJECT_EVENT_TARGET) break; 
        case NH_JS_OBJECT_HTML_COLLECTION             : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_EVENT                       : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_CANVAS_RENDERING_CONTEXT_2D : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_HTML_CANVAS_ELEMENT         : CREATE_PARENT_OBJECT(NH_JS_OBJECT_HTML_ELEMENT) break;
        case NH_JS_OBJECT_PATH_2D                     : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_MATH                        : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_UI_EVENT                    : CREATE_PARENT_OBJECT(NH_JS_OBJECT_EVENT) break;
        case NH_JS_OBJECT_MOUSE_EVENT                 : CREATE_PARENT_OBJECT(NH_JS_OBJECT_UI_EVENT) break;
        case NH_JS_OBJECT_EVENT_LISTENER              : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        case NH_JS_OBJECT_WHEEL_EVENT                 : CREATE_PARENT_OBJECT(NH_JS_OBJECT_MOUSE_EVENT) break;
        case NH_JS_OBJECT_STRING                      : CREATE_PARENT_OBJECT(NH_JS_OBJECT_OBJECT) break;
        default : break;
    }

NH_END(NH_SUCCESS)
}

void Nh_JS_destroyObject(
    Nh_JS_Object *Object_p)
{
NH_BEGIN()

    for (int i = 0; i < Object_p->functionCount; ++i) {
        Nh_free(Object_p->Functions_p[i].data_p);
        Nh_JS_destroyObject(&Object_p->Functions_p[i]); 
    }
    Nh_free(Object_p->Functions_p);

    if (Object_p->Inherit_p != NULL && Object_p->type != NH_JS_OBJECT_FUNCTION) {
        Nh_JS_destroyObject(Object_p->Inherit_p);
    }
    if (Object_p->type != NH_JS_OBJECT_FUNCTION) {
        Nh_free(Object_p);
    }

NH_SILENT_END()
}

Nh_JS_Object *Nh_JS_getInheritedObject(
    Nh_JS_Object *Object_p, NH_JS_OBJECT type)
{
NH_BEGIN()

    Nh_JS_Object *Inherit_p = Object_p;
    while (Inherit_p != NULL && Inherit_p->type != type) {Inherit_p = Inherit_p->Inherit_p;}

NH_END(Inherit_p)
}

Nh_JS_Object *Nh_JS_getObject(
    Nh_JS_Object *Object_p, NH_JS_OBJECT type)
{
NH_BEGIN()

    if (Object_p->type == type) {NH_END(Object_p)}

    Nh_JS_Object *Result_p = Nh_JS_getInheritedObject(Object_p, type);
    if (Result_p != NULL) {NH_END(Result_p)}

    Result_p = Nh_JS_getInheritingObject(Object_p, type);

NH_END(Result_p)
}

Nh_JS_Object *Nh_JS_getInheritingObject(
    Nh_JS_Object *Object_p, NH_JS_OBJECT type)
{
NH_BEGIN()

    Nh_JS_Object *Inheriting_p = Object_p;
    while (Inheriting_p != NULL && Inheriting_p->type != type) {Inheriting_p = Inheriting_p->Inheriting_p;}

NH_END(Inheriting_p)
}

#include NH_CUSTOM_CHECK

char *Nh_JS_stringifyObject(
    Nh_JS_Object *Object_p, bool newline, bool extended)
{
NH_BEGIN()

    Nh_String *String_p = Nh_allocateString(NULL);

    NH_CHECK(NULL, Nh_appendFormatToString(String_p, 
        "%p %s", Object_p, types_pp[Object_p->type]
    ))

    if (extended) 
    {
        char *ext_p = NULL;
        switch (Object_p->type)
        {
            case NH_JS_OBJECT_HTML_ELEMENT :
                ext_p = Nh_JS_stringifyHTMLElementData(Object_p, false);
                break;
            case NH_JS_OBJECT_EVENT_LISTENER :
                ext_p = Nh_JS_stringifyEventListenerData(Object_p, false);
                break;
        }
        if (ext_p != NULL) {
            NH_CHECK(NULL, Nh_appendFormatToString(String_p, " %s", ext_p))
            Nh_free(ext_p);
        }
    }

    if (newline) {NH_CHECK(NULL, Nh_appendToString(String_p, "\n"))}

    char *chars_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);

NH_END(chars_p)
}

static void Nh_JS_initObject(
    Nh_JS_Object *Object_p, Nh_JS_Object *Inheriting_p, NH_JS_OBJECT type)
{
NH_BEGIN()

    Object_p->type          = type;
    Object_p->functionCount = 0;
    Object_p->data_p        = NULL;
    Object_p->Functions_p   = NULL;
    Object_p->Inherit_p     = NULL;
    Object_p->Inheriting_p  = Inheriting_p;

NH_SILENT_END()
}

