// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ==========================================================================================

#include "../Header/Netzhaut.h"
#include "../Header/Macros.h"

#include "../../CSS/Header/Parser.h"
#include "../../HTML/Header/Parser.h"
#include "../../JavaScript/Header/Parser.h"
#include "../../TTY/Header/Terminal.h"
#include "../../TTY/Header/Query.h"
#include "../../Network/Header/Network.h"

#include "../../Core/Header/URI.h"
#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/Window.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/File.h"
#include "../../Core/Header/Output.h"
#include "../../Core/Header/Config.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_CSS_UTILS
#include NH_HTML_UTILS
#include NH_JS_UTILS

#include <string.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/wait.h>
#endif

// INIT ============================================================================================

NH_RESULT Nh_init(
    int argc, char **argv_pp, Nh_OutputCallback callback)
{
    if (Nh_getThreadPool() != NULL) {NH_END(NH_USAGE_ERROR_ALREADY_INITIALIZED)}

    NH_CHECK(Nh_initializeMemory())
    NH_CHECK(Nh_createThreadPool())
    NH_CHECK(Nh_activateThread(NH_THREAD_CORE, false))
    NH_CHECK(Nh_Net_createNetwork())
    NH_CHECK(Nh_initConfig())

    Nh_TTY_initTerminal();
    Nh_TTY_initPublicFlags();

    Nh_getConfig()->Settings.startpage_p = Nh_TTY_handleStartUpCommands(argc, argv_pp);

#ifdef __unix__

    if (Nh_TTY_getTerminal()->Flags.run) 
    {
        Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();

        pipe(Terminal_p->IPC.readFD_p);
        pipe(Terminal_p->IPC.writeFD_p);

        for (int i = 0; i < Nh_getThreadPool()->totalThreads; ++i) {
            pipe(Nh_getThreadPool()->Threads_p[i].IPC.TTY.readFD_p);
            pipe(Nh_getThreadPool()->Threads_p[i].IPC.TTY.writeFD_p);
        }

        Terminal_p->IPC.pid = fork(); 

        if (Terminal_p->IPC.pid == 0) { // child handles TTY
            close(Terminal_p->IPC.readFD_p[0]);
            close(Terminal_p->IPC.writeFD_p[1]);
            for (int i = 0; i < Nh_getThreadPool()->totalThreads; ++i) {
                close(Nh_getThreadPool()->Threads_p[i].IPC.TTY.readFD_p[0]);
                close(Nh_getThreadPool()->Threads_p[i].IPC.TTY.writeFD_p[1]);
            }
            Nh_TTY_run();
        }
        else {
            close(Terminal_p->IPC.readFD_p[1]);
            close(Terminal_p->IPC.writeFD_p[0]);
            for (int i = 0; i < Nh_getThreadPool()->totalThreads; ++i) {
                close(Nh_getThreadPool()->Threads_p[i].IPC.TTY.readFD_p[1]);
                close(Nh_getThreadPool()->Threads_p[i].IPC.TTY.writeFD_p[0]);
            }
            struct sigaction action;
            memset(&action, 0, sizeof(action));
            action.sa_handler = Nh_TTY_handleTermination;
            sigaction(SIGCHLD, &action, NULL);
        }
    }

#endif

    NH_CHECK(Nh_initOutputs(callback))
    NH_CHECK(Nh_initFontManager())
    NH_CHECK(Nh_addInitialFonts())
    NH_CHECK(Nh_addInitialFontPrototypes())
    NH_CHECK(Nh_createHashMaps())
    NH_CHECK(Nh_initSystem())

    return NH_SUCCESS;
}

// START ===========================================================================================

NH_RESULT Nh_start()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_USAGE_ERROR_INIT_MISSING, Nh_getThreadPool())
    NH_CHECK_NULL(NH_USAGE_ERROR_INIT_MISSING, Nh_getConfig()->Settings.startpage_p)

#include NH_DEFAULT_CHECK

    if (Nh_getThreadPool()->activeThreads > 1) { // already started
        NH_END(NH_SUCCESS)
    }

    char *page_p = Nh_getConfig()->Settings.startpage_p;
    int pxSize_p[2] = {1000, 1000};

    Nh_Thread *Thread_p = Nh_startWindowThread(pxSize_p);
    NH_CHECK_NULL(Thread_p)

    NH_CHECK(Nh_startTabThread(Thread_p->Data_p, page_p, pxSize_p))

NH_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

NH_RESULT Nh_update()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_USAGE_ERROR_INIT_MISSING, Nh_getThreadPool())

#include NH_DEFAULT_CHECK

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    int count = ThreadPool_p->activeThreads;

    for (int i = 1; i < ThreadPool_p->totalThreads && count > 0; ++i) 
    {
        Nh_Thread *Thread_p = &ThreadPool_p->Threads_p[i];
        Nh_Window *Window_p = Thread_p->Data_p;

        if (Thread_p->Info.active) {count--;}
        if (Thread_p->type == NH_THREAD_WINDOW && Window_p->Sync.ready)
        {
            if (Window_p->Flags.getInput) {NH_CHECK(Nh_IO_getInput(Window_p))}
        }
    }

    NH_CHECK(Nh_runVirtualThreads())
    if (Nh_TTY_getTerminal()->Flags.run) {NH_CHECK(Nh_TTY_handleQueries(NULL))}

NH_END(NH_SUCCESS)
}

// KEEP RUNNING ====================================================================================

NH_BOOL Nh_keepRunning()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_FALSE, Nh_getThreadPool())

#include NH_DEFAULT_CHECK

NH_END(Nh_getThreadPool()->activeThreads > 1 ? NH_TRUE : NH_FALSE)
}

// RELEASE =========================================================================================

NH_RESULT Nh_release(
    Nh_Output *Output_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_USAGE_ERROR_INIT_MISSING, Nh_getThreadPool())

    if (Output_p == NH_NULL || Output_p == NULL)
    {
        NH_CHECK_FALSE(NH_USAGE_ERROR_STILL_RUNNING, Nh_keepRunning())

#include NH_DEFAULT_CHECK

#ifdef __unix__

        if (Nh_TTY_getTerminal()->Flags.run)
        {
            Nh_TTY_Terminal *Terminal_p = Nh_TTY_getTerminal();
            if (!Terminal_p->Flags.manualExit) {kill(Terminal_p->IPC.pid, SIGTERM);}
            int w = -1;
            wait(&w);
            close(Terminal_p->IPC.readFD_p[0]);
        }

        for (int i = 0; i < Nh_getThreadPool()->totalThreads; ++i) {
            if (Nh_getThread(i)->Info.active && !Nh_getThread(i)->Info.isVirtual) {
                pthread_join(Nh_getThread(i)->unixId, NULL);
            } 

        }

#endif

        Nh_TTY_freeTerminal();
        Nh_destroyFontManager();
        Nh_freeHashMaps();
        Nh_freeSystem();
        Nh_freeOutputs();
        Nh_freeConfig();
        Nh_freeThreadPool();
    }
    else {Nh_freeOutput(Output_p);}

NH_END(NH_SUCCESS)
}

// PARSE ===========================================================================================

Nh_Output *Nh_parse(
    char *path_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_NULL, Nh_getThreadPool(), path_p)

//    char absPath_p[PATH_MAX];
//    Nh_getAbsPath(path_p, absPath_p);
//
//    if (NH_CSS_IS_SHEET(absPath_p))
//    {
//        NH_FileData Data;
//        NH_CHECK(NH_NULL, Nh_getFileData(absPath_p, &Data))
//
//        NH_CSS_Sheet *Sheet_p = Nh_allocate(sizeof(NH_CSS_Sheet));
//        Nh_Output *Output_p = Nh_allocate(sizeof(Nh_Output));
//        NH_CHECK_NULL(NH_NULL, Sheet_p, Output_p)
//
//        Nh_CSS_initSheet(Sheet_p);
//        Sheet_p->source_p = Data.data_p;
//        NH_CHECK(NH_NULL, Nh_CSS_parseSheet(Sheet_p))
//
//        Output_p->type = NH_OUTPUT_CSS_SHEET;
//        Output_p->pointer = Sheet_p;
//
//        NH_CHECK(NH_NULL, Nh_addListItem(&Nh_getConfig()->Outputs, Output_p))
//        NH_END(Output_p)
//    }
//    if (NH_HTML_IS_DOCUMENT(absPath_p))
//    {
//        Nh_HTML_Document *Document_p = Nh_allocate(sizeof(Nh_HTML_Document));
//        Nh_Output *Output_p = Nh_allocate(sizeof(Nh_Output));
//        NH_CHECK_NULL(NH_NULL, Document_p, Output_p)
//
//        NH_CHECK(NH_NULL, Nh_HTML_initDocument(absPath_p, Document_p))
//        NH_CHECK(NH_NULL, Nh_HTML_parseDocument(Document_p))
//
//        Output_p->type = NH_OUTPUT_HTML_DOCUMENT;
//        Output_p->pointer = Document_p;
//
//        NH_CHECK(NH_NULL, Nh_addListItem(&Nh_getConfig()->Outputs, Output_p))
//        NH_END(Output_p)
//    }
//    if (NH_JS_IS_SCRIPT(absPath_p))
//    {
//        NH_FileData Data;
//        NH_CHECK(NH_NULL, Nh_getFileData(absPath_p, &Data))
//
//        Nh_JS_Script *Script_p = Nh_allocate(sizeof(Nh_JS_Script));
//        Nh_Output *Output_p = Nh_allocate(sizeof(Nh_Output));
//        NH_CHECK_NULL(NH_NULL, Script_p, Output_p)
//
//        Nh_JS_initScript(Script_p);
//        Script_p->Info.source_p = Data.data_p;
//        NH_CHECK(NH_NULL, Nh_JS_parseScript(Script_p))
//
//        Output_p->type = NH_OUTPUT_JS_SCRIPT;
//        Output_p->pointer = Script_p;
//
//        NH_CHECK(NH_NULL, Nh_addListItem(&Nh_getConfig()->Outputs, Output_p))
//        NH_END(Output_p)
//    }

#include NH_DEFAULT_CHECK

NH_END(NULL)
}

// STRINGIFY =======================================================================================

const char *Nh_stringify(
    Nh_Output *Output_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_NULL, Nh_getThreadPool(), Output_p)

    if (Output_p->type == NH_OUTPUT_CHARS) {NH_END(Output_p->pointer)}

    char *p = NH_NULL;

    switch (Output_p->type)
    {
        case NH_OUTPUT_CSS_SHEET :
            p = Nh_CSS_stringifySheet(Output_p->pointer);
            break;

        case NH_OUTPUT_HTML_DOCUMENT :
            p = Nh_HTML_stringifyDocument(Output_p->pointer, NH_TRUE);
            break;

        case NH_OUTPUT_JS_SCRIPT :
            p = Nh_JS_stringifyScript(Output_p->pointer);
            break;
    }

    if (p != NH_NULL) {Nh_addOutput(NH_OUTPUT_CHARS, p, NH_TRUE);}

#include NH_DEFAULT_CHECK

NH_END(p)
}

// PRINT ===========================================================================================

NH_RESULT Nh_print(
    Nh_Output *Output_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NH_USAGE_ERROR_INIT_MISSING, Nh_getThreadPool())
    NH_CHECK_NULL(NH_USAGE_ERROR_NULL_INPUT, Output_p)

#include NH_DEFAULT_CHECK

    if (Output_p->type != NH_OUTPUT_CHARS) {printf(Nh_stringify(Output_p));}
    else {printf(Output_p->pointer);}

NH_END(NH_SUCCESS)
}

