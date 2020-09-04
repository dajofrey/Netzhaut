#ifndef NH_SYNC_H
#define NH_SYNC_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tab.h"

#endif

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    void Nh_syncRecordCommandBufferBegin(
        Nh_Tab *Tab_p
    );
    
    void Nh_syncRecordCommandBufferEnd(
        Nh_Tab *Tab_p
    );
    
    void Nh_syncUseCommandBuffer(
        Nh_Tab *Tab_p
    );
    
    void Nh_syncRenderBegin(
        Nh_Window *Window_p
    );
    
    void Nh_syncRenderEnd(
        Nh_Window *Window_p
    );
    
    void Nh_syncResizeBegin(
        Nh_Window *Window_p
    );
    
    void Nh_syncResizeEnd(
        Nh_Window *Window_p
    );
    
    void Nh_syncWaitForWindow(
        Nh_Window *Window_p
    );
    
    void Nh_syncHaltWindow(
        Nh_Window *Window_p
    );

/** @} */

#endif
