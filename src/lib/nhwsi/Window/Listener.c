// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Listener.h"
#include "Window.h"

#include "../Common/Macros.h"
#include "../Platforms/X11/Window.h"

#include "../../nhcore/System/Thread.h"
#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/String.h"
#include "../../nhcore/Util/LinkedList.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// VARS ============================================================================================

static nh_wsi_Listener NH_WSI_LISTENER;

// NORMALIZE ============================================================================================

NH_WSI_RESULT_E nh_wsi_normalizeListener()
{
NH_WSI_BEGIN()

    memset(&NH_WSI_LISTENER, 0, sizeof(nh_wsi_Listener));
    NH_WSI_LISTENER.Windows = nh_core_initLinkedList();

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS)
}

// RUN =============================================================================================

static NH_WSI_RESULT_E nh_wsi_getInput(
    nh_wsi_Window *Window_p, NH_BOOL *idle_p)
{
NH_WSI_BEGIN()

    switch (Window_p->type) {
        case NH_WSI_TYPE_X11 : NH_WSI_DIAGNOSTIC_END(nh_x11_getInput(Window_p, idle_p))
    }

NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_BAD_STATE)
}

static void *nh_wsi_initListener(
    nh_core_Workload *Workload_p)
{
NH_WSI_BEGIN()

    static NH_BYTE *name_p = "WSI Listener";
    static NH_BYTE *path_p = "nhwsi/Main/Listener.c";
    Workload_p->name_p = name_p;
    Workload_p->path_p = path_p;
    Workload_p->module = NH_MODULE_WSI;

NH_WSI_END(&NH_WSI_LISTENER)
}

static NH_SIGNAL nh_wsi_runListener(
    void *args_p) 
{
NH_WSI_BEGIN()

    // As soon as there are no windows anymore, we close the window listener.
    // The listener will be started automatically when a new window is created and no listener exists.
    if (NH_WSI_LISTENER.Windows.count == 0) {
        NH_WSI_LISTENER.running = NH_FALSE;
        NH_WSI_END(NH_SIGNAL_FINISH)
    }

    NH_BOOL idle = NH_TRUE;
    NH_WSI_LISTENER.Clipboard.updated = NH_FALSE;

    for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
        if (nh_wsi_getInput(nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, i), &idle) != NH_WSI_SUCCESS) {
            NH_WSI_END(NH_SIGNAL_ERROR)
        }
    }

NH_WSI_END(idle ? NH_SIGNAL_IDLE : NH_SIGNAL_OK)
}

static void nh_wsi_terminateListener(
    void *args_p)
{
NH_WSI_BEGIN()

    for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
        nh_wsi_destroyWindow(nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, i));
    }

    nh_wsi_freeClipboard();

NH_WSI_SILENT_END()
}

NH_WSI_RESULT_E nh_wsi_enableWindowListener(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    if (!NH_WSI_LISTENER.running) {
        nh_core_activateWorkload(nh_wsi_initListener, nh_wsi_runListener, nh_wsi_terminateListener, NULL, NULL, NH_TRUE);
        NH_WSI_LISTENER.running = NH_TRUE;
    }
 
    nh_core_appendToLinkedList(&NH_WSI_LISTENER.Windows, Window_p);

NH_WSI_END(NH_WSI_SUCCESS)
}

NH_WSI_RESULT_E nh_wsi_disableWindowListener(
    nh_wsi_Window *Window_p)
{
NH_WSI_BEGIN()

    nh_core_removeFromLinkedList2(&NH_WSI_LISTENER.Windows, Window_p, NH_FALSE);

    // Listener will stop automatically once there are no more windows in the list.

NH_WSI_END(NH_WSI_SUCCESS)
}

// CLIPBOARD =======================================================================================

void nh_wsi_freeClipboard()
{
    if (NH_WSI_LISTENER.Clipboard.data_p) {
        nh_core_free(NH_WSI_LISTENER.Clipboard.data_p);
    }
    NH_WSI_LISTENER.Clipboard.data_p = NULL;
}

NH_WSI_RESULT_E nh_wsi_setClipboard(
    NH_BYTE *utf8_p, int size, NH_BOOL selfOwned)
{
NH_WSI_BEGIN()

    nh_wsi_freeClipboard();

    NH_WSI_LISTENER.Clipboard.data_p = nh_core_allocate(sizeof(NH_BYTE)*(size+1));
    NH_WSI_CHECK_MEM(NH_WSI_LISTENER.Clipboard.data_p)

    strcpy(NH_WSI_LISTENER.Clipboard.data_p, utf8_p);

    if (NH_WSI_LISTENER.Windows.count > 0 && selfOwned) {
        nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, 0);
        switch (Window_p->type) {
            case NH_WSI_TYPE_X11 : NH_WSI_CHECK(nh_x11_setClipboardOwner(&Window_p->X11)) break;
        }
    }

    NH_WSI_LISTENER.Clipboard.updated = NH_TRUE;

NH_WSI_END(NH_WSI_SUCCESS)
}

// TODO sync
NH_BYTE *nh_wsi_getClipboard()
{
NH_WSI_BEGIN()

    if (NH_WSI_LISTENER.Windows.count <= 0) {NH_WSI_END(NULL)}

    // Check if self owned.
    NH_BOOL selfOwned = NH_FALSE;
    for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
        nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, 0);
        switch (Window_p->type) {
            case NH_WSI_TYPE_X11 : selfOwned = nh_x11_isClipboardOwner(&Window_p->X11); break; 
        }
    } 
 
    if (selfOwned) {
        NH_WSI_END(NH_WSI_LISTENER.Clipboard.data_p)
    }

    nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, 0);
    NH_WSI_LISTENER.Clipboard.updated = NH_FALSE;
    switch (Window_p->type) {
        case NH_WSI_TYPE_X11 : NH_WSI_CHECK_2(NULL, nh_x11_requestClipboardConversion(&Window_p->X11)) break;
    }

    // This may be forever blocking if the program that holds the clipboard does not respond. So we use time limitation.
    nh_SystemTime Begin = nh_core_getSystemTime();
    while (!NH_WSI_LISTENER.Clipboard.updated && nh_core_getSystemTimeDiffInSeconds(Begin, nh_core_getSystemTime()) < 1.0f) {
        nh_core_executeWorkload(&NH_WSI_LISTENER);
    }

NH_WSI_END(NH_WSI_LISTENER.Clipboard.updated ? NH_WSI_LISTENER.Clipboard.data_p : NULL)
}

