// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Process.h"

#include "../Common/Macros.h"

#include "../../nh-ecmascript/Engine/Agent.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#ifdef __unix__
    #include <sys/wait.h>
#endif

// DATA ============================================================================================

static bool init = false;
nh_ProcessPool NH_PROCESS_POOL;

// INIT/FREE =======================================================================================

NH_API_RESULT nh_core_initProcessPool()
{
NH_CORE_BEGIN()

    if (init == true) {NH_CORE_END(NH_API_ERROR_BAD_STATE)}

    NH_PROCESS_POOL.forks = 0;
    NH_PROCESS_POOL.IPC.updateIntervalInSeconds = 1;
    NH_PROCESS_POOL.IPC.LastUpdate = nh_core_getSystemTime();

    NH_PROCESS_POOL.Main.id = 0;
    NH_PROCESS_POOL.Main.ThreadPool = nh_core_initThreadPool();

    nh_core_initChannel(&NH_PROCESS_POOL.Main.IPC.In);
    nh_core_initChannel(&NH_PROCESS_POOL.Main.IPC.Out);

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        NH_PROCESS_POOL.Forks_p[i].id = 0;
        nh_core_initChannel(&NH_PROCESS_POOL.Forks_p[i].IPC.In);
        nh_core_initChannel(&NH_PROCESS_POOL.Forks_p[i].IPC.Out);
    }

    init = true;
 
NH_CORE_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_core_freeProcessPool() 
{
NH_CORE_BEGIN()

    nh_core_freeThreadPool(&NH_PROCESS_POOL.Main.ThreadPool);
    init = false;

NH_CORE_END(NH_API_SUCCESS)
}

// FORK ============================================================================================

static nh_Fork *nh_core_getAvailableFork()
{
NH_CORE_BEGIN()

    if (NH_PROCESS_POOL.forks == NH_MAX_FORKS) {NH_CORE_END(NULL)}

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        if (NH_PROCESS_POOL.Forks_p[i].id == 0) {
            NH_PROCESS_POOL.forks++;
            NH_CORE_END(&NH_PROCESS_POOL.Forks_p[i])
        }
    }

NH_CORE_END(NULL)
}

nh_Fork *nh_core_fork()
{
NH_CORE_BEGIN()

    if (init == false) {NH_CORE_END(NULL)}
    nh_Fork *Fork_p = nh_core_getAvailableFork();
    if (Fork_p == NULL) {NH_CORE_END(NULL)}

    nh_openChannel(&Fork_p->IPC.In);
    nh_openChannel(&Fork_p->IPC.Out);

    Fork_p->id = fork();

    if (Fork_p->id == 0) { // child
        Fork_p->id = getpid();
        nh_closeChannelWriteAccess(&Fork_p->IPC.In);
        nh_closeChannelReadAccess(&Fork_p->IPC.Out);
        NH_CORE_END(NULL)
    }

    nh_closeChannelReadAccess(&Fork_p->IPC.In);
    nh_closeChannelWriteAccess(&Fork_p->IPC.Out);

NH_CORE_END(Fork_p)
}

static NH_API_RESULT nh_unregisterFork(
    nh_Fork *Fork_p)
{
NH_CORE_BEGIN()

    if (init == false || Fork_p->id == 0) {NH_CORE_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)}

    nh_closeChannelWriteAccess(&Fork_p->IPC.In);
    nh_closeChannelReadAccess(&Fork_p->IPC.Out);

    Fork_p->id = 0;

    NH_PROCESS_POOL.forks--;

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

void nh_checkForks()
{
NH_CORE_BEGIN()

    if (init == false) {NH_CORE_SILENT_END()}

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        nh_Fork *Fork_p = &NH_PROCESS_POOL.Forks_p[i];
        if (Fork_p->id != 0) {
            int status;
#ifdef __unix__
            int result = waitpid(Fork_p->id, &status, WNOHANG);
            if (result == -1) {
                printf("nh_checkForks %s\n", strerror(errno));
            }
            if (result == -1 || WIFEXITED(status)) {
                nh_unregisterFork(&NH_PROCESS_POOL.Forks_p[i]);
            }
#endif
        }
    }

NH_CORE_SILENT_END()
}

int nh_core_activeForks()
{
NH_CORE_BEGIN()

    if (init == false) {NH_CORE_END(0)}

NH_CORE_END(NH_PROCESS_POOL.forks)
}

void nh_killFork(
    nh_Fork *Fork_p)
{
NH_CORE_BEGIN()

    if (init == false) {NH_CORE_SILENT_END()}

    kill(Fork_p->id, SIGTERM);
    nh_unregisterFork(Fork_p);

NH_CORE_SILENT_END()
}

void nh_killForks()
{
NH_CORE_BEGIN()

    if (init == false) {NH_CORE_SILENT_END()}

    for (int i = 0; i < NH_MAX_FORKS; ++i) { 
        if (NH_PROCESS_POOL.Forks_p[i].id != 0) {
            kill(NH_PROCESS_POOL.Forks_p[i].id, SIGTERM);
            nh_unregisterFork(&NH_PROCESS_POOL.Forks_p[i]);
        }
    }

NH_CORE_SILENT_END()
}

nh_Fork *nh_core_getFork()
{
NH_CORE_BEGIN()

    pid_t pid = getpid();

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        if (NH_PROCESS_POOL.Forks_p[i].id == pid) {
            NH_CORE_END(&NH_PROCESS_POOL.Forks_p[i])
        }
    }

NH_CORE_END(NULL)
}

// WRITE ===========================================================================================

char *_nh_core_writeToProcess(
    nh_Process *Proc_p, char *write_p, int writeLen, bool getResponse)
{
    nh_core_writeToChannel(&Proc_p->IPC.In, write_p, writeLen);

    while (getResponse) 
    {
        char *response_p = nh_readFromChannel(&Proc_p->IPC.Out, NULL);
        if (response_p != NULL) {
            return response_p;
        }
    }

    return NULL;
}

char *nh_core_writeToProcess(
    nh_Process *Proc_p, char *write_p, int writeLen, bool getResponse)
{
NH_CORE_BEGIN()
NH_CORE_END(_nh_core_writeToProcess(Proc_p, write_p, writeLen, getResponse))
}

// IPC HANDLER =====================================================================================

// TODO make better
NH_API_RESULT nh_handleIPCReceive(
    char *bytes_p)
{
NH_CORE_BEGIN()

//    nh_ecmascript_AgentArgs Args;
//    Args.encoding = NH_UNICODE_ENCODING_UTF8;
//    Args.bytes_p  = bytes_p + 17;
//
//    if (NH_LOADER.load_f("nh-ecmascript", 0) == NH_API_SUCCESS) 
//    {
//        nh_core_activateWorkload(
//            (nh_ecmascript_initAgent_f)NH_LOADER.loadSymbol_f("nh-ecmascript", 0, "nh_ecmascript_initAgent"), 
//            (nh_ecmascript_runAgent_f)NH_LOADER.loadSymbol_f("nh-ecmascript", 0, "nh_ecmascript_runAgent"), 
//            &Args, false
//        );
//    }

NH_CORE_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_SIGNAL nh_core_runIPCHandler(
    void *args_p)
{
NH_CORE_BEGIN()

    if (nh_core_getSystemTimeDiffInSeconds(NH_PROCESS_POOL.IPC.LastUpdate, nh_core_getSystemTime()) < NH_PROCESS_POOL.IPC.updateIntervalInSeconds) {
        NH_CORE_END(NH_SIGNAL_OK)
    }

    for (int i = 0; i < NH_MAX_FORKS; ++i) {
        if (NH_PROCESS_POOL.Forks_p[i].id != 0) {
            char *receive_p = nh_readFromChannel(&NH_PROCESS_POOL.Forks_p[i].IPC.Out, NULL);
            if (receive_p != NULL) {
                NH_CORE_CHECK(nh_handleIPCReceive(receive_p))
            }
// TODO free?
        }
    }

    NH_PROCESS_POOL.IPC.LastUpdate = nh_core_getSystemTime();

NH_CORE_END(NH_SIGNAL_OK)
}

