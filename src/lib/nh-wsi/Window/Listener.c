// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Listener.h"
#include "Window.h"

#if defined(__unix__)
    #include "../Platforms/X11/Window.h"
#elif defined(__APPLE__)
    #include "../Platforms/Cocoa/Window.h"
#endif

#include "../../nh-core/System/Thread.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/String.h"
#include "../../nh-core/Util/LinkedList.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// VARS ============================================================================================

static nh_wsi_Listener NH_WSI_LISTENER;

// NORMALIZE ============================================================================================

NH_API_RESULT nh_wsi_normalizeListener()
{
    memset(&NH_WSI_LISTENER, 0, sizeof(nh_wsi_Listener));
    NH_WSI_LISTENER.Windows = nh_core_initLinkedList();

    return NH_API_SUCCESS;
}

// RUN =============================================================================================

static NH_API_RESULT nh_wsi_getInput(
    nh_wsi_Window *Window_p, bool *idle_p)
{
    switch (Window_p->type) {
#if defined(__unix__)
        case NH_WSI_TYPE_X11   : return nh_wsi_getX11Input(Window_p, idle_p);
#elif defined(__APPLE__)
        case NH_WSI_TYPE_COCOA : return nh_wsi_getCocoaInput(Window_p, idle_p);
#endif
        default                : return NH_API_ERROR_BAD_STATE;
    }
}

static void *nh_wsi_initListener(
    nh_core_Workload *Workload_p)
{
    static char *name_p = "WSI Listener";
    static char *path_p = "nh-wsi/Window/Listener.c";
    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_WSI;

    return &NH_WSI_LISTENER;
}

static NH_SIGNAL nh_wsi_runListener(
    void *args_p) 
{
puts("hello");
    // As soon as there are no windows anymore, we close the window listener.
    // The listener will be started automatically when a new window is created and no listener exists.
    if (NH_WSI_LISTENER.Windows.count == 0) {
        NH_WSI_LISTENER.running = false;
        return NH_SIGNAL_FINISH;
    }

    bool idle = true;
    NH_WSI_LISTENER.Clipboard.updated = false;

    for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
        if (nh_wsi_getInput(nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, i), &idle) != NH_API_SUCCESS) {
            return NH_SIGNAL_ERROR;
        }
    }

    return idle ? NH_SIGNAL_IDLE : NH_SIGNAL_OK;
}

static void nh_wsi_terminateListener(
    void *args_p)
{
    for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
        nh_wsi_destroyWindow(nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, i));
    }

    nh_wsi_freeClipboard();

    return;
}

NH_API_RESULT nh_wsi_enableWindowListener(
    nh_wsi_Window *Window_p)
{
    if (!NH_WSI_LISTENER.running) {
        nh_core_activateWorkload(nh_wsi_initListener, nh_wsi_runListener, nh_wsi_terminateListener, NULL, NULL, true);
        NH_WSI_LISTENER.running = true;
    }

    nh_core_appendToLinkedList(&NH_WSI_LISTENER.Windows, Window_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_disableWindowListener(
    nh_wsi_Window *Window_p)
{
    nh_core_removeFromLinkedList2(&NH_WSI_LISTENER.Windows, Window_p, false);

    // Listener will stop automatically once there are no more windows in the list.

    return NH_API_SUCCESS;
}

// CLIPBOARD =======================================================================================

void nh_wsi_freeClipboard()
{
    if (NH_WSI_LISTENER.Clipboard.data_p) {
        nh_core_free(NH_WSI_LISTENER.Clipboard.data_p);
    }
    NH_WSI_LISTENER.Clipboard.data_p = NULL;
}

NH_API_RESULT nh_wsi_setClipboard(
    char *utf8_p, int size, bool selfOwned)
{
    nh_wsi_freeClipboard();

    NH_WSI_LISTENER.Clipboard.data_p = nh_core_allocate(sizeof(char)*(size+1));
    NH_CORE_CHECK_MEM(NH_WSI_LISTENER.Clipboard.data_p)

    strcpy(NH_WSI_LISTENER.Clipboard.data_p, utf8_p);

    if (NH_WSI_LISTENER.Windows.count > 0 && selfOwned) {
        nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, 0);
        switch (Window_p->type) {
            #if defined(__unix__)
                case NH_WSI_TYPE_X11 : NH_CORE_CHECK(nh_x11_setClipboardOwner(&Window_p->X11)) break;
            #elif defined(__APPLE__)
                case NH_WSI_TYPE_COCOA : NH_CORE_CHECK(nh_wsi_setCocoaClipboardOwner(&Window_p->Cocoa)) break;
            #endif
        }
    }

    NH_WSI_LISTENER.Clipboard.updated = true;

    return NH_API_SUCCESS;
}

// TODO sync
char *nh_wsi_getClipboard()
{
    if (NH_WSI_LISTENER.Windows.count <= 0) {return NULL;}

    // Check if self owned.
    bool selfOwned = false;
    for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
        nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, 0);
        switch (Window_p->type) {
            #if defined(__unix__)
                case NH_WSI_TYPE_X11 : selfOwned = nh_x11_isClipboardOwner(&Window_p->X11); break; 
            #elif defined(__APPLE__)
                case NH_WSI_TYPE_COCOA : selfOwned = nh_wsi_isCocoaClipboardOwner(&Window_p->Cocoa); break;
            #endif
        }
    } 
 
    if (selfOwned) {
        return NH_WSI_LISTENER.Clipboard.data_p;
    }

    nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, 0);
    NH_WSI_LISTENER.Clipboard.updated = false;
    switch (Window_p->type) {
        #if defined(__unix__)
            case NH_WSI_TYPE_X11 : NH_CORE_CHECK_2(NULL, nh_x11_requestClipboardConversion(&Window_p->X11)) break;
        #elif defined(__APPLE__)
            case NH_WSI_TYPE_COCOA : NH_CORE_CHECK_2(NULL, nh_wsi_requestCocoaClipboardConversion(&Window_p->Cocoa)) break;
        #endif
    }

    // This may be forever blocking if the program that holds the clipboard does not respond. So we use time limitation.
    nh_core_SystemTime Begin = nh_core_getSystemTime();
    while (!NH_WSI_LISTENER.Clipboard.updated && nh_core_getSystemTimeDiffInSeconds(Begin, nh_core_getSystemTime()) < 1.0f) {
        nh_core_executeWorkload(&NH_WSI_LISTENER);
    }

    return NH_WSI_LISTENER.Clipboard.updated ? NH_WSI_LISTENER.Clipboard.data_p : NULL;
}

