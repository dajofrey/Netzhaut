// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/GenericProperty.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Memory.h"
#include "../../HTML/Header/Document.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>

// DATA ============================================================================================

const char *propertyNames_pp[] = 
{
    "-webkit-line-clamp",
    "align-content",             
    "align-items",                
    "align-self",                
    "all",                        	
    "animation",  	              
    "animation-delay",           
    "animation-direction",  	  
    "animation-duration",        
    "animation-fill-mode",  	  
    "animation-iteration-count", 
    "animation-name",  	       
    "animation-play-state", 	   
    "animation-timing-function",  
    "backface-visibility",        
    "background",                 
    "background-attachment",      
    "background-blend-mode",      
    "background-clip",            
    "background-color", 	       
    "background-image",           
    "background-origin",            
    "background-position",        
    "background-repeat",          
    "background-size",      
    "block-size",
    "border",  
    "border-block",
    "border-block-color",
    "border-block-end",
    "border-block-end-color",
    "border-block-end-style",
    "border-block-end-width",
    "border-block-start",
    "border-block-start-color",
    "border-block-start-style",
    "border-block-start-width",
    "border-block-style",
    "border-block-width",
    "border-bottom",  	           
    "border-bottom-color",  	   
    "border-bottom-left-radius",  
    "border-bottom-right-radius", 
    "border-bottom-style", 	   
    "border-bottom-width", 	   
    "border-collapse",
    "border-color",
    "border-end-end-radius",
    "border-end-start-radius",
    "border-image",  	           
    "border-image-outset",        
    "border-image-repeat",        
    "border-image-slice",         
    "border-image-source",        
    "border-image-width",  	 
    "border-inline",
    "border-inline-color",
    "border-inline-end",
    "border-inline-end-color",
    "border-inline-end-style",
    "border-inline-end-width",
    "border-inline-start",
    "border-inline-start-color",
    "border-inline-start-style",
    "border-inline-start-width",
    "border-inline-style",
    "border-inline-width",
    "border-left",  	           
    "border-left-color",  	       
    "border-left-style", 	       
    "border-left-width", 	       
    "border-radius", 	           
    "border-right", 	           
    "border-right-color",         
    "border-right-style",         
    "border-right-width",         
    "border-spacing", 
    "border-start-end-radius",
    "border-start-start-radius",
    "border-style",  	           
    "border-top",  	           
    "border-top-color",  	       
    "border-top-left-radius",  	   
    "border-top-right-radius",    
    "border-top-style",           
    "border-top-width",           
    "border-width",  	           
    "bottom",                     
    "box-decoration-break",       
    "box-shadow",                 
    "box-sizing",                 
    "break-after",                
    "break-before",   	           
    "break-inside",  	           
    "caption-side", 	           
    "caret-color", 	           
    "clear",  	                   
    "clip",
    "clip-path",
    "color",
    "color-adjust",
    "column-count",               
    "column-fill",                
    "column-gap",  	           
    "column-rule",  	           
    "column-rule-color",          
    "column-rule-style",          
    "column-rule-width",          
    "column-span",                
    "column-width",  	           
    "columns",  	               
    "content",  	               
    "counter-increment",  	       
    "counter-reset",
    "counter-set",
    "cursor",                     
    "direction",                  
    "display", 	               
    "empty-cells",                
    "filter", 	                   
    "flex", 	                   
    "flex-basis", 	               
    "flex-direction",             
    "flex-flow", 	               
    "flex-grow", 	               
    "flex-shrink",                
    "flex-wrap", 	               
    "float", 	                   
    "font", 	                   
    "font-family", 	           
    "font-feature-settings",      
    "font-kerning", 	           
    "font-language-override",     
    "font-optical-sizing",
    "font-size", 	                
    "font-size-adjust",            
    "font-stretch", 	            
    "font-style", 	                
    "font-synthesis", 	            
    "font-variant", 	            
    "font-variant-alternates",     
    "font-variant-caps", 	        
    "font-variant-east-asian",     
    "font-variant-ligatures", 	    
    "font-variant-numeric", 	    
    "font-variant-position", 	    
    "font-weight",
    "gap",
    "grid", 	                    
    "grid-area", 	                
    "grid-auto-columns",           
    "grid-auto-flow", 	            
    "grid-auto-rows", 	            
    "grid-column", 	            
    "grid-column-end",             
    "grid-column-gap",             
    "grid-column-start",           
    "grid-gap", 	                
    "grid-row", 	                
    "grid-row-end", 	            
    "grid-row-gap", 	            
    "grid-row-start", 	            
    "grid-template",               
    "grid-template-areas",         
    "grid-template-columns",       
    "grid-template-rows",          
    "hanging-punctuation", 	    
    "height",                      
    "hyphens",                     
    "image-rendering",        
    "inline-size",
    "inset",
    "inset-block",
    "inset-block-end",
    "inset-block-start",
    "inset-inline",
    "inset-inline-end",
    "inset-inline-start",
    "isolation",                   
    "justify-content",
    "justify-items",
    "justify-self",
    "left",                        
    "letter-spacing",              
    "line-break", 	                
    "line-height", 	            
    "list-style", 	                
    "list-style-image", 	        
    "list-style-position",         
    "list-style-type", 	        
    "margin", 
    "margin-block",
    "margin-block-end",
    "margin-block-start",
    "margin-bottom",
    "margin-inline",
    "margin-inline-end",
    "margin-inline-start",
    "margin-left", 	            
    "margin-right", 	            
    "margin-top",
    "mask",
    "mask-clip",
    "mask-composite",
    "mask-image",
    "mask-mode",
    "mask-origin",
    "mask-position",
    "mask-repeat",
    "mask-size",
    "mask-type",
    "max-height",                  
    "max-width",                   
    "min-block-size",                       
    "min-height",
    "min-inline-size",
    "min-width",                   
    "mix-blend-mode",              
    "object-fit", 	                
    "object-position",             
    "opacity", 	                
    "order",                       
    "orphans", 	                
    "outline", 	                
    "outline-color", 	            
    "outline-offset",              
    "outline-style", 	            
    "outline-width", 	 
    "overflow",
    "overflow-wrap",               
    "overflow-x", 	                
    "overflow-y", 	                
    "padding",
    "padding-block",
    "padding-block-end",
    "padding-block-start",
    "padding-bottom", 	  
    "padding-inline",
    "padding-inline-end",
    "padding-inline-start",
    "padding-left", 	            
    "padding-right", 	            
    "padding-top", 	            
    "page-break-after",            
    "page-break-before",           
    "page-break-inside",           
    "perspective",                 
    "perspective-origin",         
    "place-content",
    "place-items",
    "place-self",
    "pointer-events", 	            
    "position",                    
    "quotes",                      
    "resize",                      
    "right",  
    "rotate",
    "row-gap",
    "scale",
    "scroll-behavior",    
    "scroll-margin",
    "scroll-margin-block",
    "scroll-margin-block-end",
    "scroll-margin-block-start",
    "scroll-margin-bottom",
    "scroll-margin-inline",
    "scroll-margin-inline-end",
    "scroll-margin-inline-start",
    "scroll-margin-left",
    "scroll-margin-right",
    "scroll-margin-top",
    "scroll-padding",
    "scroll-padding-block",
    "scroll-padding-block-end",
    "scroll-padding-block-start",
    "scroll-padding-bottom",
    "scroll-padding-inline",
    "scroll-padding-inline-end",
    "scroll-padding-inline-start",
    "scroll-padding-left",
    "scroll-padding-right",
    "scroll-padding-top",
    "scroll-snap-align",
    "scroll-snap-stop",
    "scroll-snap-type",
    "scrollbar-color",
    "scrollbar-width",
    "shape-image-threshold",
    "shape-margin",
    "shape-outside",
    "tab-size",                    
    "table-layout",                
    "text-align",                  
    "text-align-last",             
    "text-combine-upright",        
    "text-decoration",             
    "text-decoration-color",       
    "text-decoration-line",        
    "text-decoration-style",      
    "text-decoration-thickness",
    "text-emphasis",
    "text-emphasis-color",
    "text-emphasis-position",
    "text-emphasis-style",
    "text-indent",                 
    "text-justify",                 
    "text-orientation",            
    "text-overflow", 
    "text-rendering",
    "text-shadow",
    "text-transform",
    "text-underline-offset",
    "text-underline-position",      
    "top", 	  
    "touch-action",
    "transform", 
    "transform-box",
    "transform-origin",            
    "transform-style",             
    "transition", 	                
    "transition-delay", 	        
    "transition-duration",         
    "transition-property",         
    "transition-timing-function",   
    "translate",
    "unicode-bidi",                
    "user-select",                 
    "visibility",                  
    "white-space",
    "widows",
    "width",
    "will-change",
    "word-break", 	                
    "word-spacing", 	            
    "word-wrap", 	                
    "writing-mode", 	            
    "vertical-align",
    "z-index", 	                
};

// CONFIGURE =======================================================================================

void Nh_CSS_configureGenericProperty(
    Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Node_p->Parent_p != NULL && Nh_getListItem(&Node_p->Parent_p->Children.Unformatted, 0) == Node_p) {
        if (Property_p->Pseudo._class == NH_CSS_PSEUDO_CLASS_FIRST_CHILD) {Property_p->active = true;}
    }

NH_SILENT_END()
}

// GET =============================================================================================

void Nh_CSS_getGenericProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Properties_pp[NH_CSS_PROPERTY_COUNT], 
    bool forceInit)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Properties.count; ++i) 
    {
        NH_CSS_GenericProperty *Property_p = Nh_CSS_getProperty(&Node_p->Properties, i);

        if (Property_p->active) 
        {
            if (Properties_pp[Property_p->type] != NULL) 
            {
                if (Nh_CSS_getPrecedence(Property_p->selector) <= Nh_CSS_getPrecedence(Properties_pp[Property_p->type]->selector)) {
                    Properties_pp[Property_p->type] = Property_p;
                }
            } 
            else {Properties_pp[Property_p->type] = Property_p;}
        } 
        else if (forceInit && Properties_pp[Property_p->type] == NULL) {
            Nh_CSS_setDefaultProperty(Tab_p, Node_p, Property_p->type);
        }
    }

NH_SILENT_END()
}

// PROPERTY LIST ===================================================================================

NH_RESULT Nh_CSS_addGenericProperty(
    Nh_List *Properties_p, NH_CSS_GenericProperty *Copy_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Properties_p, Copy_p)

    NH_CSS_GenericProperty *Property_p = Nh_allocate(sizeof(NH_CSS_GenericProperty));
    NH_CHECK_MEM(Property_p)

    Property_p->values_pp          = NULL;
    Property_p->valueCount         = 0; 
    Property_p->Sheet_p            = Copy_p->Sheet_p;
    Property_p->selector           = Copy_p->selector;
    Property_p->type               = Copy_p->type;
    Property_p->Pseudo._class      = Copy_p->Pseudo._class;
    Property_p->Pseudo.parentClass = Copy_p->Pseudo.parentClass;
    Property_p->active             = Copy_p->active;
    Property_p->update             = Copy_p->update;

    if (Copy_p->valueCount > 0) {
        Property_p->values_pp  = Copy_p->values_pp;
        Property_p->valueCount = Copy_p->valueCount;
    }

    Nh_addListItem(Properties_p, Property_p);

NH_END(NH_SUCCESS)
}

NH_CSS_GenericProperty *Nh_CSS_getProperty(
    Nh_List *Properties_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(Properties_p, index))
}

void Nh_CSS_destroyGenericProperties(
    Nh_List *Properties_p)
{
NH_BEGIN()

    Nh_destroyList(Properties_p, true);

NH_SILENT_END()
}

// NAMES ===========================================================================================

const char** Nh_CSS_getPropertyNames(
    size_t *size_p)
{
NH_BEGIN()

    if (size_p != NULL) {*size_p = sizeof(propertyNames_pp) / sizeof(propertyNames_pp[0]);}

NH_END(propertyNames_pp);
}

