// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Debug.h"
#include "../Header/Macros.h"

#include "../../Core/Header/String.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Config.h"
#include "../../Core/Header/Output.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DECLARE =========================================================================================

static int Nh_JS_getScriptNameOffset(
    Nh_JS_Script *Script_p
);
static NH_RESULT Nh_JS_prependDebugInfo(
    Nh_JS_Script *Script_p, Nh_String *String_p
);

// DEBUG ===========================================================================================

void Nh_JS_debugEventListener( // TODO multithreading
    Nh_JS_Script *Script_p, Nh_JS_Object *Listener_p, int action)
{
NH_BEGIN()

#define ADD     0
#define TRIGGER 1

    if (Nh_getConfig()->Flags.Log.listener && Listener_p->type == NH_JS_OBJECT_EVENT_LISTENER)
    {
        Nh_String *String_p = Nh_allocateString(NULL);
        if (Nh_JS_prependDebugInfo(Script_p, String_p) != NH_SUCCESS) {NH_SILENT_END()}
        if (Nh_appendToString(String_p, "LSTN ") != NH_SUCCESS) {NH_SILENT_END()}

             if (action == ADD)     {if (Nh_appendToString(String_p, " ADD ") != NH_SUCCESS) {NH_SILENT_END()}}
        else if (action == TRIGGER) {if (Nh_appendToString(String_p, "TRGR ") != NH_SUCCESS) {NH_SILENT_END()}}

        char *main_p = Nh_JS_stringifyObject(Listener_p, true, true);
        if (main_p == NULL) {NH_SILENT_END()}

        if (Nh_appendToString(String_p, main_p) != NH_SUCCESS) {NH_SILENT_END()}
        Nh_free(main_p);

        char *chars_p = Nh_getChars(String_p);
        Nh_freeString(String_p, false);

        if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
            Nh_free(chars_p);
        }
    }

#undef ADD
#undef TRIGGER

NH_SILENT_END()
}

void Nh_JS_debugEvent( // TODO multithreading
    Nh_JS_Script *Script_p, Nh_JS_Event *Event_p)
{
NH_BEGIN()

    if (Nh_getConfig()->Flags.Log.event)
    {
        Nh_String *String_p = Nh_allocateString(NULL);
        if (Nh_JS_prependDebugInfo(Script_p, String_p) != NH_SUCCESS) {NH_SILENT_END()}
        if (Nh_appendToString(String_p, "EVNT SEND ") != NH_SUCCESS) {NH_SILENT_END()}

        char *main_p = Nh_JS_stringifyEventData(Event_p, true);
        if (main_p == NULL) {NH_SILENT_END()}

        if (Nh_appendToString(String_p, main_p) != NH_SUCCESS) {NH_SILENT_END()}
        Nh_free(main_p);

        char *chars_p = Nh_getChars(String_p);
        Nh_freeString(String_p, false);

        if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
            Nh_free(chars_p);
        }
    }

NH_SILENT_END()
}

void Nh_JS_debugInterpreter( // TODO multithreading
    Nh_JS_Script *Script_p, const char *func_p, void *data_p, int action)
{
NH_BEGIN()

#define BEGIN 0
#define END   1

    if (Nh_getConfig()->Flags.Log.jsinterpreter)
    {
        if (action == BEGIN) 
        {
            Nh_String *String_p = Nh_allocateString(NULL);
            if (Nh_JS_prependDebugInfo(Script_p, String_p) != NH_SUCCESS) {NH_SILENT_END()}
            if (Nh_appendToString(String_p, "  JS ENTR ") != NH_SUCCESS) {NH_SILENT_END()}

            for (int indent = 0; indent < Script_p->Debug.depth; ++indent) {if (Nh_appendToString(String_p, "  ") != NH_SUCCESS) {NH_SILENT_END()}}
            if (Nh_appendFormatToString(String_p, "%s\n", func_p) != NH_SUCCESS) {NH_SILENT_END()}

            char *chars_p = Nh_getChars(String_p);
            Nh_freeString(String_p, false);
    
            if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
                Nh_free(chars_p);
            }

            Script_p->Debug.depth += 1;
        }
        else if (action == END) {
            Script_p->Debug.depth -= 1;
        }
    }

#undef BEGIN
#undef END

NH_SILENT_END()
}

void Nh_JS_debugFunction( // TODO multithreading
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p,
    const char *func_p, int action)
{
NH_BEGIN()

#define BEGIN 0
#define END   1

    if (Nh_getConfig()->Flags.Log.jsinterpreter)
    {
        if (action == BEGIN) 
        {
            Nh_String *String_p = Nh_allocateString(NULL);
            if (Nh_JS_prependDebugInfo(Script_p, String_p) != NH_SUCCESS) {NH_SILENT_END()}
            if (Nh_appendToString(String_p, "  JS EXEC ") != NH_SUCCESS) {NH_SILENT_END()}

            for (int indent = 0; indent < Script_p->Debug.depth; ++indent) {if (Nh_appendToString(String_p, "  ") != NH_SUCCESS) {NH_SILENT_END()}}
            if (Nh_appendFormatToString(String_p, "\e[1;33m%s\e[0m\n", func_p) != NH_SUCCESS) {NH_SILENT_END()}

            char *chars_p = Nh_getChars(String_p);
            Nh_freeString(String_p, false);
    
            if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
                Nh_free(chars_p);
            }

            Script_p->Debug.depth += 1;
        }
        else if (action == END) {
            Script_p->Debug.depth -= 1;
        }
    }

#undef BEGIN
#undef END

NH_SILENT_END()
}

void Nh_JS_debugScript( // TODO multithreading
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    if (Nh_getConfig()->Flags.Log.script)
    {
        Nh_String *String_p = Nh_allocateString(NULL);

        char *script_p = Nh_JS_stringifyScript(Script_p);
        if (script_p == NULL) {NH_SILENT_END()}

        if (Nh_appendFormatToString(String_p, "%s\n", script_p) != NH_SUCCESS) {NH_SILENT_END()}
        Nh_free(script_p);

        char *chars_p = Nh_getChars(String_p);
        Nh_freeString(String_p, false);
 
        if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
            Nh_free(chars_p);
        }
    }

NH_SILENT_END()
}

// HELPER ==========================================================================================

static int Nh_JS_getScriptNameOffset(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

//    int offset = Script_p->Info.path_p == NULL ? 6 : strlen(Script_p->Info.path_p);
//    int max = 0;
//
//    for (int i = 0; i < Script_p->Run.Tab_p->Document.Scripts.count; ++i) {
//        Nh_JS_Script *Compare_p = Nh_getListItem(&Script_p->Run.Tab_p->Document.Scripts, i);
//        if (Compare_p->Info.path_p != NULL && strlen(Compare_p->Info.path_p) > max) {
//            max = strlen(Compare_p->Info.path_p);
//        }        
//    }

//    offset = max - offset; 

    int offset = 0;

NH_END(offset)
}

static NH_RESULT Nh_JS_prependDebugInfo(
    Nh_JS_Script *Script_p, Nh_String *String_p)
{
NH_BEGIN()

    int nameOffset = Nh_JS_getScriptNameOffset(Script_p);
    char *info_p = Script_p->Flags.internalScript ? "INTL" : "EXTL";

    NH_CHECK(Nh_appendFormatToString(String_p, "%s ", info_p))
    for (int i = 0; i < nameOffset; ++i) {
        NH_CHECK(Nh_appendToString(String_p, " "))
    }

NH_END(NH_SUCCESS)
}

