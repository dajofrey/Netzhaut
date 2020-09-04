#ifndef NH_JS_SCRIPT_H
#define NH_JS_SCRIPT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"

#include "../Core/Header/Object.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/System.h"
#include "../../Core/Header/List.h"
#include "../../Core/Header/URI.h"

#include <stdbool.h>

typedef struct NH_JS_Document NH_JS_Document;
typedef struct Nh_HTML_Document Nh_HTML_Document;
typedef struct Nh_Tab Nh_Tab;

#endif

/** @addtogroup JSStructs Structs
 *  \ingroup JS
 *  @{
 */

    typedef struct Nh_JS_Script {

        Nh_URI URI;

        struct {
            Nh_Tab *Tab_p;
            bool callPending;
            bool unresolvedArguments;
            int interpretingLeftSide;
        } Run;

        struct {
            struct {
                Nh_JS_Object *Math_p;
            } Objects;
        } Core;

        struct {
            struct {
                Nh_JS_Object *Window_p;
                Nh_JS_Object *Location_p;
                Nh_JS_Object *Document_p;
                Nh_JS_Object *Console_p;
                Nh_JS_Object *NhWindow_p;
                Nh_JS_Object *NhTab_p;
                Nh_JS_Object *Event_p;
            } Objects;
            Nh_List EventListeners;
        } DOM;

        struct {
            bool interpreted;
            bool loaded;
            bool parsed;
            bool internalScript;
            bool external;
        } Flags;

        struct {
            int depth;
        } Debug;

        Nh_JS_BlockStatement Container;

    } Nh_JS_Script;

/** @} */

/** @addtogroup JSFunctions Functions
 *  \ingroup JS
 *  @{
 */

    char *Nh_JS_getDefaultScript(
        NH_INTERNAL_FILE file
    );

    void Nh_JS_initScript(
        Nh_JS_Script *Script_p
    );

    NH_RESULT Nh_JS_addScripts(
        Nh_HTML_Document *Document_p, bool addDefaultScripts
    );

    Nh_JS_Script *Nh_JS_getScript(
        Nh_List *Scripts_p, int index
    );

    NH_JS_Document *Nh_JS_getDocument(
        Nh_JS_Script *Script_p
    );
   
    void Nh_JS_destroyScriptList(
        Nh_List *Scripts_p
    );

    void Nh_JS_destroyScript(
        Nh_JS_Script *Script_p
    );

    Nh_JS_Parsable *Nh_JS_getFunctionDeclaration(
        Nh_JS_Script *Script_p, char *name_p
    );

    char *Nh_JS_stringifyScript(
        Nh_JS_Script *Script_p
    );

/** @} */

#endif
