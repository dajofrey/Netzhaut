// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/NhTab.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Tab.h"
#include "../../../Core/Header/Memory.h"
#include "../../../CSS/Header/Parser.h"

#include <string.h>

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result Nh_JS_NhTab_NhTab(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_alwaysOnTop(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    switch(aCount)
    {
        case 0:; // get
            NH_END(Nh_JS_getResult(Script_p->Run.Tab_p->Flags.alwaysOnTop ? NH_JS_TYPE_BOOLEAN_TRUE : NH_JS_TYPE_BOOLEAN_FALSE, false, NULL))

        case 1: // set 
            switch (Arguments_p[0].type)
            {
                case NH_JS_TYPE_BOOLEAN_TRUE  : Script_p->Run.Tab_p->Flags.alwaysOnTop = true;  break;
                case NH_JS_TYPE_BOOLEAN_FALSE : Script_p->Run.Tab_p->Flags.alwaysOnTop = false; break;
            }
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_closed(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_console(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_controllers(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_crop(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    switch(aCount)
    {
        case 0: // get
            NH_END(Nh_JS_getResult(Script_p->Run.Tab_p->Flags.crop ? NH_JS_TYPE_BOOLEAN_TRUE : NH_JS_TYPE_BOOLEAN_FALSE, false, NULL))

        case 1: // set 
            switch (Arguments_p[0].type)
            {
                case NH_JS_TYPE_BOOLEAN_TRUE  : Script_p->Run.Tab_p->Flags.crop = true;  break;
                case NH_JS_TYPE_BOOLEAN_FALSE : Script_p->Run.Tab_p->Flags.crop = false; break;
            }
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_customElements(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_crypto(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_devicePixelRatio(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_dialogArguments(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_document(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_DOMMatrix(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_DOMMatrixReadOnly(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_DOMPoint(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_DOMPointReadOnly(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_DOMQuad(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_DOMRect(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_DOMRectReadOnly(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_event(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_frameElement(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_frames(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_fullScreen(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_history(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_innerHeight(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_innerWidth(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_isSecureContext(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_length(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_location(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_locationbar(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_localStorage(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_menubar(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_messageManager(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_mozInnerScreenX(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_mozInnerScreenY(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_name(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_navigator(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_opener(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_orientation(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_outerHeight(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_outerWidth(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_pageXOffset(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_pageYOffset(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_parent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_performance(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_personalbar(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_screen(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_screenX(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_screenY(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollbars(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollMaxX(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollMaxY(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollX(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollY(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_self(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_sessionStorage(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_sidebar(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_speechSynthesis(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}


Nh_JS_Result Nh_JS_NhTab_status(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_statusbar(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_toolbar(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_top(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_visualViewport(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_window(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_caches(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_indexedDB(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_origin(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_content(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_defaultStatus(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_directories(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_globalStorage(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_mozAnimationStartTime(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_mozPaintCount(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_pkcs11(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_returnValue(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_alert(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_blur(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_cancelAnimationFrame(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_cancelIdleCallback(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_clearImmediate(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_close(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_confirm(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_dump(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_find(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_focus(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_getComputedStyle(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_getDefaultComputedStyle(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_getSelection(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_matchMedia(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_maximize(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_minimize(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_moveBy(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_moveTo(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_open(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount <= 0) {NH_END(Nh_JS_getNULLResult())}
    if (Arguments_p[0].type != NH_JS_TYPE_STRING) {NH_END(Nh_JS_getNULLResult())}

// TODO
//    char documentPath_p[2048] = {'\0'};
//    Nh_getCustomAbsolutePath(
//        Arguments_p[0].data_p, Script_p->Run.Tab_p->Document.Location.url_p, documentPath_p
//    );

    switch (aCount)
    {
        case 1 : 
        {
            Nh_startTabThread(Script_p->Run.Tab_p->Window_p, Arguments_p[0].data_p, Script_p->Run.Tab_p->Info.pxSize_p);
            break;
        }
//        case 2 : 
//        {
//            if (Arguments_p[1].type != NH_JS_TYPE_STRING) {NH_END(Nh_JS_getNULLResult())}
//
//            Nh_Tab *Tab_p = Nh_getTabFromString(Arguments_p[1].data_p);
//            if (Tab_p == NULL) {Tab_p = Nh_getTabFromString(documentPath_p);} 
//
//            if (Tab_p == NULL) {
//                Nh_startTabThread(Script_p->Run.Tab_p->Window_p, Arguments_p[1].data_p, documentPath_p, Script_p->Run.Tab_p->Info.pxSize_p);
//            } else {
//                NH_reloadTab(Tab_p, documentPath_p);
//            }
//
//            break;
//        }
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_postMessage(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}


Nh_JS_Result Nh_JS_NhTab_print(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_prompt(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_requestAnimationFrame(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_requestIdleCallback(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_resizeBy(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_resizeTo(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scroll(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollBy(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollByLines(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollByPages(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_scrollTo(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_setCursor(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_setImmediate(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_setResizable(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_sizeToContent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_stop(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_NhTab_updateCommands(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

