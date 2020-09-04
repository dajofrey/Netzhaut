#ifndef NH_TAB_H
#define NH_TAB_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Window.h"
#include "URI.h"

#include "../../HTML/Header/Document.h"
#include "../../JavaScript/Header/Script.h"
#include "../../IO/Header/Input.h"

#endif

/** @addtogroup CoreEnums Enums
 *  \ingroup Core
 *  @{
 */

    typedef enum NH_CLONE_BEHAVIOR {
        NH_CLONE_BEHAVIOR_UNIQUE,
        NH_CLONE_BEHAVIOR_MIRROR,
        NH_CLONE_BEHAVIOR_UNDEFINED,
    } NH_CLONE_BEHAVIOR;

/** @} */

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_Tab {

        Nh_Window *Window_p;       /**<A pointer to the window which contains the tab.*/
        struct Nh_Tab *Original_p; /**<A pointer to the original tab. Is NULL when the tab isn't a clone.*/
       
        Nh_HTML_Document Document; /**<The HTML-Document which is displayed in the tab.*/          

        Nh_List Textures;  
        Nh_List Callbacks; 

        struct {
            int priority;              /**<Defines the rendering order. 0 means highest priority aka the front tab. Lesser priorities ascend to +infinity.*/
            bool isClone;         
            int pxSize_p[2];           /**<Pixel values of the tab-width and the tab-height, in that order.*/ 
            float relativeOffset_p[2]; /**<From 0 (0%) to 1 (100&). Relative to the upper left corner of the window.*/ 
            int pxScrollOffset_p[2];
        } Info;

        struct {
            int newestBuffers_p[2];    
            int submittedBuffers_p[2]; 
            int ownerId;
            bool atomicRecording;      
            bool render;              
            bool atomic;
        } Sync;

        struct {
            bool alwaysOnTop;    /**<Tab will remain on top of the tab stacking order when true.*/
            bool update;         /**<Tab will update its state when true.*/
            bool crop;           /**<Tab will be rendered using a cropped view when true.*/
            bool close;          /**<Tab will close as soon as possible when true.*/
            bool updateGraphics;
        } Flags;

        struct {
            VkCommandBuffer CommandBuffers_p[4];
        } Vulkan;     

    } Nh_Tab;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    NH_RESULT Nh_startTabThread(
        Nh_Window *Window_p, char *documentURI_p, int pxSize_p[2]
    );

    NH_RESULT Nh_runTab(
    );
   
    NH_RESULT NH_reloadTab(
        Nh_Tab *Tab_p, char *documentURI_p 
    );
    
    NH_RESULT Nh_closeTab(
        Nh_Tab *Window_p
    );
    
    NH_RESULT Nh_focusTab(
        Nh_Tab *Tab_p
    );
    
    bool Nh_isForemostTab(
        Nh_Tab *Window_p, int position_p[2]
    );
    
    void Nh_getTabOffset(
        Nh_Tab *Tab_p, int xy_p[2]
    );
    
    Nh_Tab *Nh_getTab(
        Nh_Window *Window_p, int index
    );

/** @} */

#endif
