#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

#ifndef NH_JS_NH_WINDOW_H
#define NH_JS_NH_WINDOW_H

#include "../../Header/Interpreter.h"

/** \defgroup NhWindow
 *  \brief Custom built-in DOM Object which allows for extended window control.
 *  \ingroup JS
 */

/** @addtogroup NhWindow
 *  @{
 */ 

    Nh_JS_Result Nh_JS_NhWindow_NhWindow(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_NhWindow_backgroundColor(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_NhWindow_closed(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_console(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_controllers(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_customElements(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_crypto(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_devicePixelRatio(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_dialogArguments(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_document(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_DOMMatrix(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_DOMMatrixReadOnly(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_DOMPoint(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_DOMPointReadOnly(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_DOMQuad(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_DOMRect(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_DOMRectReadOnly(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_event(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_frameElement(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_frames(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_fullScreen(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_history(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_innerHeight(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_innerWidth(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_isSecureContext(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_length(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_location(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_locationbar(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_localStorage(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_menubar(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_messageManager(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_mozInnerScreenX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_mozInnerScreenY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_name(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_navigator(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_opener(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_orientation(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_outerHeight(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_outerWidth(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_pageXOffset(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_pageYOffset(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_parent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_performance(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_personalbar(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_screen(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_screenX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_screenY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollbars(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollMaxX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollMaxY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollX(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollY(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_self(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_sessionStorage(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_sidebar(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_speechSynthesis(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_status(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_statusbar(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_toolbar(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_top(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_visualViewport(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_window(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_caches(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_indexedDB(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_origin(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_content(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_defaultStatus(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_directories(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_globalStorage(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_mozAnimationStartTime(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_mozPaintCount(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_pkcs11(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_returnValue(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_alert(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_blur(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_cancelAnimationFrame(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_cancelIdleCallback(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_clearImmediate(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_close(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_confirm(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_dump(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_find(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_focus(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_getComputedStyle(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_getDefaultComputedStyle(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_getSelection(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_matchMedia(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_maximize(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_minimize(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_moveBy(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_moveTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_open(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_postMessage(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_print(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_prompt(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_requestAnimationFrame(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_requestIdleCallback(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_resizeBy(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_resizeTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scroll(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollBy(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollByLines(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollByPages(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_scrollTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_setCursor(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_setImmediate(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_setResizable(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_sizeToContent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_stop(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_NhWindow_updateCommands(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

/** @} */

#endif 
