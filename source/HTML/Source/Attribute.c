// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Attribute.h"
#include "../Header/Document.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/HashMap.h"

#include "../../JavaScript/DOM/Header/HTMLElement.h"
#include "../../JavaScript/DOM/Header/EventListener.h"

#include "../../CSS/Header/Properties.h"
#include "../../CSS/Header/Associate.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>

// DATA ============================================================================================

const char *NH_HTML_ATTRIBUTES_PP[] = 
{
    "accept",                 
    "accept-charset",          
    "accesskey",              
    "action",                 
    "align",                     
    "alt",                    
    "async",                  
    "autocomplete",           
    "autofocus",              
    "autoplay",               
    "bgcolor",                
    "border",                 
    "charset", 	            
    "checked", 	            
    "cite", 	                
    "class", 	                
    "color", 	                
    "cols", 	                
    "colspan", 	            
    "content", 	              
    "contenteditable",        
    "controls", 	            
    "coords", 	            
    "data", 	                
    "data-*", 	            
    "datetime",               
    "default",                
    "defer", 	                
    "dir", 	                
    "dirname",                
    "disabled",               
    "download",               
    "draggable",              
    "dropzone",               
    "enctype",                
    "for", 	                
    "form", 	                
    "formaction",             
    "headers",                
    "height", 	            
    "hidden", 	            
    "high", 	                
    "href", 	                
    "hreflang", 	            
    "http-equiv", 	        
    "id", 	                
    "ismap", 	                
    "kind", 	                
    "label",                  
    "lang", 	                
    "list", 	                
    "loop", 	                
    "low", 	                
    "max", 	                
    "maxlength",              
    "media",                  
    "method", 	            
    "min", 	                
    "multiple", 	            
    "muted", 	                
    "name", 	                
    "novalidate", 	        
    "onabort",                
    "onafterprint", 	          
    "onbeforeprint", 	        
    "onbeforeunload", 	    
    "onblur",                 
    "oncanplay",              
    "oncanplaythrough",       
    "onchange",               
    "onclick",                
    "oncontextmenu",          
    "oncopy",                 
    "oncuechange",            
    "oncut",                  
    "ondblclick",             
    "ondrag",                 
    "ondragend",              
    "ondragenter",            
    "ondragleave",            
    "ondragover",             
    "ondragstart",            
    "ondrop",                 
    "ondurationchange",       
    "onemptied",              
    "onended",                
    "onerror",                
    "onfocus",                
    "onhashchange",           
    "oninput", 	            
    "oninvalid", 	            
    "onkeydown", 	            
    "onkeypress", 	        
    "onkeyup", 	            
    "onload", 	            
    "onloadeddata",           
    "onloadedmetadata",       
    "onloadstart", 	        
    "onmousedown", 	        
    "onmousemove", 	        
    "onmouseout", 	        
    "onmouseover",            
    "onmouseup", 	             
    "onmousewheel",           
    "onoffline", 	            
    "ononline", 	            
    "onpagehide", 	        
    "onpageshow", 	        
    "onpaste", 	            
    "onpause", 	            
    "onplay", 	            
    "onplaying",              
    "onpopstate",             
    "onprogress", 	        
    "onratechange",           
    "onreset", 	            
    "onresize", 	            
    "onscroll", 	            
    "onsearch", 	            
    "onseeked", 	            
    "onseeking", 	            
    "onselect", 	            
    "onstalled",              
    "onstorage", 	            
    "onsubmit", 	            
    "onsuspend", 	            
    "ontimeupdate",           
    "ontoggle", 	            
    "onunload", 	            
    "onvolumechange",         
    "onwaiting", 	            
    "onwheel", 	            
    "open", 	                
    "optimum",                
    "pattern",                
    "placeholder",            
    "poster", 	            
    "preload",                
    "readonly",               
    "rel", 	                
    "required",               
    "reversed",               
    "rows", 	                
    "rowspan",                
    "sandbox",                
    "scope", 	                
    "selected",               
    "shape", 	                
    "size", 	                
    "sizes", 	                
    "span", 	 	            
    "spellcheck",             
    "src", 	                
    "srcdoc", 	            
    "srclang",                
    "srcset", 	            
    "start", 	                
    "step", 	                
    "style", 	                
    "tabindex",               
    "target", 	            
    "title", 	                
    "translate",              
    "type", 	                
    "usemap", 	            
    "value", 	                
    "width", 	                
    "wrap", 	                 
    "link",
    "alink",
    "vlink",
};

size_t NH_HTML_ATTRIBUTES_PP_COUNT = sizeof(NH_HTML_ATTRIBUTES_PP) / sizeof(NH_HTML_ATTRIBUTES_PP[0]);

// DECLARE =========================================================================================

static inline NH_RESULT Nh_HTML_setId(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setOnClick(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setSrc(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setType(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setValue(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setClass(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setSelected(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setOnMouseOver(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setOnMouseDown(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setOnMouseOut(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setOnMouseMove(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);
static inline NH_RESULT Nh_HTML_setOnChange(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p
);

static NH_RESULT Nh_HTML_addEventListener(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p, NH_JS_EVENT type
);

// INIT ============================================================================================

static void Nh_HTML_initAttribute(
    Nh_HTML_Node *Node_p, NH_HTML_ATTRIBUTE type)
{
NH_BEGIN()

    switch (type)
    {
        case NH_HTML_ATTRIBUTE_ID       : Node_p->Computed.Attributes.id_p       = NULL; break;
        case NH_HTML_ATTRIBUTE_ON_CLICK : Node_p->Computed.Attributes.onClick_p  = NULL; break;
        case NH_HTML_ATTRIBUTE_TYPE     : Node_p->Computed.Attributes.type       = NH_HTML_TYPE_TEXT; break;
        case NH_HTML_ATTRIBUTE_SRC      : Node_p->Computed.Attributes.Texture_p  = NULL; break;
        case NH_HTML_ATTRIBUTE_VALUE    : Node_p->Computed.Attributes.value_p    = NULL; break;
        case NH_HTML_ATTRIBUTE_CLASS    : Node_p->Computed.Attributes.class_p    = NULL; break;
        case NH_HTML_ATTRIBUTE_SELECTED : Node_p->Computed.Attributes.selected   = false; break;
    }

NH_SILENT_END()
}

static void Nh_HTML_initAttributes(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_initAttribute(Node_p, NH_HTML_ATTRIBUTE_ID);
    Nh_HTML_initAttribute(Node_p, NH_HTML_ATTRIBUTE_ON_CLICK);
    Nh_HTML_initAttribute(Node_p, NH_HTML_ATTRIBUTE_TYPE);
    Nh_HTML_initAttribute(Node_p, NH_HTML_ATTRIBUTE_SRC);
    Nh_HTML_initAttribute(Node_p, NH_HTML_ATTRIBUTE_VALUE);
    Nh_HTML_initAttribute(Node_p, NH_HTML_ATTRIBUTE_CLASS);
    Nh_HTML_initAttribute(Node_p, NH_HTML_ATTRIBUTE_SELECTED);

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_HTML_computeAttributes(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Attribute *Attributes_pp[NH_HTML_ATTRIBUTE_COUNT];
    for (int i = 0; i < NH_HTML_ATTRIBUTE_COUNT; ++i) {Attributes_pp[i] = NULL;}

    Nh_HTML_getAttributes(Tab_p, Node_p, Attributes_pp);
    Nh_HTML_initAttributes(Node_p);

    Nh_HTML_setId(Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_ID]);
    Nh_HTML_setOnClick(Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_ON_CLICK]);
    Nh_HTML_setType(Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_TYPE]);
    Nh_HTML_setSrc(Tab_p, Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_SRC]);
    Nh_HTML_setValue(Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_VALUE]);
    Nh_HTML_setClass(Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_CLASS]);
    Nh_HTML_setSelected(Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_SELECTED]);
    Nh_HTML_setOnMouseDown(Tab_p, Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_ON_MOUSE_DOWN]);
    Nh_HTML_setOnMouseOver(Tab_p, Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_ON_MOUSE_OVER]);
    Nh_HTML_setOnMouseMove(Tab_p, Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_ON_MOUSE_MOVE]);
    Nh_HTML_setOnMouseOut(Tab_p, Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_ON_MOUSE_OUT]);
    Nh_HTML_setOnChange(Tab_p, Node_p, Attributes_pp[NH_HTML_ATTRIBUTE_ON_CHANGE]);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_handleAttributeChange(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_HTML_ATTRIBUTE type)
{
NH_BEGIN()

    switch (type)
    {
        case NH_HTML_ATTRIBUTE_ID    :
        case NH_HTML_ATTRIBUTE_CLASS :
            break; // TODO

        default : NH_CHECK(Nh_HTML_computeAttributes(Tab_p, Node_p))
    }

NH_END(NH_SUCCESS)
}

// SET =============================================================================================

static inline NH_RESULT Nh_HTML_setId(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) {Node_p->Computed.Attributes.id_p = Attribute_p->value_p;} 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setClass(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) {Node_p->Computed.Attributes.class_p = Attribute_p->value_p;} 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setOnClick(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) {Node_p->Computed.Attributes.onClick_p = Attribute_p->value_p;} 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setSrc(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) 
    {
        if (Node_p->tag == NH_HTML_TAG_IMG) {
            Node_p->Computed.Attributes.Texture_p = Nh_Gfx_initTexture(Tab_p, NULL, Attribute_p->value_p);
        }
    } 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setValue(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) {Node_p->Computed.Attributes.value_p = Attribute_p->value_p;} 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setType(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL)
    {
             if (!strcmp(Attribute_p->value_p, "button"))         {Node_p->Computed.Attributes.type = NH_HTML_TYPE_BUTTON;}
        else if (!strcmp(Attribute_p->value_p, "checkbox"))       {Node_p->Computed.Attributes.type = NH_HTML_TYPE_CHECKBOX;}
        else if (!strcmp(Attribute_p->value_p, "color"))          {Node_p->Computed.Attributes.type = NH_HTML_TYPE_COLOR;}
        else if (!strcmp(Attribute_p->value_p, "date"))           {Node_p->Computed.Attributes.type = NH_HTML_TYPE_DATE;}
        else if (!strcmp(Attribute_p->value_p, "datetime"))       {Node_p->Computed.Attributes.type = NH_HTML_TYPE_DATETIME;}
        else if (!strcmp(Attribute_p->value_p, "datetime-local")) {Node_p->Computed.Attributes.type = NH_HTML_TYPE_DATETIME_LOCAL;}
        else if (!strcmp(Attribute_p->value_p, "email"))          {Node_p->Computed.Attributes.type = NH_HTML_TYPE_EMAIL;}
        else if (!strcmp(Attribute_p->value_p, "file"))           {Node_p->Computed.Attributes.type = NH_HTML_TYPE_FILE;}
        else if (!strcmp(Attribute_p->value_p, "hidden"))         {Node_p->Computed.Attributes.type = NH_HTML_TYPE_HIDDEN;}
        else if (!strcmp(Attribute_p->value_p, "image"))          {Node_p->Computed.Attributes.type = NH_HTML_TYPE_IMAGE;}
        else if (!strcmp(Attribute_p->value_p, "month"))          {Node_p->Computed.Attributes.type = NH_HTML_TYPE_MONTH;}
        else if (!strcmp(Attribute_p->value_p, "number"))         {Node_p->Computed.Attributes.type = NH_HTML_TYPE_NUMBER;}
        else if (!strcmp(Attribute_p->value_p, "password"))       {Node_p->Computed.Attributes.type = NH_HTML_TYPE_PASSWORD;}
        else if (!strcmp(Attribute_p->value_p, "radio"))          {Node_p->Computed.Attributes.type = NH_HTML_TYPE_RADIO;}
        else if (!strcmp(Attribute_p->value_p, "range"))          {Node_p->Computed.Attributes.type = NH_HTML_TYPE_RANGE;}
        else if (!strcmp(Attribute_p->value_p, "reset"))          {Node_p->Computed.Attributes.type = NH_HTML_TYPE_RESET;}
        else if (!strcmp(Attribute_p->value_p, "search"))         {Node_p->Computed.Attributes.type = NH_HTML_TYPE_SEARCH;}
        else if (!strcmp(Attribute_p->value_p, "submit"))         {Node_p->Computed.Attributes.type = NH_HTML_TYPE_SUBMIT;}
        else if (!strcmp(Attribute_p->value_p, "tel"))            {Node_p->Computed.Attributes.type = NH_HTML_TYPE_TEL;}
        else if (!strcmp(Attribute_p->value_p, "text"))           {Node_p->Computed.Attributes.type = NH_HTML_TYPE_TEXT;}
        else if (!strcmp(Attribute_p->value_p, "time"))           {Node_p->Computed.Attributes.type = NH_HTML_TYPE_TIME;}
        else if (!strcmp(Attribute_p->value_p, "url"))            {Node_p->Computed.Attributes.type = NH_HTML_TYPE_URL;}
        else if (!strcmp(Attribute_p->value_p, "week"))           {Node_p->Computed.Attributes.type = NH_HTML_TYPE_WEEK;}
    }

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setSelected(
    Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    Node_p->Computed.Attributes.selected = Attribute_p != NULL;

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setOnMouseOver(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) {
        NH_CHECK(Nh_HTML_addEventListener(Tab_p, Node_p, Attribute_p, NH_JS_EVENT_MOUSE_OVER))
    } 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setOnMouseDown(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) {
        NH_CHECK(Nh_HTML_addEventListener(Tab_p, Node_p, Attribute_p, NH_JS_EVENT_MOUSE_DOWN))
    } 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setOnMouseOut(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) {
        NH_CHECK(Nh_HTML_addEventListener(Tab_p, Node_p, Attribute_p, NH_JS_EVENT_MOUSE_OUT))
    } 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setOnMouseMove(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL) {
        NH_CHECK(Nh_HTML_addEventListener(Tab_p, Node_p, Attribute_p, NH_JS_EVENT_MOUSE_MOVE))
    } 

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_HTML_setOnChange(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p)
{
NH_BEGIN()

    if (Attribute_p != NULL && Node_p->tag == NH_HTML_TAG_SELECT) {
        NH_CHECK(Nh_HTML_addEventListener(Tab_p, Node_p, Attribute_p, NH_JS_EVENT_SELECTION_CHANGE))
    } 

NH_END(NH_SUCCESS)
}

// GET =============================================================================================

void Nh_HTML_getAttributes(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attributes_pp[NH_HTML_ATTRIBUTE_COUNT])
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Attributes.count; ++i) 
    {
        Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttribute(&Node_p->Attributes, i);    
        Attributes_pp[Attribute_p->type] = Attribute_p;
    }

NH_SILENT_END()
}

// ATTRIBUTE LIST ==================================================================================

NH_RESULT Nh_HTML_addAttribute(
    Nh_List *Attributes_p, NH_HTML_ATTRIBUTE type, char *value_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Attributes_p)
    Nh_HTML_Attribute *Attribute_p = NULL;

    Attribute_p = Nh_HTML_getAttributeFromType(Attributes_p, type);
    if (Attribute_p != NULL) {
        Attribute_p->value_p = value_p;
        NH_END(NH_SUCCESS)
    }

    Attribute_p = Nh_allocate(sizeof(Nh_HTML_Attribute));
    NH_CHECK_MEM(Attribute_p)
    
    Attribute_p->value_p = value_p;
    Attribute_p->type    = type;

    NH_CHECK(Nh_addListItem(Attributes_p, Attribute_p))

NH_END(NH_SUCCESS)
}

Nh_HTML_Attribute *Nh_HTML_getAttribute(
    Nh_List *Attributes_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(Attributes_p, index))
}

void Nh_HTML_removeAttribute(
    Nh_List *Attributes_p, NH_HTML_ATTRIBUTE type)
{
NH_BEGIN()

    if (Attributes_p == NULL) {NH_SILENT_END()}

    for (int i = 0; i < Attributes_p->count; ++i) {
        Nh_HTML_Attribute *Attribute_p = Nh_getListItem(Attributes_p, i);
        if (Attribute_p->type == type) {
            Nh_free(Attribute_p->value_p);
            Nh_destroyListItem(Attributes_p, i, true);
            NH_SILENT_END()
        }
    }

NH_SILENT_END()
}

Nh_HTML_Attribute *Nh_HTML_getAttributeFromType(
    Nh_List *Attributes_p, NH_HTML_ATTRIBUTE type)
{
NH_BEGIN()

    for (int i = 0; i < Attributes_p->count; ++i) {
        Nh_HTML_Attribute *Attribute_p = Nh_getListItem(Attributes_p, i);
        if (Attribute_p->type == type) {NH_END(Attribute_p)}
    }

NH_END(NULL)
}

void Nh_HTML_destroyAttributes(
    Nh_List *Attributes_p)
{
NH_BEGIN()

    for (int i = 0; i < Attributes_p->count; i++)
    {
        Nh_HTML_Attribute *Attribute_p = Nh_getListItem(Attributes_p, i);
        Nh_free(Attribute_p->value_p);
    }

    Nh_destroyList(Attributes_p, true);

NH_SILENT_END()
}

// HELPER ==========================================================================================

static NH_RESULT Nh_HTML_addEventListener(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attribute_p, NH_JS_EVENT type)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Document.Scripts.count; ++i) 
    {
        Nh_JS_Script *Script_p = Nh_getListItem(&Tab_p->Document.Scripts, i);

        if (!Script_p->Flags.internalScript && Nh_JS_getFunctionDeclaration(Script_p, Attribute_p->value_p) != NULL) 
        {
            Nh_JS_Object *Object_p = Nh_JS_getHTMLElementObject(Script_p, Node_p);
            if (Object_p == NULL) {NH_END(NH_ERROR_BAD_STATE)}
            NH_CHECK(Nh_JS_addEventListener(Script_p, type, Attribute_p->value_p, Object_p))
        }
    }

NH_END(NH_SUCCESS)
}

