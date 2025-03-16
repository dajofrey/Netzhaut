#ifndef NH_WEBIDL_RUNTIME_OBJECT_H
#define NH_WEBIDL_RUNTIME_OBJECT_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../ECMAScriptBinding/Interface.h"

// MACROS ==========================================================================================

#define NH_WEBIDL_GET_HTML_DOCUMENT(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "HTML", "Document") 
#define NH_WEBIDL_GET_HTML_ELEMENT(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "HTML", "HTMLElement") 
#define NH_WEBIDL_GET_HTML_IMAGE_ELEMENT(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "HTML", "HTMLImageElement") 
#define NH_WEBIDL_GET_DOM_ELEMENT(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "DOM", "Element") 
#define NH_WEBIDL_GET_DOM_NODE(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "DOM", "Node")
#define NH_WEBIDL_GET_DOM_TEXT(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "DOM", "Text")
#define NH_WEBIDL_GET_CSS_COUNTER_STYLE_RULE(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "CSS", "CSSCounterStyleRule")
#define NH_WEBIDL_GET_CSS_STYLE_RULE(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "CSS", "CSSStyleRule")
#define NH_WEBIDL_GET_CSS_RULE(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "CSS", "CSSRule")
#define NH_WEBIDL_GET_CSS_STYLE_SHEET(ObjectHandle_p) nh_webidl_getObject(ObjectHandle_p, "CSS", "CSSStyleSheet")

// STRUCTS =========================================================================================

typedef struct nh_webidl_Object { 
    nh_webidl_Interface *Interface_p; 
    struct nh_webidl_Object *Parent_p; 
    struct nh_webidl_Object *Child_p; 
    nh_core_List Attributes; 
    nh_core_List Parts; 
    void *internal_p; 
} nh_webidl_Object; 

// FUNCTIONS =======================================================================================

nh_webidl_Object *nh_webidl_createObject(
    char *specification_p, char *interface_p
);

nh_webidl_Object *nh_webidl_createObjectFromInterface(
   nh_webidl_Interface *Interface_p 
);

nh_webidl_Object *nh_webidl_createCompositeObject(
    char *specification_p, char *interface_p, ...
);

nh_webidl_Object *nh_webidl_getObject(
    nh_webidl_Object *Object_p, char *specification_p, char *interface_p
);

void *nh_webidl_getAttribute(
    nh_webidl_Object *Object_p, char *attribute_p
);

nh_core_String nh_webidl_stringifyObjectForDebugging( 
    nh_webidl_Object *Object_p
);

#endif
