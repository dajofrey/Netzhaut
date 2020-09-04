#ifndef NH_CSS_POSITION_H
#define NH_CSS_POSITION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Utils.h"
#include "GenericProperty.h"

#include "../../API/Header/Netzhaut.h"

typedef struct Nh_Tab Nh_Tab;
typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_POSITION {
        NH_CSS_POSITION_STATIC,  
        NH_CSS_POSITION_ABSOLUTE,
        NH_CSS_POSITION_FIXED,   
        NH_CSS_POSITION_RELATIVE,
        NH_CSS_POSITION_STICKY,  
    } NH_CSS_POSITION;
    
    typedef enum NH_CSS_DISPLAY {
        NH_CSS_DISPLAY_INLINE, 	       	
        NH_CSS_DISPLAY_BLOCK, 	            
        NH_CSS_DISPLAY_CONTENTS, 	       	
        NH_CSS_DISPLAY_FLEX, 	       
        NH_CSS_DISPLAY_GRID, 	            
        NH_CSS_DISPLAY_INLINE_BLOCK,       	
        NH_CSS_DISPLAY_INLINE_FLEX,             
        NH_CSS_DISPLAY_INLINE_GRID,        	
        NH_CSS_DISPLAY_INLINE_TABLE,       	
        NH_CSS_DISPLAY_LIST_ITEM,          	
        NH_CSS_DISPLAY_RUN_IN,             	
        NH_CSS_DISPLAY_TABLE,              	
        NH_CSS_DISPLAY_TABLE_CAPTION,      	
        NH_CSS_DISPLAY_TABLE_COLUMN_GROUP, 	
        NH_CSS_DISPLAY_TABLE_HEADER_GROUP, 	
        NH_CSS_DISPLAY_TABLE_FOOTER_GROUP, 	
        NH_CSS_DISPLAY_TABLE_ROW_GROUP,    	
        NH_CSS_DISPLAY_TABLE_CELL,         	
        NH_CSS_DISPLAY_TABLE_COLUMN,            
        NH_CSS_DISPLAY_TABLE_ROW,          	
        NH_CSS_DISPLAY_NONE,               
    } NH_CSS_DISPLAY;

    typedef enum NH_CSS_FLEX_DIRECTION {
        NH_CSS_FLEX_DIRECTION_ROW,     
        NH_CSS_FLEX_DIRECTION_ROW_REVERSE,
        NH_CSS_FLEX_DIRECTION_COLUMN, 
        NH_CSS_FLEX_DIRECTION_COLUMN_REVERSE, 
    } NH_CSS_FLEX_DIRECTION;
    
    typedef enum NH_CSS_FLEX_WRAP {
        NH_CSS_FLEX_WRAP_NOWRAP,     
        NH_CSS_FLEX_WRAP_WRAP,
        NH_CSS_FLEX_WRAP_WRAP_REVERSE, 
    } NH_CSS_FLEX_WRAP;
    
    typedef enum NH_CSS_FLEX_BASIS {
        NH_CSS_FLEX_BASIS__LENGTH,     
        NH_CSS_FLEX_BASIS__PX, 
        NH_CSS_FLEX_BASIS_AUTO,        	
        NH_CSS_FLEX_BASIS_FILL,      
        NH_CSS_FLEX_BASIS_MAX_CONTENT,      
        NH_CSS_FLEX_BASIS_MIN_CONTENT, 
        NH_CSS_FLEX_BASIS_FIT_CONTENT, 
        NH_CSS_FLEX_BASIS_CONTENT, 
    } NH_CSS_FLEX_BASIS;

    typedef enum NH_CSS_ALIGN_CONTENT {
        NH_CSS_ALIGN_CONTENT_START,          
        NH_CSS_ALIGN_CONTENT_END,            
        NH_CSS_ALIGN_CONTENT_STRETCH,          
        NH_CSS_ALIGN_CONTENT_CENTER,           
        NH_CSS_ALIGN_CONTENT_FLEX_START,     
        NH_CSS_ALIGN_CONTENT_FLEX_END,       
        NH_CSS_ALIGN_CONTENT_SPACE_BETWEEN,    
        NH_CSS_ALIGN_CONTENT_SPACE_AROUND,   
        NH_CSS_ALIGN_CONTENT_SPACE_EVENLY,   
        NH_CSS_ALIGN_CONTENT_SAFE,           
        NH_CSS_ALIGN_CONTENT_UNSAFE,         
        NH_CSS_ALIGN_CONTENT_NORMAL,         
        NH_CSS_ALIGN_CONTENT_BASELINE,          
        NH_CSS_ALIGN_CONTENT_FIRST_BASELINE, 
        NH_CSS_ALIGN_CONTENT_LAST_BASELINE,  
    } NH_CSS_ALIGN_CONTENT;

    typedef enum NH_CSS_ALIGN_ITEMS {
        NH_CSS_ALIGN_ITEMS_NORMAL,     
        NH_CSS_ALIGN_ITEMS_START,      
        NH_CSS_ALIGN_ITEMS_END,        
        NH_CSS_ALIGN_ITEMS_SELF_START, 
        NH_CSS_ALIGN_ITEMS_SELF_END,   
        NH_CSS_ALIGN_ITEMS_SAFE,       
        NH_CSS_ALIGN_ITEMS_UNSAFE,     
        NH_CSS_ALIGN_ITEMS_STRETCH,    
        NH_CSS_ALIGN_ITEMS_CENTER,     
        NH_CSS_ALIGN_ITEMS_FLEX_START, 
        NH_CSS_ALIGN_ITEMS_FLEX_END,   
        NH_CSS_ALIGN_ITEMS_BASELINE,   
    } NH_CSS_ALIGN_ITEMS;

    typedef enum NH_CSS_ALIGN_SELF {
        NH_CSS_ALIGN_SELF_AUTO,       
        NH_CSS_ALIGN_SELF_STRETCH,    
        NH_CSS_ALIGN_SELF_CENTER,     
        NH_CSS_ALIGN_SELF_FLEX_START, 
        NH_CSS_ALIGN_SELF_FLEX_END,   
        NH_CSS_ALIGN_SELF_BASELINE,   
    } NH_CSS_ALIGN_SELF;

/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_Position {
        NH_CSS_Generic Width, Height, MinWidth, MinHeight;   
        NH_CSS_Generic Top, Bottom, Left, Right;
        NH_CSS_POSITION position; 
        NH_CSS_DISPLAY display;  
        NH_CSS_Generic FlexBasis;
        NH_CSS_FLEX_DIRECTION flexDirection;
        float flexGrow, flexShrink;
        NH_CSS_FLEX_WRAP flexWrap;
        int order;
        NH_CSS_ALIGN_CONTENT alignContent;
        NH_CSS_ALIGN_ITEMS alignItems;                
        NH_CSS_ALIGN_SELF alignSelf;                  
    } NH_CSS_Position;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_setDefaultPositionProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );
    
    void Nh_CSS_setDefaultPositionProperty(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type
    );
    
    NH_RESULT Nh_CSS_computePositionProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty **Properties_pp
    );

/** @} */

#endif
