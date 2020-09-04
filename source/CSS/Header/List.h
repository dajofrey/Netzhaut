#ifndef NH_CSS_LIST_H
#define NH_CSS_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "GenericProperty.h"

#include "../../API/Header/Netzhaut.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_LIST_STYLE_TYPE {
        NH_CSS_LIST_STYLE_TYPE__CUSTOM_IDENT,       
        NH_CSS_LIST_STYLE_TYPE__SYMBOLS,            
        NH_CSS_LIST_STYLE_TYPE_DISC,                
        NH_CSS_LIST_STYLE_TYPE_ARMENIAN,            
        NH_CSS_LIST_STYLE_TYPE_CIRCLE,              
        NH_CSS_LIST_STYLE_TYPE_CJK_IDEOGRAPHIC,     
        NH_CSS_LIST_STYLE_TYPE_DECIMAL, 	        
        NH_CSS_LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO,
        NH_CSS_LIST_STYLE_TYPE_GEORGIAN, 	        
        NH_CSS_LIST_STYLE_TYPE_HEBREW, 	        
        NH_CSS_LIST_STYLE_TYPE_HIRAGANA, 	        
        NH_CSS_LIST_STYLE_TYPE_HIRAGANA_IROHA,      
        NH_CSS_LIST_STYLE_TYPE_KATAKANA,            
        NH_CSS_LIST_STYLE_TYPE_KATAKANA_IROHA,      
        NH_CSS_LIST_STYLE_TYPE_LOWER_ALPHA, 	
        NH_CSS_LIST_STYLE_TYPE_LOWER_GREEK, 	
        NH_CSS_LIST_STYLE_TYPE_LOWER_LATIN, 	
        NH_CSS_LIST_STYLE_TYPE_LOWER_ROMAN, 	
        NH_CSS_LIST_STYLE_TYPE_NONE, 	        
        NH_CSS_LIST_STYLE_TYPE_SQUARE, 	        
        NH_CSS_LIST_STYLE_TYPE_UPPER_ALPHA,         
        NH_CSS_LIST_STYLE_TYPE_UPPER_GREEK,         
        NH_CSS_LIST_STYLE_TYPE_UPPER_LATIN,         
        NH_CSS_LIST_STYLE_TYPE_UPPER_ROMAN,         
    } NH_CSS_LIST_STYLE_TYPE;

/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_List {
        NH_CSS_LIST_STYLE_TYPE styleType;
    } NH_CSS_List;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_setDefaultListProperties(
        Nh_HTML_Node *Node_p
    );
    
    void Nh_CSS_setDefaultListProperty(
        Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type
    );
    
    NH_RESULT Nh_CSS_computeListProperties(
        Nh_HTML_Node *Node_p, NH_CSS_GenericProperty **Properties_pp
    );

/** @} */

#endif
