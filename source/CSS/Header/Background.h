#ifndef NH_CSS_BACKGROUND_H
#define NH_CSS_BACKGROUND_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "GenericProperty.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/List.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_Tab Nh_Tab;
typedef struct Nh_Window Nh_Window;
typedef struct NH_CSS_Image NH_CSS_Image;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_BACKGROUND_ATTACHMENT {
        NH_CSS_BACKGROUND_ATTACHMENT_FIXED,  
        NH_CSS_BACKGROUND_ATTACHMENT_LOCAL,  
        NH_CSS_BACKGROUND_ATTACHMENT_SCROLL, 
    } NH_CSS_BACKGROUND_ATTACHMENT;
    
    typedef enum NH_CSS_BACKGROUND_BLEND_MODE {
        NH_CSS_BACKGROUND_BLEND_MODE_NORMAL,     
        NH_CSS_BACKGROUND_BLEND_MODE_MULTIPLY,   
        NH_CSS_BACKGROUND_BLEND_MODE_SCREEN,     
        NH_CSS_BACKGROUND_BLEND_MODE_OVERLAY,    
        NH_CSS_BACKGROUND_BLEND_MODE_DARKEN,     
        NH_CSS_BACKGROUND_BLEND_MODE_LIGHTEN,    
        NH_CSS_BACKGROUND_BLEND_MODE_COLOR_DODGE,
        NH_CSS_BACKGROUND_BLEND_MODE_COLOR_BURN, 
        NH_CSS_BACKGROUND_BLEND_MODE_HARD_LIGHT, 
        NH_CSS_BACKGROUND_BLEND_MODE_SOFT_LIGHT, 
        NH_CSS_BACKGROUND_BLEND_MODE_DIFFERENCE,  
        NH_CSS_BACKGROUND_BLEND_MODE_EXCLUSION,  
        NH_CSS_BACKGROUND_BLEND_MODE_HUE,        
        NH_CSS_BACKGROUND_BLEND_MODE_SATURATION, 
        NH_CSS_BACKGROUND_BLEND_MODE_COLOR,      
        NH_CSS_BACKGROUND_BLEND_MODE_LUMINOSITY, 
    } NH_CSS_BACKGROUND_BLEND_MODE;
    
    typedef enum NH_CSS_BACKGROUND_CLIP {
        NH_CSS_BACKGROUND_CLIP_BORDER_BOX, 
        NH_CSS_BACKGROUND_CLIP_PADDING_BOX,
        NH_CSS_BACKGROUND_CLIP_CONTENT_BOX,
        NH_CSS_BACKGROUND_CLIP_TEXT,       
    } NH_CSS_BACKGROUND_CLIP;
    
    typedef enum NH_CSS_BACKGROUND_ORIGIN {
        NH_CSS_BACKGROUND_ORIGIN_BORDER_BOX,  
        NH_CSS_BACKGROUND_ORIGIN_PADDING_BOX, 
        NH_CSS_BACKGROUND_ORIGIN_CONTENT_BOX, 
    } NH_CSS_BACKGROUND_ORIGIN;
    
    typedef enum NH_CSS_BACKGROUND_REPEAT {
        NH_CSS_BACKGROUND_REPEAT_REPEAT_X,  
        NH_CSS_BACKGROUND_REPEAT_REPEAT_Y,  
        NH_CSS_BACKGROUND_REPEAT_REPEAT,    
        NH_CSS_BACKGROUND_REPEAT_SPACE,     
        NH_CSS_BACKGROUND_REPEAT_ROUND,     
        NH_CSS_BACKGROUND_REPEAT_NO_REPEAT, 
    } NH_CSS_BACKGROUND_REPEAT;

    typedef enum NH_CSS_BACKGROUND_SIZE {
        NH_CSS_BACKGROUND_SIZE_COVER,
        NH_CSS_BACKGROUND_SIZE_CONTAIN,
        NH_CSS_BACKGROUND_SIZE_AUTO,
        NH_CSS_BACKGROUND_SIZE_PERCENT,
        NH_CSS_BACKGROUND_SIZE_PIXEL,
    } NH_CSS_BACKGROUND_SIZE;

    /** 
     * The background-position property is kept internally as either percent or length value.
     */
    typedef enum NH_CSS_BACKGROUND_POSITION {
        NH_CSS_BACKGROUND_POSITION_PERCENT, /**<Indicates a percent value.*/
        NH_CSS_BACKGROUND_POSITION_LENGTH,  /**<Indicates a length value.*/
    } NH_CSS_BACKGROUND_POSITION;

/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    /**
     * Can be set using the background-size property. Defines background image size.
     */
    typedef struct NH_CSS_BackgroundSize {
        NH_CSS_BACKGROUND_SIZE type;
        float value;
    } NH_CSS_BackgroundSize;

    /**
     * Can be set using the background-position property. Defines background image position.
     */
    typedef struct NH_CSS_BackgroundPosition {
        NH_CSS_BACKGROUND_POSITION type;
        float value;
    } NH_CSS_BackgroundPosition;

    typedef struct NH_CSS_Background {
        NH_CSS_BACKGROUND_ATTACHMENT attachment;
        NH_CSS_BACKGROUND_BLEND_MODE blendMode; 
        NH_CSS_BACKGROUND_CLIP clip;            
        float color_p[4];                      
        Nh_List Images;
    } NH_CSS_Background;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_setDefaultBackgroundProperties(
        Nh_Window *Window_p, Nh_HTML_Node *Node_p
    );
    
    void Nh_CSS_setDefaultBackgroundProperty(
        Nh_Window *Window_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p, NH_CSS_PROPERTY type
    );
    
    NH_RESULT Nh_CSS_computeBackgroundProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp
    );

/** @} */

#endif 
