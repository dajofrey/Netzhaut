/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_ELEMENT_H
#define NH_JS_ELEMENT_H

#include "../../Header/Interpreter.h"

/** @addtogroup Functions
 *  @{
 */

    Nh_JS_Result Nh_JS_Element_Element(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_Element_after(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_append(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_attributes(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_before(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_childElementCount(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_children(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_classList(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_className(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_clientHeight(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_clientLeft(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_clientTop(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_clientWidth(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_firstElementChild(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_id(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_innerHTML(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_lastElementChild(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_localName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_namespaceURI(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_nextElementSibling(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_outerHTML(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_part(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_prefix(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_prepend(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_previousElementSibling(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollHeight(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollLeft(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollLeftMax(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollTop(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollTopMax(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollWidth(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_shadowRoot(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_openOrClosedShadowRoot(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_slot(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_tabStop(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_tagName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_attachShadow(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_animate(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_closest(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_createShadowRoot(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_computedStyleMap(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getAnimations(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getAttribute(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getAttributeNames(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getAttributeNS(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getBoundingClientRect(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getClientRects(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getElementsByClassName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getElementsByTagName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_getElementsByTagNameNS(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_hasAttribute(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_hasAttributeNS(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_hasAttributes(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_hasPointerCapture(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_insertAdjacentElement(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_insertAdjacentHTML(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_insertAdjacentText(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_matches(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_pseudo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_querySelector(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_querySelectorAll(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_releasePointerCapture(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_remove(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_removeAttribute(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_removeAttributeNS(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_replaceWith(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_requestFullscreen(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_requestPointerLock(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scroll(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollBy(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollIntoView(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_scrollTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_setAttribute(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_setAttributeNS(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_setAttributeNode(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_setAttributeNodeNS(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_setCapture(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_setPointerCapture(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Element_toggleAttribute(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

/** @} */

#endif 
