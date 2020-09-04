// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Query.h"
#include "../Header/Terminal.h"
#include "../../Core/Header/Config.h"

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// DECLARE =========================================================================================

void Nh_TTY_handleThrdDumpQuery(
);

// QUERIES =========================================================================================

NH_RESULT Nh_TTY_handleQueries(
    char *send_p)
{
#ifdef __unix__

    char str_p[512] = {'\0'};

    if (send_p == NULL)
    {
        int r = Nh_TTY_getTerminal()->IPC.readFD_p[0];
    
        fd_set set;
        FD_ZERO(&set);
        FD_SET(r, &set);
      
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1;
    
        int rv = select(r + 1, &set, NULL, NULL, &timeout);
        if (rv == -1 || rv == 0) {return NH_SUCCESS;}
    
        int n = 0;
        read(r, &str_p[n], 1);
        while (str_p[n] != '\0' && n < 511) {read(r, &str_p[++n], 1);}
    }
    else {strcpy(str_p, send_p);}

         if (!strcmp(str_p, "thrddump"))         {Nh_TTY_handleThrdDumpQuery();}

    else if (!strcmp(str_p, "loginit"))          {Nh_getConfig()->Flags.Log.init = true;}
    else if (!strcmp(str_p, "logmouse"))         {Nh_getConfig()->Flags.Log.mouse = true;}
    else if (!strcmp(str_p, "logvulkan"))        {Nh_getConfig()->Flags.Log.vulkan = true;}
    else if (!strcmp(str_p, "logscript"))        {Nh_getConfig()->Flags.Log.script = true;}
    else if (!strcmp(str_p, "loglistener"))      {Nh_getConfig()->Flags.Log.listener = true;}
    else if (!strcmp(str_p, "logevent"))         {Nh_getConfig()->Flags.Log.event = true;}
    else if (!strcmp(str_p, "logjsinterpreter")) {Nh_getConfig()->Flags.Log.jsinterpreter = true;}
    else if (!strcmp(str_p, "logjsparser"))      {Nh_getConfig()->Flags.Log.jsparser = true;}
    else if (!strcmp(str_p, "logsheet"))         {Nh_getConfig()->Flags.Log.sheet = true;}
    else if (!strcmp(str_p, "logcssparser"))     {Nh_getConfig()->Flags.Log.cssparser = true;}
    else if (!strcmp(str_p, "logdocument"))      {Nh_getConfig()->Flags.Log.document = true;}
    else if (!strcmp(str_p, "loghtmlparser"))    {Nh_getConfig()->Flags.Log.htmlparser = true;}
    else if (!strcmp(str_p, "logreceive"))       {Nh_getConfig()->Flags.Log.receive = NH_TRUE;}
    else if (!strcmp(str_p, "logsend"))          {Nh_getConfig()->Flags.Log.send = NH_TRUE;}
    else if (!strcmp(str_p, "logmargin"))        {Nh_getConfig()->Flags.Log.margin = NH_TRUE;}
    else if (!strcmp(str_p, "logreset"))         {Nh_initLogFlags();}

    else if (!strcmp(str_p, "ttyrun"))           {Nh_TTY_getTerminal()->Flags.run = true;}
    else if (!strcmp(str_p, "ttyfatal"))         {Nh_TTY_getTerminal()->Flags.fatal = true;}
    else if (!strcmp(str_p, "ttymanualexit"))    {Nh_TTY_getTerminal()->Flags.manualExit = true;}
    else if (!strcmp(str_p, "ttysimple"))        {Nh_TTY_getTerminal()->Flags.simple = true;}
    else if (!strcmp(str_p, "ttyreset"))         {Nh_TTY_initFlags();}
    else {return NH_ERROR_BAD_STATE;}

#endif

    return NH_SUCCESS;
}

// HELPER ==========================================================================================

void Nh_TTY_handleThrdDumpQuery()
{

    Nh_ThreadPool *ThreadPool_p = Nh_getThreadPool();
    int count = ThreadPool_p->activeThreads;

    char init_p[255] = {'\0'};
    char str_p[255] = {'\0'};

    snprintf(str_p, 255, "General info:\n");
    Nh_TTY_write(str_p);
    memcpy(str_p, init_p, sizeof(char) * 255);

    snprintf(str_p, 255, "- active threads       : %d/%d\n", ThreadPool_p->activeThreads, ThreadPool_p->totalThreads);
    Nh_TTY_write(str_p);
    memcpy(str_p, init_p, sizeof(char) * 255);

    snprintf(str_p, 255, "- active kernel threads: %d/%d\n", ThreadPool_p->activeKernelThreads, ThreadPool_p->maxKernelThreads);
    Nh_TTY_write(str_p);
    memcpy(str_p, init_p, sizeof(char) * 255);

    snprintf(str_p, 255, "Active thread listing:\n");
    Nh_TTY_write(str_p);
    memcpy(str_p, init_p, sizeof(char) * 255);

    for (int i = 0; i < ThreadPool_p->totalThreads && count > 0; ++i) 
    {
        Nh_Thread *Thread_p = &ThreadPool_p->Threads_p[i];
        
        if (Thread_p->Info.active) 
        {
            snprintf(str_p, 255, "Thread %d\n", Thread_p->id);
            Nh_TTY_write(str_p);
            memcpy(str_p, init_p, sizeof(char) * 255);

            snprintf(str_p, 255, "- type  : %s\n", Nh_stringifyThreadType(Thread_p->type));
            Nh_TTY_write(str_p);
            memcpy(str_p, init_p, sizeof(char) * 255);

            snprintf(str_p, 255, "- kernel: %s\n", Thread_p->Info.isVirtual ? "No" : "Yes");
            Nh_TTY_write(str_p);
            memcpy(str_p, init_p, sizeof(char) * 255);

            snprintf(str_p, 255, "- owner : %d\n", Thread_p->Info.owner);
            Nh_TTY_write(str_p);
            memcpy(str_p, init_p, sizeof(char) * 255);
        }
    }
}

