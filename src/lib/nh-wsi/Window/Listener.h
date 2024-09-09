#ifndef NH_WSI_WINDOW_LISTENER_H
#define NH_WSI_WINDOW_LISTENER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nh-core/System/Thread.h"

#endif

/** @addtogroup lib_nh-wsi_structs
 *  @{
 */

    typedef struct nh_wsi_Clipboard {
        NH_BOOL updated;
        NH_BYTE *data_p; 
    } nh_wsi_Clipboard;

    typedef struct nh_wsi_Listener {
        NH_BOOL running;
        nh_LinkedList Windows;
        nh_wsi_Clipboard Clipboard;
    } nh_wsi_Listener;

/** @} */

/** @addtogroup lib_nh-wsi_typedefs
 *  @{
 */

    typedef NH_WSI_RESULT_E (*nh_wsi_setClipboard_f)(
        NH_BYTE *utf8_p, int size, NH_BOOL selfOwned
    );

    typedef NH_BYTE *(*nh_wsi_getClipboard_f)(
    );

/** @} */

/** @addtogroup lib_nh-wsi_functions
 *  @{
 */
    
    NH_WSI_RESULT_E nh_wsi_normalizeListener(
    );

    NH_WSI_RESULT_E nh_wsi_enableWindowListener(
        nh_wsi_Window *Window_p
    );

    NH_WSI_RESULT_E nh_wsi_disableWindowListener(
        nh_wsi_Window *Window_p
    );

    void nh_wsi_freeClipboard(
    );

    NH_WSI_RESULT_E nh_wsi_setClipboard(
        NH_BYTE *utf8_p, int size, NH_BOOL selfOwned
    );

    NH_BYTE *nh_wsi_getClipboard(
    );

/** @} */

#endif // NH_WSI_WINDOW_LISTENER_H 
