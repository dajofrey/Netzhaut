#ifndef NH_CSS_TEXT_H
#define NH_CSS_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "GenericProperty.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/Font.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_Tab Nh_Tab;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_WRITING_MODE {
        NH_CSS_WRITING_MODE_HORIZONTAL_TB,
        NH_CSS_WRITING_MODE_VERTICAL_RL,  
        NH_CSS_WRITING_MODE_VERTICAL_LR,  
        NH_CSS_WRITING_MODE_SIDEWAYS_RL,  
        NH_CSS_WRITING_MODE_SIDEWAYS_LR,  
    } NH_CSS_WRITING_MODE;

    typedef enum NH_CSS_DIRECTION {
        NH_CSS_DIRECTION_LTR, 
        NH_CSS_DIRECTION_RTL, 
    } NH_CSS_DIRECTION;
    
    typedef enum NH_CSS_TEXT_ALIGN {
        NH_CSS_TEXT_ALIGN_START,       
        NH_CSS_TEXT_ALIGN_END,         
        NH_CSS_TEXT_ALIGN_LEFT,        	
        NH_CSS_TEXT_ALIGN_RIGHT,       
        NH_CSS_TEXT_ALIGN_CENTER,      
        NH_CSS_TEXT_ALIGN_JUSTIFY,     
        NH_CSS_TEXT_ALIGN_JUSTIFY_ALL, 
        NH_CSS_TEXT_ALIGN_MATCH_PARENT,
        NH_CSS_TEXT_ALIGN__STRING,     
    } NH_CSS_TEXT_ALIGN;
    
    typedef enum NH_CSS_SIZE {
        NH_CSS_SIZE__LENGTH,     
        NH_CSS_SIZE__PERCENTAGE, 
        NH_CSS_SIZE_AUTO,        	
        NH_CSS_SIZE_MAX_CONTENT,     
        NH_CSS_SIZE_MIN_CONTENT, 
        NH_CSS_SIZE_FIT_CONTENT, 
    } NH_CSS_SIZE;

/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_Text {
        float color_p[4];               
        float fontSize;                 
        Nh_Font *Font_p;
        NH_CSS_DIRECTION direction;     
        NH_CSS_TEXT_ALIGN align;        
        NH_CSS_WRITING_MODE writingMode;
    } NH_CSS_Text;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_computeTextProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp
    );
    
    NH_RESULT Nh_CSS_setDefaultTextProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );
    
    void Nh_CSS_setDefaultTextProperty(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type
    );

/** @} */

#endif 
