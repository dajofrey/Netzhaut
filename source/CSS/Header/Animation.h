#ifndef NH_CSS_ANIMATION_H
#define NH_CSS_ANIMATION_H

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

    typedef enum NH_CSS_ANIMATION_DIRECTION {
        NH_CSS_ANIMATION_DIRECTION_NORMAL,            
        NH_CSS_ANIMATION_DIRECTION_REVERSE,           
        NH_CSS_ANIMATION_DIRECTION_ALTERNATE,         
        NH_CSS_ANIMATION_DIRECTION_ALTERNATE_REVERSE, 
    } NH_CSS_ANIMATION_DIRECTION;
    
    typedef enum NH_CSS_ANIMATION_FILL_MODE {
        NH_CSS_ANIMATION_FILL_MODE_NONE,      
        NH_CSS_ANIMATION_FILL_MODE_FORWARDS,  
        NH_CSS_ANIMATION_FILL_MODE_BACKWARDS, 
        NH_CSS_ANIMATION_FILL_MODE_BOTH,      
    } NH_CSS_ANIMATION_FILL_MODE;
    
    typedef enum NH_CSS_ANIMATION_PLAY_STATE {
        NH_CSS_ANIMATION_PLAY_STATE_PAUSED,  
        NH_CSS_ANIMATION_PLAY_STATE_RUNNING, 
    } NH_CSS_ANIMATION_PLAY_STATE;
    
    typedef enum NH_CSS_ANIMATION_TIMING_FUNCTION {
        NH_CSS_ANIMATION_TIMING_FUNCTION_LINEAR,       	
        NH_CSS_ANIMATION_TIMING_FUNCTION_EASE,         	
        NH_CSS_ANIMATION_TIMING_FUNCTION_EASE_IN,      	
        NH_CSS_ANIMATION_TIMING_FUNCTION_EASE_OUT,     	
        NH_CSS_ANIMATION_TIMING_FUNCTION_EASE_IN_OUT,  	
        NH_CSS_ANIMATION_TIMING_FUNCTION_STEP_START,   	
        NH_CSS_ANIMATION_TIMING_FUNCTION_STEP_END,     	
        NH_CSS_ANIMATION_TIMING_FUNCTION_STEPS,        	
        NH_CSS_ANIMATION_TIMING_FUNCTION_CUBIC_BEZIER, 	
    } NH_CSS_ANIMATION_TIMING_FUNCTION;
    
/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_Animation {
        float duration;                                      
        float delay;                                         
        int iterationCount;                                 
        char *name_p;                                        
        NH_CSS_ANIMATION_DIRECTION direction;                
        NH_CSS_ANIMATION_FILL_MODE fillMode;                 
        NH_CSS_ANIMATION_PLAY_STATE playState;               
        NH_CSS_ANIMATION_TIMING_FUNCTION timingFunctionType; 
        // TODO TimingFunction
    } NH_CSS_Animation;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_setDefaultAnimationProperties(
        Nh_HTML_Node *Node_p
    );
    
    void Nh_CSS_setDefaultAnimationProperty(
        Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type
    );
    
    NH_RESULT Nh_CSS_computeAnimationProperties(
        Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp
    );

/** @} */

#endif
