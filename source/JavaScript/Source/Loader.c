// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Loader.h"
#include "../Header/Macros.h"
#include "../Header/Variable.h"

#include "../DOM/Header/EventListener.h"
#include "../DOM/Header/Document.h"
#include "../Core/Header/Object.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_JS_UTILS

#include <string.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_JS_loadCoreObjects(
    Nh_JS_Script *Script_p, Nh_JS_Script *Copy_p
);
static NH_RESULT Nh_JS_loadDOMObjects(
    Nh_JS_Script *Script_p, Nh_JS_Script *Copy_p
);

// LOAD ============================================================================================

NH_RESULT Nh_JS_loadScript(
    Nh_Tab *Tab_p, Nh_JS_Script *Script_p)
{
NH_BEGIN()

    if (Script_p->Flags.interpreted || Script_p->Flags.loaded) {NH_END(NH_ERROR_BAD_STATE)}

    Script_p->Run.Tab_p = Tab_p;

    Nh_JS_Script *Copy_p = NULL;

    for (int i = 0; i < Tab_p->Document.Scripts.count; ++i) { 
        Nh_JS_Script *Other_p = Nh_getListItem(&Tab_p->Document.Scripts, i);
        if (Other_p->Flags.loaded) {Copy_p = Other_p; break;}
    }

    NH_CHECK(Nh_JS_loadCoreObjects(Script_p, Copy_p))
    NH_CHECK(Nh_JS_loadDOMObjects(Script_p, Copy_p))

    Script_p->Flags.loaded = true;

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_loadCoreObjects(
    Nh_JS_Script *Script_p, Nh_JS_Script *Copy_p)
{
NH_BEGIN()

    if (Copy_p == NULL) {
        NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_MATH, &Script_p->Core.Objects.Math_p))
    }
    else {
        Script_p->Core.Objects.Math_p = Copy_p->Core.Objects.Math_p;
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_loadDOMObjects(
    Nh_JS_Script *Script_p, Nh_JS_Script *Copy_p)
{
NH_BEGIN()

    if (Copy_p == NULL) {
        NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_WINDOW, &Script_p->DOM.Objects.Window_p))
        NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_LOCATION, &Script_p->DOM.Objects.Location_p))
        NH_CHECK(Nh_JS_createDocumentObject(Script_p, &Script_p->DOM.Objects.Document_p))
        NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_CONSOLE, &Script_p->DOM.Objects.Console_p))
        NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_NH_WINDOW, &Script_p->DOM.Objects.NhWindow_p))
        NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_NH_TAB, &Script_p->DOM.Objects.NhTab_p))
        NH_CHECK(Nh_JS_createEventObject(Script_p, &Script_p->DOM.Objects.Event_p, NULL))
    }
    else {
        Script_p->DOM.Objects.Window_p   = Copy_p->DOM.Objects.Window_p;
        Script_p->DOM.Objects.Location_p = Copy_p->DOM.Objects.Location_p;
        Script_p->DOM.Objects.Document_p = Copy_p->DOM.Objects.Document_p;
        Script_p->DOM.Objects.Console_p  = Copy_p->DOM.Objects.Console_p;
        Script_p->DOM.Objects.NhWindow_p = Copy_p->DOM.Objects.NhWindow_p;
        Script_p->DOM.Objects.NhTab_p    = Copy_p->DOM.Objects.NhTab_p;
        Script_p->DOM.Objects.Event_p    = Copy_p->DOM.Objects.Event_p;
    }

NH_END(NH_SUCCESS)
}

// UNLOAD ==========================================================================================

void Nh_JS_unloadScript(
    Nh_JS_Script *Script_p)
{   
NH_BEGIN()

    if (!Script_p->Flags.loaded) {NH_SILENT_END()}

    int loadedCount = 0;
    for (int i = 0; i < Script_p->Run.Tab_p->Document.Scripts.count; ++i) { 
        Nh_JS_Script *Other_p = Nh_getListItem(&Script_p->Run.Tab_p->Document.Scripts, i);
        if (Other_p->Flags.loaded) {loadedCount++;}
    }

    if (loadedCount == 1)
    {
        Nh_JS_destroyObject(Script_p->Core.Objects.Math_p);
        Nh_JS_destroyObject(Script_p->DOM.Objects.Window_p);
        Nh_JS_destroyObject(Script_p->DOM.Objects.Location_p);
        Nh_JS_destroyDocumentObject(Script_p->DOM.Objects.Document_p);
        Nh_JS_destroyObject(Script_p->DOM.Objects.Console_p);
        Nh_JS_destroyObject(Script_p->DOM.Objects.NhWindow_p);
        Nh_JS_destroyObject(Script_p->DOM.Objects.NhTab_p);
        Nh_JS_destroyEventObject(Script_p->DOM.Objects.Event_p);
    }

    Nh_JS_resetScriptVariables(Script_p);
    Nh_JS_freeEventListeners(Script_p);

    Script_p->Run.Tab_p = NULL;
    Script_p->Flags.loaded = false;

NH_SILENT_END()
}

