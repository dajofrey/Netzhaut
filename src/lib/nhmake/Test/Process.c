// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Process.h"
#include "Channel.h"

#include "../Common/Macros.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#if defined(__linux__) || defined(__APPLE__)
    #include <sys/wait.h>
    #include <signal.h>
#endif

// FORK ============================================================================================

static nh_make_Process nh_make_initProcess()
{
NH_MAKE_BEGIN()

    nh_make_Process Process;
    Process.id = 0;

    nh_make_initChannel(&Process.IPC.In);
    nh_make_initChannel(&Process.IPC.Out);

NH_MAKE_END(Process)
}

nh_make_Process nh_make_fork()
{
NH_MAKE_BEGIN()

    nh_make_Process Fork = nh_make_initProcess();

    nh_make_openChannel(&Fork.IPC.In);
    nh_make_openChannel(&Fork.IPC.Out);

    Fork.id = fork();

    if (Fork.id == 0) { // child
        nh_make_closeChannelWriteAccess(&Fork.IPC.In);
        nh_make_closeChannelReadAccess(&Fork.IPC.Out);
        NH_MAKE_END(Fork)
    }

    nh_make_closeChannelReadAccess(&Fork.IPC.In);
    nh_make_closeChannelWriteAccess(&Fork.IPC.Out);

NH_MAKE_END(Fork)
}












// TODO ============================================================================

static NH_MAKE_RESULT nh_make_unregisterFork(
    nh_make_Process *Fork_p)
{
NH_MAKE_BEGIN()

    if (Fork_p->id == 0) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}

    nh_make_closeChannelWriteAccess(&Fork_p->IPC.In);
    nh_make_closeChannelReadAccess(&Fork_p->IPC.Out);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

void nh_make_checkForks()
{
NH_MAKE_BEGIN()

//    if (init == NH_MAKE_FALSE) {NH_MAKE_SILENT_END()}
//
//    for (int i = 0; i < NH_MAKE_MAX_FORKS; ++i) {
//        nh_make_Process *Proc_p = &NH_MAKE_PROCESS_POOL.Forks_p[i];
//        if (Proc_p->id != 0) {
//            int status;
//#if defined(__linux__) || defined(__APPLE__)
//            int result = waitpid(Proc_p->id, &status, WNOHANG);
//            if (result == -1) {
//                printf("nh_make_checkForks %s\n", strerror(errno));
//            }
//            if (result == -1 || WIFEXITED(status)) {
//                nh_make_unregisterFork(&NH_MAKE_PROCESS_POOL.Forks_p[i]);
//            }
//#endif
//        }
//    }

NH_MAKE_SILENT_END()
}

void nh_make_killFork(
    nh_make_Process *Fork_p)
{
NH_MAKE_BEGIN()

    kill(Fork_p->id, SIGTERM);
    nh_make_unregisterFork(Fork_p);

NH_MAKE_SILENT_END()
}

// WRITE ===========================================================================================

NH_BYTE *_nh_make_writeToProcess(
    nh_make_Process *Proc_p, NH_BYTE *write_p, int writeLen, NH_MAKE_BOOL getResponse)
{
    nh_make_writeToChannel(&Proc_p->IPC.In, write_p, writeLen);

    while (getResponse) 
    {
        NH_BYTE *response_p = nh_make_readFromChannel(&Proc_p->IPC.Out, NULL);
        if (response_p != NULL) {
            return response_p;
        }
    }

    return NULL;
}

NH_BYTE *nh_make_writeToProcess(
    nh_make_Process *Proc_p, NH_BYTE *write_p, int writeLen, NH_MAKE_BOOL getResponse)
{
NH_MAKE_BEGIN()
NH_MAKE_END(_nh_make_writeToProcess(Proc_p, write_p, writeLen, getResponse))
}

