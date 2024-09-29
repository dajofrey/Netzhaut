#ifndef NH_WSI_WINDOW_LISTENER_H
#define NH_WSI_WINDOW_LISTENER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Window.h"
#include "../Common/Includes.h"
#include "../../nh-core/System/Thread.h"
#include "../../nh-core/Util/LinkedList.h"

#endif

/** @addtogroup lib_nh-wsi_structs
 *  @{
 */

    typedef struct nh_wsi_Clipboard {
        bool updated;
        char *data_p; 
    } nh_wsi_Clipboard;

    typedef struct nh_wsi_Listener {
        bool running;
        nh_LinkedList Windows;
        nh_wsi_Clipboard Clipboard;
    } nh_wsi_Listener;

/** @} */

/** @addtogroup lib_nh-wsi_typedefs
 *  @{
 */

    typedef NH_API_RESULT (*nh_wsi_setClipboard_f)(
        char *utf8_p, int size, bool selfOwned
    );

    typedef char *(*nh_wsi_getClipboard_f)(
    );

/** @} */

/** @addtogroup lib_nh-wsi_functions
 *  @{
 */
    
    NH_API_RESULT nh_wsi_normalizeListener(
    );

    NH_API_RESULT nh_wsi_enableWindowListener(
        nh_wsi_Window *Window_p
    );

    NH_API_RESULT nh_wsi_disableWindowListener(
        nh_wsi_Window *Window_p
    );

    void nh_wsi_freeClipboard(
    );

    NH_API_RESULT nh_wsi_setClipboard(
        char *utf8_p, int size, bool selfOwned
    );

    char *nh_wsi_getClipboard(
    );

/** @} */

#endif // NH_WSI_WINDOW_LISTENER_H 
