// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Tab.h"
#include "../Header/Sync.h"
#include "../Header/Time.h"
#include "../Header/Memory.h"
#include "../Header/Callbacks.h"
#include "../Header/Macros.h"
#include "../Header/String.h"

#include "../../IO/Header/Mouse.h"
#include "../../IO/Header/Keyboard.h"

#include "../../HTML/Header/Document.h"
#include "../../CSS/Header/Box.h"
#include "../../Graphics/Header/Tab.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>

// DECLARE =========================================================================================

typedef struct Nh_TabArgs {
    char *documentURI_p; 
    int pxSize_p[2];     
    float relOffset_p[2]; /**<Offset from top-left window corner in relation to window size.*/ 
    Nh_Window *Window_p;  
} Nh_TabArgs;

static NH_RESULT Nh_initTab(
    Nh_Tab *Tab_p, Nh_Window *Window_p, int pxSize_p[2], float relativeOffset_p[2]
);
static NH_RESULT Nh_startTab(
    Nh_TabArgs *Args_p
);
static NH_RESULT Nh_updateTab(
    Nh_Tab *Tab_p
);
static bool NH_hitTab(
    Nh_Tab *Tab_p, int position_p[2]
);
static char *NH_generateTabName(
    Nh_Window *Window_p
);

// THREAD ==========================================================================================

NH_RESULT Nh_startTabThread(
    Nh_Window *Window_p, char *documentURI_p, int pxSize_p[2])
{
NH_BEGIN()

    Nh_TabArgs *Args_p = Nh_allocate(sizeof(Nh_TabArgs));
    NH_CHECK_MEM(Args_p)

    Args_p->Window_p       = Window_p;
    Args_p->pxSize_p[0]    = pxSize_p[0];
    Args_p->pxSize_p[1]    = pxSize_p[1];
    Args_p->relOffset_p[0] = 0.0f;
    Args_p->relOffset_p[1] = 0.0f;
    Args_p->documentURI_p  = documentURI_p;

    NH_CHECK_NULL(Nh_startThread(
        NH_THREAD_TAB, (NH_RESULT(*)(void *))Nh_startTab, (void*)Args_p
    ))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_startTab(
    Nh_TabArgs *Args_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Args_p, Args_p->Window_p, Args_p->pxSize_p, Args_p->relOffset_p, Nh_getThread())
    NH_CHECK_TYPE(Nh_getThread()->type, NH_THREAD_TAB)

    Nh_syncWaitForWindow(Args_p->Window_p);

    Nh_Tab *Tab_p = Nh_getThread()->Data_p;

    NH_CHECK(Nh_initTab(Tab_p, Args_p->Window_p, Args_p->pxSize_p, Args_p->relOffset_p))
    NH_CHECK(Nh_Gfx_createTab(Tab_p))
    NH_CHECK(Nh_HTML_createDocument(Args_p->documentURI_p, Tab_p))
    NH_CHECK(Nh_Gfx_initDefaultTextures(Tab_p))
    NH_CHECK(Nh_HTML_computeDocument(Tab_p, NH_FALSE))
    NH_CHECK(Nh_focusTab(Tab_p))
    NH_CHECK(Nh_addListItem(&Tab_p->Window_p->Tabs, Tab_p)) // TODO possible race condition

    Tab_p->Flags.update = true;

    if (!Nh_isVirtualThread()) {NH_CHECK(Nh_runTab())}

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_runTab()
{
NH_BEGIN()

    Nh_Thread *Thread_p = Nh_getThread();

    NH_CHECK_NULL(Thread_p)
    NH_CHECK_TYPE(Thread_p->type, NH_THREAD_TAB)

    Nh_Tab *Tab_p = Thread_p->Data_p;

    do {
        if (Thread_p->Flags.run)
        {
            NH_START_TAT()
            if (Tab_p->Flags.update) {
                NH_CHECK(Nh_updateTab(Tab_p))
                NH_RUN_DELAY()
            } 
            else {NH_IDLE_DELAY()}
            NH_STOP_TAT()
            NH_CHECK(Nh_runVirtualThreads())

        } else {NH_IDLE_DELAY()}

    } while (!Tab_p->Flags.close && !Thread_p->Info.isVirtual);

    if (Tab_p->Flags.close) {NH_CHECK(Nh_closeTab(Tab_p))}

NH_END(NH_SUCCESS)
}

// INIT ============================================================================================

static NH_RESULT Nh_initTab(
    Nh_Tab *Tab_p, Nh_Window *Window_p, int pxSize_p[2], float relativeOffset_p[2])
{
NH_BEGIN()

    Tab_p->Original_p = NULL;
    Tab_p->Window_p   = Window_p;

    Tab_p->Info.isClone = false;

    memcpy(Tab_p->Info.pxSize_p, pxSize_p, sizeof(int) * 2);
    memcpy(Tab_p->Info.relativeOffset_p, relativeOffset_p, sizeof(float) * 2);
    memset(Tab_p->Info.pxScrollOffset_p, 0, sizeof(int) * 2);

    Tab_p->Flags.alwaysOnTop = false;
    Tab_p->Flags.update      = false;
    Tab_p->Flags.close       = false;
    Tab_p->Flags.crop        = false;

    Tab_p->Sync.newestBuffers_p[0] = 0;
    Tab_p->Sync.newestBuffers_p[1] = 2;
    Tab_p->Sync.atomicRecording = false;
    Tab_p->Sync.render          = false;

    NH_INIT_LIST(Tab_p->Callbacks)
    NH_INIT_LIST(Tab_p->Textures)

    Tab_p->Sync.atomic  = false;
    Tab_p->Sync.ownerId = Nh_getThread()->id;
    Tab_p->Flags.updateGraphics = false;

NH_END(NH_SUCCESS)
}

// RELOAD ==========================================================================================

NH_RESULT NH_reloadTab(
    Nh_Tab *Tab_p, char *documentURI_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p)

    Tab_p->Flags.update = false; // TODO sync

    // destroy
    Nh_HTML_destroyDocument(&Tab_p->Document, &Tab_p->Window_p->GPU, NH_FALSE);
    NH_CHECK(Nh_Gfx_deleteTextures(Tab_p))

    Nh_removeTab(Tab_p->Window_p, Tab_p);

    // create
    NH_CHECK(Nh_HTML_createDocument(documentURI_p, Tab_p))
    NH_CHECK(Nh_Gfx_initDefaultTextures(Tab_p))
    NH_CHECK(Nh_HTML_computeDocument(Tab_p, NH_FALSE))

    NH_CHECK(Nh_addListItem(&Tab_p->Window_p->Tabs, Tab_p)) // TODO possible race condition
    NH_CHECK(Nh_focusTab(Tab_p))

    Tab_p->Flags.update = true;

NH_END(NH_SUCCESS)
}

// CLOSE ===========================================================================================

NH_RESULT Nh_closeTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    if (Nh_getThread()->Data_p != Tab_p) {NH_END(NH_ERROR_BAD_STATE)}

    Nh_HTML_destroyDocument(&Tab_p->Document, &Tab_p->Window_p->GPU, NH_FALSE);

    Nh_deleteCallbacks(Tab_p);
    NH_CHECK(Nh_Gfx_deleteTextures(Tab_p))

    Nh_Gfx_destroyTab(Tab_p);

    NH_CHECK(Nh_deactivateThread(NULL))

NH_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

static NH_RESULT Nh_updateTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_updateDocument(Tab_p))
    NH_CHECK(Nh_Gfx_updateTabRendering(Tab_p))

NH_END(NH_SUCCESS)
}

//static NH_RESULT NH_processInput(
//    Nh_Tab *Tab_p)
//{
//NH_BEGIN()
//
//// TODO to script
//    while (1) 
//    {
//        NH_IO_MouseEvent *MouseEvent_p = Nh_incrementRingIteratorMark(
//            &Tab_p->Window_p->Input.Mouse.Events, &Tab_p->Internal.Input.Mouse
//        );
//        if (MouseEvent_p == NULL) {break;}
//
//        if (MouseEvent_p->trigger == NH_IO_TRIGGER_PRESS && NH_hitTab(Tab_p, MouseEvent_p->position_p)) 
//        {
//            if (Nh_isForemostTab(Tab_p, MouseEvent_p->position_p) && Tab_p->Info.priority != 0) {
//                Nh_focusTab(Tab_p);
//            }
//        }
//    }
//
//NH_END(NH_SUCCESS)
//}

NH_RESULT Nh_focusTab(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p->Window_p)

    for (int i = 0; i < Tab_p->Document.Tree.Flat.Unformatted.count; ++i) 
    {
        Nh_HTML_Node *Node_p = Nh_getListItem(&Tab_p->Document.Tree.Flat.Unformatted, i); 
        if (Node_p->tag == NH_HTML_TAG_TITLE) {
            Nh_IO_setWindowTitle(&Tab_p->Window_p->WSI, Node_p->text_p);
        }
    }

    for (int i = 0; i < Tab_p->Window_p->Tabs.count; ++i) 
    {
        Nh_Tab *Other_p = Nh_getTab(Tab_p->Window_p, i);
        if (Other_p != Tab_p) {Other_p->Info.priority += 1;}
    }

    Tab_p->Info.priority = 0;

NH_END(NH_SUCCESS)
}

// CHECK ===========================================================================================

bool Nh_isForemostTab(
    Nh_Tab *Tab_p, int position_p[2])
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p->Window_p)

    int count = Tab_p->Window_p->Tabs.count;

    for (int i = 0; count > 0; ++i) 
    {
        Nh_Tab *Other_p = Nh_getTab(Tab_p->Window_p, i);
        
        if (Other_p != NULL) {
            if (NH_hitTab(Other_p, position_p)) {
                if (Other_p->Info.priority < Tab_p->Info.priority) {
                    NH_END(false)
                }
            }
            count--;
        }
    }

NH_END(true)
}

static bool NH_hitTab(
    Nh_Tab *Tab_p, int position_p[2])
{
NH_BEGIN()

    // TODO relativeOffset_p to pxOffset_p

NH_END(true)
}

// GET =============================================================================================

void Nh_getTabOffset(
    Nh_Tab *Tab_p, int xy_p[2])
{
NH_BEGIN()

    xy_p[0] = (Tab_p->Window_p->Info.pxSize_p[0] * Tab_p->Info.relativeOffset_p[0]);
    xy_p[1] = (Tab_p->Window_p->Info.pxSize_p[1] * Tab_p->Info.relativeOffset_p[1]);

NH_END()
}

Nh_Tab *Nh_getTab(
    Nh_Window *Window_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(&Window_p->Tabs, index))
}

