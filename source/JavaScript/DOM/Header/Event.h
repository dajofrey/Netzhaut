/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_EVENT_H
#define NH_JS_EVENT_H

#include "../../Header/Data.h"
#include "../../Header/Interpreter.h"
#include "../../Header/Variable.h"

#include "../../../Core/Header/System.h"
#include "../../../Core/Header/Tab.h"
#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/List.h"

/** @addtogroup Enums
 *  @{
 */

    typedef enum NH_JS_EVENT {
        NH_JS_EVENT_ABORT,
        NH_JS_EVENT_AFTER_PRINT,
        NH_JS_EVENT_ANIMATION_CANCEL,
        NH_JS_EVENT_ANIMATION_END,
        NH_JS_EVENT_ANIMATION_ITERATION,
        NH_JS_EVENT_ANIMATION_START,
        NH_JS_EVENT_APP_INSTALLED,
        NH_JS_EVENT_AUDIO_PROCESS,
        NH_JS_EVENT_AUDIO_END,
        NH_JS_EVENT_AUDIO_START,
        NH_JS_EVENT_BEFORE_PRINT, 
        NH_JS_EVENT_BEFORE_UNLOAD, 	
        NH_JS_EVENT_BEGIN_EVENT,	
        NH_JS_EVENT_BLOCKED,
        NH_JS_EVENT_BLUR,
        NH_JS_EVENT_BOUNDARY, 	
        NH_JS_EVENT_CAN_PLAY,
        NH_JS_EVENT_CAN_PLAY_THROUGH, 	
        NH_JS_EVENT_CHANGE, 
        NH_JS_EVENT_CHARGING_CHANGE,
        NH_JS_EVENT_CHARGING_TIME_CHANGE,
        NH_JS_EVENT_CLICK,
        NH_JS_EVENT_CLOSE,
        NH_JS_EVENT_COMPLETE,
        NH_JS_EVENT_COMPOSITION_END, 
        NH_JS_EVENT_COMPOSITION_START,
        NH_JS_EVENT_COMPOSITION_UPDATE,
        NH_JS_EVENT_CONTEXT_MENU, 
        NH_JS_EVENT_COPY,
        NH_JS_EVENT_CUT,
        NH_JS_EVENT_DBL_CLICK, 	
        NH_JS_EVENT_DEVICE_CHANGE, 
        NH_JS_EVENT_DEVICE_MOTION, 
        NH_JS_EVENT_DEVICE_ORIENTATION,    
        NH_JS_EVENT_DISCHARGING_TIME_CHANGE,
        NH_JS_EVENT_DOM_ACTIVATE,
        NH_JS_EVENT_DOM_ATTRIBUTE_NAME_CHANGED,
        NH_JS_EVENT_DOM_ATTR_MODIFIED, 
        NH_JS_EVENT_DOM_CHARACTER_DATA_MODIFIED,
        NH_JS_EVENT_DOM_CONTENT_LOADED,      
        NH_JS_EVENT_DOM_ELEMENT_NAME_CHANGED,
        NH_JS_EVENT_DOM_FOCUS_IN,  
        NH_JS_EVENT_DOM_FOCUS_OUT,
        NH_JS_EVENT_DOM_NODE_INSERTED,
        NH_JS_EVENT_DOM_NODE_INSERTED_INTO_DOCUMENT,
        NH_JS_EVENT_DOM_NODE_REMOVED, 
        NH_JS_EVENT_DOM_NODE_REMOVED_FROM_DOCUMENT,
        NH_JS_EVENT_DOM_SUBTREE_MODIFIED,
        NH_JS_EVENT_DRAG,
        NH_JS_EVENT_DRAG_END,
        NH_JS_EVENT_DRAG_ENTER,
        NH_JS_EVENT_DRAG_LEAVE,
        NH_JS_EVENT_DRAG_OVER, 
        NH_JS_EVENT_DRAG_START,
        NH_JS_EVENT_DROP,
        NH_JS_EVENT_DURATION_CHANGE, 	
        NH_JS_EVENT_EMPTIED, 	
        NH_JS_EVENT_END, 
        NH_JS_EVENT_ENDED, 
        NH_JS_EVENT_END_EVENT,
        NH_JS_EVENT_ERROR, 	
        NH_JS_EVENT_FOCUS, 
        NH_JS_EVENT_FOCUS_IN, 
        NH_JS_EVENT_FOCUS_OUT,
        NH_JS_EVENT_FULLSCREEN_CHANGE, 	
        NH_JS_EVENT_FULLSCREEN_ERROR, 	
        NH_JS_EVENT_GAMEPAD_CONNECTED, 	
        NH_JS_EVENT_GAMEPAD_DISCONNECTED, 	
        NH_JS_EVENT_GOT_POINTER_CAPTURE, 	
        NH_JS_EVENT_HASH_CHANGE,
        NH_JS_EVENT_LOST_POINTER_CAPTURE,
        NH_JS_EVENT_INPUT,
        NH_JS_EVENT_INVALID, 
        NH_JS_EVENT_KEY_DOWN, 
        NH_JS_EVENT_KEY_PRESS,
        NH_JS_EVENT_KEY_UP,
        NH_JS_EVENT_LANGUAGE_CHANGE,
        NH_JS_EVENT_LEVEL_CHANGE, 
        NH_JS_EVENT_LOAD, 	          
        NH_JS_EVENT_LOADED_DATA, 	  
        NH_JS_EVENT_LOADED_META_DATA, 	  
        NH_JS_EVENT_LOAD_END, 	
        NH_JS_EVENT_LOAD_START, 	  
        NH_JS_EVENT_MARK, 	       
        NH_JS_EVENT_MESSAGE, 	  
        NH_JS_EVENT_MESSAGE_ERROR, 	  
        NH_JS_EVENT_MOUSE_DOWN, 	  
        NH_JS_EVENT_MOUSE_ENTER, 	  
        NH_JS_EVENT_MOUSE_LEAVE, 	  
        NH_JS_EVENT_MOUSE_MOVE, 	  
        NH_JS_EVENT_MOUSE_OUT, 	  
        NH_JS_EVENT_MOUSE_OVER, 	  
        NH_JS_EVENT_MOUSE_UP, 	  
        NH_JS_EVENT_NO_MATCH, 	  
        NH_JS_EVENT_NOTIFICATION_CLICK, 
        NH_JS_EVENT_OFFLINE, 	  
        NH_JS_EVENT_ONLINE, 	         
        NH_JS_EVENT_OPEN, 	          
        NH_JS_EVENT_ORIENTATION_CHANGE, 
        NH_JS_EVENT_PAGE_HIDE, 	  
        NH_JS_EVENT_PAGE_SHOW, 	  
        NH_JS_EVENT_PASTE, 	         
        NH_JS_EVENT_PAUSE, 	          
        NH_JS_EVENT_POINTER_CANCEL, 	  
        NH_JS_EVENT_POINTER_DOWN, 	  
        NH_JS_EVENT_POINTER_ENTER, 	  
        NH_JS_EVENT_POINTER_LEAVE, 	  
        NH_JS_EVENT_POINTER_LOCK_CHANGE, 
        NH_JS_EVENT_POINTER_LOCK_ERROR, 
        NH_JS_EVENT_POINTER_MOVE, 	  
        NH_JS_EVENT_POINTER_OUT, 	 
        NH_JS_EVENT_POINTER_OVER, 	 
        NH_JS_EVENT_POINTER_UP, 	  
        NH_JS_EVENT_PLAY,     	  
        NH_JS_EVENT_PLAYING,    	  
        NH_JS_EVENT_POP_STATE, 	  
        NH_JS_EVENT_PROGRESS, 	  
        NH_JS_EVENT_PUSH, 	          
        NH_JS_EVENT_PUSH_SUBSCRIPTION_CHANGE, 	 
        NH_JS_EVENT_RATE_CHANGE, 	         
        NH_JS_EVENT_READY_STATE_CHANGE, 	
        NH_JS_EVENT_REPEAT_EVENT, 	         
        NH_JS_EVENT_RESET, 	                 
        NH_JS_EVENT_RESIZE, 	                 
        NH_JS_EVENT_RESOURCE_TIMING_BUFFER_FULL,
        NH_JS_EVENT_RESULT, 	                
        NH_JS_EVENT_RESUME, 	                
        NH_JS_EVENT_SCROLL, 	               
        NH_JS_EVENT_SEEKED, 	                 
        NH_JS_EVENT_SEEKING, 	         
        NH_JS_EVENT_SELECT,   	         
        NH_JS_EVENT_SELECT_START, 	         
        NH_JS_EVENT_SELECTION_CHANGE, 
        NH_JS_EVENT_SHOW, 	        
        NH_JS_EVENT_SLOT_CHANGE, 	 
        NH_JS_EVENT_SOUND_END, 	
        NH_JS_EVENT_SOUND_START, 	
        NH_JS_EVENT_SPEECH_END, 	 
        NH_JS_EVENT_SPEECH_START, 	
        NH_JS_EVENT_STALLED, 	
        NH_JS_EVENT_START, 	        
        NH_JS_EVENT_STORAGE, 	
        NH_JS_EVENT_SUBMIT,           
        NH_JS_EVENT_SUCCESS, 	 
        NH_JS_EVENT_SUSPEND, 	 
        NH_JS_EVENT_SVG_ABORT, 	 
        NH_JS_EVENT_SVG_ERROR, 	 
        NH_JS_EVENT_SVG_LOAD, 	 
        NH_JS_EVENT_SVG_RESIZE, 	 
        NH_JS_EVENT_SVG_SCROLL, 	 
        NH_JS_EVENT_SVG_UNLOAD, 	 
        NH_JS_EVENT_SVG_ZOOM, 	 
        NH_JS_EVENT_TIME_OUT, 	 
        NH_JS_EVENT_TIME_UPDATE, 	 
        NH_JS_EVENT_TOUCH_CANCEL, 
        NH_JS_EVENT_TOUCH_END, 	 
        NH_JS_EVENT_TOUCH_MOVE, 	 
        NH_JS_EVENT_TOUCH_START, 	 
        NH_JS_EVENT_TRANSITION_END, 	 
        NH_JS_EVENT_UNLOAD, 	         
        NH_JS_EVENT_UPGRADE_NEEDED, 	 
        NH_JS_EVENT_USER_PROXIMITY, 	 
        NH_JS_EVENT_VOICES_CHANGED, 	 
        NH_JS_EVENT_VERSION_CHANGE, 	 
        NH_JS_EVENT_VISIBILITY_CHANGE,
        NH_JS_EVENT_VOLUME_CHANGE, 	 
        NH_JS_EVENT_WAITING, 	 
        NH_JS_EVENT_WHEEL, 	        
    } NH_JS_EVENT; 

/** @} */

/** @addtogroup Structs
 *  @{
 */

    typedef struct Nh_JS_Event {
        NH_JS_EVENT type;
        char *type_p;
        Nh_JS_Variable Target;
    } Nh_JS_Event;

/** @} */

/** @addtogroup Functions
 *  @{
 */

// API

    Nh_JS_Result NH_JS_EVENT_Event(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result NH_JS_EVENT_bubbles(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_cancelBubble(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_cancelable(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_composed(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_currentTarget(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_deepPath(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_defaultPrevented(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_eventPhase(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_explicitOriginalTarget(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_originalTarget(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_returnValue(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_srcElement(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_target(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_timeStamp(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_type(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_isTrusted(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_createEvent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_composedPath(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_initEvent(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_preventDefault(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_stopImmediatePropagation(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_EVENT_stopPropagation(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

// INTERNAL

    NH_RESULT Nh_JS_createEventObject(
        Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_JS_Event *Event_p
    );

    NH_RESULT Nh_JS_destroyEventObject(
        Nh_JS_Object *Object_p
    );

    NH_RESULT Nh_JS_broadcastEvent(
        Nh_Tab *Tab_p, Nh_JS_Object *Object_p
    );

    NH_RESULT Nh_JS_broadcastInternalEvent(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_JS_EVENT type, char *type_p, ...
    );

    char *Nh_JS_stringifyEventData(
        Nh_JS_Event *Event_p, bool newline
    );

/** @} */

#endif 

