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
#include "../../Core/Header/Tab.h"
#include "../../HTML/Header/Document.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>

// DATA ============================================================================================

const char *NH_CSS_PROPERTIES_PP[] = 
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

size_t NH_CSS_PROPERTIES_PP_COUNT = sizeof(NH_CSS_PROPERTIES_PP) / sizeof(NH_CSS_PROPERTIES_PP[0]);

// CONFIGURE =======================================================================================

static void Nh_CSS_configureAttributeSelector(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

//    NH_BOOL active = Nh_CSS_attributeSelectorHit(Property_p->selector_p, Node_p);
//    if (active && !Property_p->active) {Property_p->active = Property_p->update = true;}
//    else if (!active && Property_p->active) {
//        Property_p->active = false;
//        Property_p->update = true;
//    }

NH_SILENT_END()
}

static void Nh_CSS_configureGenericProperty(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

//    if (Node_p->Parent_p != NULL && Nh_getListItem(&Node_p->Parent_p->Children.Unformatted, 0) == Node_p) {
//        if (Property_p->Pseudo._class == NH_CSS_PSEUDO_CLASS_FIRST_CHILD) {Property_p->active = true;}
//    }
//
//    if (selector == NH_CSS_SELECTOR_ATTRIBUTE) {
//        Nh_CSS_configureAttributeSelector(Node_p, Property_p);
//    }
//
//    if (Property_p->selector == NH_CSS_SELECTOR_CHILD_COMBINATOR)
//    {
//        char *parts_pp[2], part1_p[256] = {'\0'}, part2_p[256] = {'\0'};
//        parts_pp[0] = part1_p, parts_pp[1] = part2_p;
//        Nh_CSS_getSelectorParts(Property_p->selector_p, NH_CSS_SELECTOR_CHILD_COMBINATOR, (char**)parts_pp);
//        if (Nh_CSS_naiveSelectorHit(Node_p, parts_pp[0], false))
//        Nh_CSS_GenericProperty Left, Right;
//        Left.selector_p = part1_p;
//        
//    }

NH_SILENT_END()
}

void Nh_CSS_configureGenericProperties(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Properties.count; ++i) {
        Nh_CSS_configureGenericProperty(
            Node_p, Nh_CSS_getProperty(&Node_p->Properties, i)
        );
    }

NH_SILENT_END()
}

// GET =============================================================================================

void Nh_CSS_getGenericProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Properties_pp[NH_CSS_PROPERTY_COUNT])
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Properties.count; ++i) 
    {
        Nh_CSS_GenericProperty *Property_p = Nh_CSS_getProperty(&Node_p->Properties, i);

        if (Property_p->active) 
        {
            if (Properties_pp[Property_p->type] != NULL) 
            {
                if (Nh_CSS_getPrecedence(Property_p->Selector.type) <= Nh_CSS_getPrecedence(Properties_pp[Property_p->type]->Selector.type)) {
                    Properties_pp[Property_p->type] = Property_p;
                }
            } 
            else {Properties_pp[Property_p->type] = Property_p;}
        } 
    }

NH_SILENT_END()
}

// ACTIVATE/DEACTIVATE =============================================================================

NH_RESULT Nh_CSS_deactivate(
    Nh_Tab *Tab_p, NH_CSS_PSEUDO_CLASS pseudoClass)
{
NH_BEGIN()

//    for (int i = 0; i < Tab_p->Document.Tree.Flat.Unformatted.count; ++i) 
//    {
//        Nh_HTML_Node *Node_p = Nh_getListItem(&Tab_p->Document.Tree.Flat.Unformatted, i);
//
//        for (int j = 0; j < Node_p->Properties.count; ++j) 
//        {
//            Nh_CSS_GenericProperty *Property_p = Nh_CSS_getProperty(&Node_p->Properties, j);
//
//            if ((Property_p->Pseudo._class == pseudoClass || Property_p->Pseudo.parentClass == pseudoClass) && Property_p->active)
//            {
//                Property_p->active = false;
//                Property_p->update = true;
//            }
//        }
//    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_activate(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p, NH_CSS_PSEUDO_CLASS pseudoClass)
{
NH_BEGIN()

//    if (Property_p->Pseudo._class == pseudoClass && !Property_p->active) {
//        Property_p->update = Property_p->active = true;
//    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_activateChild(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p, NH_CSS_PSEUDO_CLASS pseudoClass)
{
NH_BEGIN()

//    if (Property_p->Pseudo.parentClass == pseudoClass && !Property_p->active) {
//        Property_p->update = Property_p->active = true;
//    }

NH_END(NH_SUCCESS)
}

// PROPERTY LIST ===================================================================================

NH_RESULT Nh_CSS_addGenericProperty(
    Nh_List *Properties_p, Nh_CSS_GenericProperty *Copy_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Properties_p, Copy_p)

    Nh_CSS_GenericProperty *Property_p = Nh_allocate(sizeof(Nh_CSS_GenericProperty));
    NH_CHECK_MEM(Property_p)

    Property_p->values_pp  = NULL;
    Property_p->valueCount = 0; 
    Property_p->Sheet_p    = Copy_p->Sheet_p;
    Property_p->Selector   = Copy_p->Selector;
    Property_p->type       = Copy_p->type;
    Property_p->active     = Copy_p->active;
    Property_p->update     = Copy_p->update;

    NH_CSS_PROPERTY type = Property_p->type;

    Property_p->triggerRecompute   =

           type == NH_CSS_PROPERTY__WEBKIT_LINE_CLAMP
        || type == NH_CSS_PROPERTY_ALIGN_CONTENT               
        || type == NH_CSS_PROPERTY_ALIGN_ITEMS                             	
        || type == NH_CSS_PROPERTY_ALIGN_SELF                             
        || type == NH_CSS_PROPERTY_ALL                            	
        || type == NH_CSS_PROPERTY_BACKFACE_VISIBILITY          
        || type == NH_CSS_PROPERTY_BLOCK_SIZE                  
        || type == NH_CSS_PROPERTY_BORDER                      
        || type == NH_CSS_PROPERTY_BORDER_BLOCK                
        || type == NH_CSS_PROPERTY_BORDER_BLOCK_END            
        || type == NH_CSS_PROPERTY_BORDER_BLOCK_END_WIDTH      
        || type == NH_CSS_PROPERTY_BORDER_BLOCK_START          
        || type == NH_CSS_PROPERTY_BORDER_BLOCK_START_WIDTH    
        || type == NH_CSS_PROPERTY_BORDER_BLOCK_STYLE          
        || type == NH_CSS_PROPERTY_BORDER_BLOCK_WIDTH          
        || type == NH_CSS_PROPERTY_BORDER_BOTTOM               
        || type == NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH         
        || type == NH_CSS_PROPERTY_BORDER_COLLAPSE             
        || type == NH_CSS_PROPERTY_BORDER_END_END_RADIUS       
        || type == NH_CSS_PROPERTY_BORDER_END_START_RADIUS     
        || type == NH_CSS_PROPERTY_BORDER_IMAGE                
        || type == NH_CSS_PROPERTY_BORDER_IMAGE_OUTSET         
        || type == NH_CSS_PROPERTY_BORDER_IMAGE_REPEAT         
        || type == NH_CSS_PROPERTY_BORDER_IMAGE_SLICE          
        || type == NH_CSS_PROPERTY_BORDER_IMAGE_SOURCE         
        || type == NH_CSS_PROPERTY_BORDER_IMAGE_WIDTH          
        || type == NH_CSS_PROPERTY_BORDER_INLINE               
        || type == NH_CSS_PROPERTY_BORDER_INLINE_END           
        || type == NH_CSS_PROPERTY_BORDER_INLINE_END_WIDTH     
        || type == NH_CSS_PROPERTY_BORDER_INLINE_START         
        || type == NH_CSS_PROPERTY_BORDER_INLINE_START_STYLE   
        || type == NH_CSS_PROPERTY_BORDER_INLINE_START_WIDTH   
        || type == NH_CSS_PROPERTY_BORDER_INLINE_STYLE        
        || type == NH_CSS_PROPERTY_BORDER_INLINE_WIDTH        
        || type == NH_CSS_PROPERTY_BORDER_LEFT                 
        || type == NH_CSS_PROPERTY_BORDER_LEFT_STYLE          
        || type == NH_CSS_PROPERTY_BORDER_LEFT_WIDTH          
        || type == NH_CSS_PROPERTY_BORDER_RADIUS               
        || type == NH_CSS_PROPERTY_BORDER_RIGHT                
        || type == NH_CSS_PROPERTY_BORDER_RIGHT_STYLE          
        || type == NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH          
        || type == NH_CSS_PROPERTY_BORDER_SPACING              
        || type == NH_CSS_PROPERTY_BORDER_START_END_RADIUS     
        || type == NH_CSS_PROPERTY_BORDER_START_START_RADIUS   
        || type == NH_CSS_PROPERTY_BORDER_STYLE                
        || type == NH_CSS_PROPERTY_BORDER_TOP                  
        || type == NH_CSS_PROPERTY_BORDER_TOP_LEFT_RADIUS      
        || type == NH_CSS_PROPERTY_BORDER_TOP_RIGHT_RADIUS     
        || type == NH_CSS_PROPERTY_BORDER_TOP_STYLE            
        || type == NH_CSS_PROPERTY_BORDER_TOP_WIDTH            
        || type == NH_CSS_PROPERTY_BORDER_WIDTH               
        || type == NH_CSS_PROPERTY_BOTTOM                     
        || type == NH_CSS_PROPERTY_BOX_DECORATION_BREAK    
        || type == NH_CSS_PROPERTY_BOX_SHADOW                  
        || type == NH_CSS_PROPERTY_BOX_SIZING                  
        || type == NH_CSS_PROPERTY_BREAK_AFTER                 
        || type == NH_CSS_PROPERTY_BREAK_BEFORE                
        || type == NH_CSS_PROPERTY_BREAK_INSIDE                
        || type == NH_CSS_PROPERTY_CAPTION_SIDE                
        || type == NH_CSS_PROPERTY_CLEAR                       
        || type == NH_CSS_PROPERTY_CLIP                        
        || type == NH_CSS_PROPERTY_CLIP_PATH                   
        || type == NH_CSS_PROPERTY_COLUMN_COUNT                
        || type == NH_CSS_PROPERTY_COLUMN_FILL                 
        || type == NH_CSS_PROPERTY_COLUMN_GAP                  
        || type == NH_CSS_PROPERTY_COLUMN_RULE                 
        || type == NH_CSS_PROPERTY_COLUMN_RULE_STYLE           
        || type == NH_CSS_PROPERTY_COLUMN_RULE_WIDTH           
        || type == NH_CSS_PROPERTY_COLUMN_SPAN                 
        || type == NH_CSS_PROPERTY_COLUMN_WIDTH                
        || type == NH_CSS_PROPERTY_COLUMNS                     
        || type == NH_CSS_PROPERTY_CONTENT                     
        || type == NH_CSS_PROPERTY_COUNTER_INCREMENT           
        || type == NH_CSS_PROPERTY_COUNTER_RESET              
        || type == NH_CSS_PROPERTY_COUNTER_SET                 
        || type == NH_CSS_PROPERTY_CURSOR                      
        || type == NH_CSS_PROPERTY_DIRECTION                   
        || type == NH_CSS_PROPERTY_DISPLAY                     
        || type == NH_CSS_PROPERTY_EMPTY_CELLS                 
        || type == NH_CSS_PROPERTY_FILTER                      
        || type == NH_CSS_PROPERTY_FLEX                        
        || type == NH_CSS_PROPERTY_FLEX_BASIS                  
        || type == NH_CSS_PROPERTY_FLEX_DIRECTION              
        || type == NH_CSS_PROPERTY_FLEX_FLOW                   
        || type == NH_CSS_PROPERTY_FLEX_GROW                   
        || type == NH_CSS_PROPERTY_FLEX_SHRINK                 
        || type == NH_CSS_PROPERTY_FLEX_WRAP                   
        || type == NH_CSS_PROPERTY_FLOAT                       
        || type == NH_CSS_PROPERTY_FONT                        
        || type == NH_CSS_PROPERTY_FONT_FAMILY                 
        || type == NH_CSS_PROPERTY_FONT_FEATURE_SETTINGS       
        || type == NH_CSS_PROPERTY_FONT_KERNING                
        || type == NH_CSS_PROPERTY_FONT_LANGUAGE_OVERRIDE      
        || type == NH_CSS_PROPERTY_FONT_OPTICAL_SIZING         
        || type == NH_CSS_PROPERTY_FONT_SIZE                   
        || type == NH_CSS_PROPERTY_FONT_SIZE_ADJUST            
        || type == NH_CSS_PROPERTY_FONT_STRETCH                
        || type == NH_CSS_PROPERTY_FONT_STYLE                  
        || type == NH_CSS_PROPERTY_FONT_SYNTHESIS              
        || type == NH_CSS_PROPERTY_FONT_VARIANT                
        || type == NH_CSS_PROPERTY_FONT_VARIANT_ALTERNATES     
        || type == NH_CSS_PROPERTY_FONT_VARIANT_CAPS           
        || type == NH_CSS_PROPERTY_FONT_VARIANT_EAST_ASIAN     
        || type == NH_CSS_PROPERTY_FONT_VARIANT_LIGATURES      
        || type == NH_CSS_PROPERTY_FONT_VARIANT_NUMERIC        
        || type == NH_CSS_PROPERTY_FONT_VARIANT_POSITION       
        || type == NH_CSS_PROPERTY_FONT_WEIGHT                 
        || type == NH_CSS_PROPERTY_GAP                          
        || type == NH_CSS_PROPERTY_GRID                        
        || type == NH_CSS_PROPERTY_GRID_AREA                   
        || type == NH_CSS_PROPERTY_GRID_AUTO_COLUMNS           
        || type == NH_CSS_PROPERTY_GRID_AUTO_FLOW              
        || type == NH_CSS_PROPERTY_GRID_AUTO_ROWS              
        || type == NH_CSS_PROPERTY_GRID_COLUMN                 
        || type == NH_CSS_PROPERTY_GRID_COLUMN_END             
        || type == NH_CSS_PROPERTY_GRID_COLUMN_GAP             
        || type == NH_CSS_PROPERTY_GRID_COLUMN_START           
        || type == NH_CSS_PROPERTY_GRID_GAP                    
        || type == NH_CSS_PROPERTY_GRID_ROW                    
        || type == NH_CSS_PROPERTY_GRID_ROW_END                
        || type == NH_CSS_PROPERTY_GRID_ROW_GAP                
        || type == NH_CSS_PROPERTY_GRID_ROW_START              
        || type == NH_CSS_PROPERTY_GRID_TEMPLATE               
        || type == NH_CSS_PROPERTY_GRID_TEMPLATE_AREAS         
        || type == NH_CSS_PROPERTY_GRID_TEMPLATE_COLUMNS       
        || type == NH_CSS_PROPERTY_GRID_TEMPLATE_ROWS          
        || type == NH_CSS_PROPERTY_HANGING_PUNCTUATION         
        || type == NH_CSS_PROPERTY_HEIGHT                      
        || type == NH_CSS_PROPERTY_HYPHENS                     
        || type == NH_CSS_PROPERTY_IMAGE_RENDERING             
        || type == NH_CSS_PROPERTY_INLINE_SIZE                 
        || type == NH_CSS_PROPERTY_INSET                       
        || type == NH_CSS_PROPERTY_INSET_BLOCK                 
        || type == NH_CSS_PROPERTY_INSET_BLOCK_END             
        || type == NH_CSS_PROPERTY_INSET_BLOCK_START           
        || type == NH_CSS_PROPERTY_INSET_INLINE                
        || type == NH_CSS_PROPERTY_INSET_INLINE_END            
        || type == NH_CSS_PROPERTY_INSET_INLINE_START          
        || type == NH_CSS_PROPERTY_ISOLATION                   
        || type == NH_CSS_PROPERTY_JUSTIFY_CONTENT             
        || type == NH_CSS_PROPERTY_JUSTIFY_ITEMS               
        || type == NH_CSS_PROPERTY_JUSTIFY_SELF                
        || type == NH_CSS_PROPERTY_LEFT                        
        || type == NH_CSS_PROPERTY_LETTER_SPACING              
        || type == NH_CSS_PROPERTY_LINE_BREAK                  
        || type == NH_CSS_PROPERTY_LINE_HEIGHT                 
        || type == NH_CSS_PROPERTY_LIST_STYLE                  
        || type == NH_CSS_PROPERTY_LIST_STYLE_IMAGE            
        || type == NH_CSS_PROPERTY_LIST_STYLE_POSITION         
        || type == NH_CSS_PROPERTY_LIST_STYLE_TYPE             
        || type == NH_CSS_PROPERTY_MARGIN                      
        || type == NH_CSS_PROPERTY_MARGIN_BLOCK                
        || type == NH_CSS_PROPERTY_MARGIN_BLOCK_END            
        || type == NH_CSS_PROPERTY_MARGIN_BLOCK_START          
        || type == NH_CSS_PROPERTY_MARGIN_BOTTOM               
        || type == NH_CSS_PROPERTY_MARGIN_INLINE               
        || type == NH_CSS_PROPERTY_MARGIN_INLINE_END           
        || type == NH_CSS_PROPERTY_MARGIN_INLINE_START         
        || type == NH_CSS_PROPERTY_MARGIN_LEFT                 
        || type == NH_CSS_PROPERTY_MARGIN_RIGHT                
        || type == NH_CSS_PROPERTY_MARGIN_TOP                  
        || type == NH_CSS_PROPERTY_MASK                        
        || type == NH_CSS_PROPERTY_MASK_CLIP                   
        || type == NH_CSS_PROPERTY_MASK_COMPOSITE              
        || type == NH_CSS_PROPERTY_MASK_IMAGE                  
        || type == NH_CSS_PROPERTY_MASK_MODE                   
        || type == NH_CSS_PROPERTY_MASK_ORIGIN                 
        || type == NH_CSS_PROPERTY_MASK_POSITION               
        || type == NH_CSS_PROPERTY_MASK_REPEAT                 
        || type == NH_CSS_PROPERTY_MASK_SIZE                   
        || type == NH_CSS_PROPERTY_MASK_TYPE                   
        || type == NH_CSS_PROPERTY_MAX_HEIGHT                  
        || type == NH_CSS_PROPERTY_MAX_WIDTH                   
        || type == NH_CSS_PROPERTY_MIN_BLOCK_SIZE              
        || type == NH_CSS_PROPERTY_MIN_HEIGHT                 
        || type == NH_CSS_PROPERTY_MIN_INLINE_SIZE             
        || type == NH_CSS_PROPERTY_MIN_WIDTH                   
        || type == NH_CSS_PROPERTY_MIX_BLEND_MODE              
        || type == NH_CSS_PROPERTY_OBJECT_FIT                  
        || type == NH_CSS_PROPERTY_OBJECT_POSITION             
        || type == NH_CSS_PROPERTY_OPACITY                     
        || type == NH_CSS_PROPERTY_ORDER                       
        || type == NH_CSS_PROPERTY_ORPHANS                     
        || type == NH_CSS_PROPERTY_OUTLINE                     
        || type == NH_CSS_PROPERTY_OUTLINE_OFFSET              
        || type == NH_CSS_PROPERTY_OUTLINE_STYLE               
        || type == NH_CSS_PROPERTY_OUTLINE_WIDTH               
        || type == NH_CSS_PROPERTY_OVERFLOW                    
        || type == NH_CSS_PROPERTY_OVERFLOW_WRAP               
        || type == NH_CSS_PROPERTY_OVERFLOW_X                  
        || type == NH_CSS_PROPERTY_OVERFLOW_Y                  
        || type == NH_CSS_PROPERTY_PADDING                     
        || type == NH_CSS_PROPERTY_PADDING_BLOCK               
        || type == NH_CSS_PROPERTY_PADDING_BLOCK_END           
        || type == NH_CSS_PROPERTY_PADDING_BLOCK_START         
        || type == NH_CSS_PROPERTY_PADDING_BOTTOM              
        || type == NH_CSS_PROPERTY_PADDING_INLINE              
        || type == NH_CSS_PROPERTY_PADDING_INLINE_END          
        || type == NH_CSS_PROPERTY_PADDING_INLINE_START        
        || type == NH_CSS_PROPERTY_PADDING_LEFT                
        || type == NH_CSS_PROPERTY_PADDING_RIGHT               
        || type == NH_CSS_PROPERTY_PADDING_TOP                 
        || type == NH_CSS_PROPERTY_PAGE_BREAK_AFTER            
        || type == NH_CSS_PROPERTY_PAGE_BREAK_BEFORE           
        || type == NH_CSS_PROPERTY_PAGE_BREAK_INSIDE           
        || type == NH_CSS_PROPERTY_PERSPECTIVE                 
        || type == NH_CSS_PROPERTY_PERSPECTIVE_ORIGIN          
        || type == NH_CSS_PROPERTY_PLACE_CONTENT               
        || type == NH_CSS_PROPERTY_PLACE_ITEMS                 
        || type == NH_CSS_PROPERTY_PLACE_SELF                  
        || type == NH_CSS_PROPERTY_POINTER_EVENTS              
        || type == NH_CSS_PROPERTY_POSITION                    
        || type == NH_CSS_PROPERTY_QUOTES                      
        || type == NH_CSS_PROPERTY_RESIZE                      
        || type == NH_CSS_PROPERTY_RIGHT                       
        || type == NH_CSS_PROPERTY_ROTATE                      
        || type == NH_CSS_PROPERTY_ROW_GAP                     
        || type == NH_CSS_PROPERTY_SCALE                       
        || type == NH_CSS_PROPERTY_SCROLL_BEHAVIOR             
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN               
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_BLOCK         
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_BLOCK_END     
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_BLOCK_START   
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_BOTTOM        
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_INLINE        
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_INLINE_END    
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_INLINE_START  
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_LEFT          
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_RIGHT         
        || type == NH_CSS_PROPERTY_SCROLL_MARGIN_TOP           
        || type == NH_CSS_PROPERTY_SCROLL_PADDING             
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_BLOCK        
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_BLOCK_END    
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_BLOCK_START  
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_BOTTOM       
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_INLINE       
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_INLINE_END   
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_INLINE_START 
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_LEFT         
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_RIGHT        
        || type == NH_CSS_PROPERTY_SCROLL_PADDING_TOP          
        || type == NH_CSS_PROPERTY_SCROLL_SNAP_ALIGN           
        || type == NH_CSS_PROPERTY_SCROLL_SNAP_STOP            
        || type == NH_CSS_PROPERTY_SCROLL_SNAP_TYPE            
        || type == NH_CSS_PROPERTY_SCROLLBAR_WIDTH             
        || type == NH_CSS_PROPERTY_SHAPE_IMAGE_THRESHOLD       
        || type == NH_CSS_PROPERTY_SHAPE_MARGIN                
        || type == NH_CSS_PROPERTY_SHAPE_OUTSIDE               
        || type == NH_CSS_PROPERTY_TAB_SIZE                    
        || type == NH_CSS_PROPERTY_TABLE_LAYOUT                
        || type == NH_CSS_PROPERTY_TEXT_ALIGN                  
        || type == NH_CSS_PROPERTY_TEXT_ALIGN_LAST             
        || type == NH_CSS_PROPERTY_TEXT_COMBINE_UPRIGHT        
        || type == NH_CSS_PROPERTY_TEXT_DECORATION             
        || type == NH_CSS_PROPERTY_TEXT_DECORATION_LINE        
        || type == NH_CSS_PROPERTY_TEXT_DECORATION_STYLE       
        || type == NH_CSS_PROPERTY_TEXT_DECORATION_THICKNESS   
        || type == NH_CSS_PROPERTY_TEXT_EMPHASIS               
        || type == NH_CSS_PROPERTY_TEXT_EMPHASIS_POSITION      
        || type == NH_CSS_PROPERTY_TEXT_EMPHASIS_STYLE         
        || type == NH_CSS_PROPERTY_TEXT_INDENT                 
        || type == NH_CSS_PROPERTY_TEXT_JUSTIFY                
        || type == NH_CSS_PROPERTY_TEXT_ORIENTATION            
        || type == NH_CSS_PROPERTY_TEXT_OVERFLOW               
        || type == NH_CSS_PROPERTY_TEXT_RENDERING              
        || type == NH_CSS_PROPERTY_TEXT_SHADOW                 
        || type == NH_CSS_PROPERTY_TEXT_TRANSFORM              
        || type == NH_CSS_PROPERTY_TEXT_UNDERLINE_OFFSET       
        || type == NH_CSS_PROPERTY_TEXT_UNDERLINE_POSITION     
        || type == NH_CSS_PROPERTY_TOP                         
        || type == NH_CSS_PROPERTY_TOUCH_ACTION                
        || type == NH_CSS_PROPERTY_TRANSFORM                   
        || type == NH_CSS_PROPERTY_TRANSFORM_BOX               
        || type == NH_CSS_PROPERTY_TRANSFORM_ORIGIN            
        || type == NH_CSS_PROPERTY_TRANSFORM_STYLE             
        || type == NH_CSS_PROPERTY_TRANSITION                  
        || type == NH_CSS_PROPERTY_TRANSITION_DELAY            
        || type == NH_CSS_PROPERTY_TRANSITION_DURATION         
        || type == NH_CSS_PROPERTY_TRANSITION_PROPERTY         
        || type == NH_CSS_PROPERTY_TRANSITION_TIMING_FUNCTION  
        || type == NH_CSS_PROPERTY_TRANSLATE                   
        || type == NH_CSS_PROPERTY_UNICODE_BIDI                
        || type == NH_CSS_PROPERTY_USER_SELECT                 
        || type == NH_CSS_PROPERTY_VISIBILITY                  
        || type == NH_CSS_PROPERTY_WHITE_SPACE                 
        || type == NH_CSS_PROPERTY_WIDOWS                      
        || type == NH_CSS_PROPERTY_WIDTH                       
        || type == NH_CSS_PROPERTY_WILL_CHANGE                 
        || type == NH_CSS_PROPERTY_WORD_BREAK                  
        || type == NH_CSS_PROPERTY_WORD_SPACING                
        || type == NH_CSS_PROPERTY_WORD_WRAP                   
        || type == NH_CSS_PROPERTY_WRITING_MODE                
        || type == NH_CSS_PROPERTY_VERTICAL_ALIGN              
        || type == NH_CSS_PROPERTY_Z_INDEX;                 

    if (Copy_p->valueCount > 0) {
        Property_p->values_pp  = Copy_p->values_pp;
        Property_p->valueCount = Copy_p->valueCount;
    }

    Nh_addListItem(Properties_p, Property_p);

NH_END(NH_SUCCESS)
}

Nh_CSS_GenericProperty *Nh_CSS_getProperty(
    Nh_List *Properties_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(Properties_p, index))
}

void Nh_CSS_destroyGenericProperties(
    Nh_List *Properties_p)
{
NH_BEGIN()

    for (int i = 0; i < Properties_p->count; ++i) {
        Nh_CSS_GenericProperty *Property_p = Nh_getListItem(Properties_p, i);
        Nh_destroyList(&Property_p->Selector.Children, true);
    }

    Nh_destroyList(Properties_p, true);

NH_SILENT_END()
}

