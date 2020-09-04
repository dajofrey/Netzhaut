#ifndef NH_WINDOW_H
#define NH_WINDOW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    
/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "System.h"
#include "Thread.h"

#include "../../IO/Header/Input.h"
#include "../../IO/Header/Window.h"

#include "../../Graphics/Vulkan/Header/Surface.h"

#endif

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_Window {
        
        NH_IO_Window WSI;                             
        NH_IO_Input Input;                                
        Nh_GPU GPU;                     

        Nh_List Tabs;
 
        struct {
            bool inheritBackground;     
            int originalPxSize_p[2];    
            int pxSize_p[2];            
            float backgroundColor_p[4];
        } Info;

        struct {
            bool halt;      
            bool undoHalt;  
            bool rendering; 
            bool resize;   
            bool ready;    
            bool atomicRender;
        } Sync;

        struct {
            bool getNextImage;       
            bool skipRender;          
            float resizeFactors_p[2]; 
            bool atomic;
            int ownerId;
        } Internal;

        struct {
            bool close;
            bool getInput;
        } Flags;

        struct {
            Nh_Vk_Surface Surface;
        } Vulkan;

    } Nh_Window;           

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    Nh_Thread *Nh_startWindowThread(
        int pxSize_p[2]
    );

    NH_RESULT Nh_runWindow(
    );

    void Nh_setWindowBackgroundColor(
        Nh_Window *Window_p, float color_p[4]
    );
    
    NH_RESULT Nh_removeTab(
        Nh_Window *Window_p, void *Tab_p
    );
    
    NH_RESULT Nh_closeWindow(
        Nh_Window *Window_p
    );

/** @} */

#endif 
