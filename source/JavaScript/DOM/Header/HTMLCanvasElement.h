/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_HTML_CANVAS_ELEMENT_H
#define NH_JS_HTML_CANVAS_ELEMENT_H

#include "HTMLElement.h"

#include "../../Header/Interpreter.h"

/** @addtogroup Structs
 *  @{
 */

    typedef struct NH_JS_HTMLCanvasElement {
        Nh_JS_Object *Context2D_p;
    } NH_JS_HTMLCanvasElement;

/** @} */

/** @addtogroup Functions
 *  @{
 */

// API

    Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_HTMLCanvasElement(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_captureStream(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_getContext(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_toDataURL(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_toBlob(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_HTML_CANVAS_ELEMENT_transferControlToOffscreen(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

// INTERNAL

    NH_RESULT Nh_JS_createHTMLCanvasElementObject(
        Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_HTML_Node *Node_p, Nh_JS_Object *Parent_p
    );

    void Nh_JS_destroyHTMLCanvasElementObject(
        Nh_JS_Object *Object_p
    );

/** @} */

#endif 
