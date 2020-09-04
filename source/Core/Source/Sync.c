// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Sync.h"
#include "../Header/Time.h"
#include "../Header/Macros.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>
#include <limits.h>

// DECLARE =========================================================================================

static void forceAtomicRecording(
    Nh_Window *Window_p
);

static bool doSync(
    int ownerId
);

// COMMANDBUFFER ===================================================================================

void Nh_syncRecordCommandBufferBegin(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Tab_p->Sync.atomic = Tab_p->Sync.atomicRecording;

NH_SILENT_END()
}

void Nh_syncRecordCommandBufferEnd(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    Tab_p->Sync.render = true; // TODO unnecessary?

    // sync with window
    if (doSync(Tab_p->Window_p->Internal.ownerId) 
    &&  Tab_p->Sync.atomic) 
    {
        Tab_p->Sync.atomicRecording = false;
        Tab_p->Sync.atomic = false;
        while (Tab_p->Window_p->Sync.resize) {} // wait in case of resize
    }

NH_SILENT_END()
}

void Nh_syncUseCommandBuffer(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    // sync with tab (primarily command buffer creation)
    if (doSync(Tab_p->Sync.ownerId)) {
        Tab_p->Sync.atomicRecording = true;
        while (Tab_p->Sync.atomicRecording && Tab_p->Sync.render) {};
    }

NH_SILENT_END()
}

// WINDOW ==========================================================================================

void Nh_syncRenderBegin(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Window_p->Sync.rendering = true;
    Window_p->Internal.atomic = Window_p->Sync.atomicRender;

NH_SILENT_END()
}

void Nh_syncRenderEnd(
    Nh_Window *Window_p)
{
NH_BEGIN()

    if (Window_p->Internal.atomic) {
        Window_p->Internal.atomic = false;
        Window_p->Sync.atomicRender = false;
    }
    Window_p->Sync.rendering = false;

NH_SILENT_END()
}

void Nh_syncResizeBegin(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Window_p->Sync.resize = true;
    forceAtomicRecording(Window_p);

NH_SILENT_END()
}

void Nh_syncResizeEnd(
    Nh_Window *Window_p)
{
NH_BEGIN()

    Window_p->Sync.resize = false;
    forceAtomicRecording(Window_p);
    Window_p->Internal.skipRender = Nh_getThread()->Info.isVirtual;

NH_SILENT_END()
}

void Nh_syncWaitForWindow(
    Nh_Window *Window_p)
{
NH_BEGIN()

    if (doSync(Window_p->Internal.ownerId)) {
        while (Window_p->Sync.ready) {}
    }

NH_SILENT_END()
}

void Nh_syncHaltWindow(
    Nh_Window *Window_p)
{
NH_BEGIN()

    while (!Window_p->Sync.undoHalt) {
        Window_p->Sync.halt = false; // signal that window is on halt
        NH_sleepMs(Nh_getThread()->Timing.haltDelay);
    }

NH_SILENT_END()
}

// HELPER ==========================================================================================

static bool doSync(
    int ownerId)
{
NH_BEGIN()

    bool sync = false;

    Nh_Thread *Thread_p = Nh_getThread();
    
    if (Thread_p->id != ownerId) // executed by different threads
    {
        Nh_Thread *Owner_p = &Nh_getThreadPool()->Threads_p[ownerId];
        sync = Thread_p->Info.owner == -1 && Owner_p->Info.owner == -1;

        if (!sync) {
            sync = Owner_p->Info.owner != Thread_p->id && Owner_p->Info.owner != Thread_p->Info.owner;
        }
    }

NH_END(sync)
}

static void forceAtomicRecording(
    Nh_Window *Window_p)
{
NH_BEGIN()

    for (int i = 0; i < Window_p->Tabs.count; ++i) 
    {
        Nh_Tab *Tab_p = Nh_getTab(Window_p, i);
        
        if (doSync(Tab_p->Sync.ownerId)) 
        {
            Tab_p->Sync.atomicRecording = true;
            while (Tab_p->Sync.atomicRecording) {}
        }
    }

NH_SILENT_END()
}

