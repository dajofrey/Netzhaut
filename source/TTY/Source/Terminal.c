// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Terminal.h"
#include "../Header/Input.h"
#include "../Header/Draw.h"
#include "../Header/Row.h"
#include "../Header/Query.h"
#include "../Header/Command.h"
#include "../Header/Macros.h"

#include "../Linux/Header/Terminal.h"

#include "../../Core/Header/Config.h"
#include "../../Core/Header/Thread.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Output.h"

#include NH_UTILS

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// DATA ============================================================================================

Nh_TTY_Terminal Terminal;

// DECLARE =========================================================================================

static void Nh_TTY_initTab(
    Nh_TTY_Tab *Tab_p
);

// CALLBACKS =======================================================================================

void Nh_TTY_write(
    const char *chars_p)
{
#ifdef __unix__

    write(Terminal.IPC.writeFD_p[1], chars_p, strlen(chars_p));

#endif
}

void Nh_TTY_syncedWrite(
    const char *chars_p)
{
#ifdef __unix__

    write(Nh_getThread()->IPC.TTY.writeFD_p[1], chars_p, strlen(chars_p));

#endif
}

void Nh_TTY_writeLog(
    Nh_Output *Output_p)
{
#ifdef __unix__

    const char *log_p = Nh_stringify(Output_p);
    write(Nh_getThread()->IPC.TTY.writeFD_p[1], log_p, strlen(log_p));
    if (Nh_getOutputCallbackCount() == 1) {Nh_release(Output_p);}

#endif
}

void Nh_TTY_terminate(
    int signum)
{
    Terminal.Flags.run = false;
}

void Nh_TTY_handleTermination(
    int signum)
{
    if (Terminal.Flags.fatal) {
        Nh_getConfig()->Flags.Internal.shutdown = true;
    }
}

// TERMINAL ========================================================================================

void Nh_TTY_initPublicFlags()
{
    Nh_TTY_handleQueries("logreset");
    Nh_TTY_handleQueries("ttyreset");
}

char *Nh_TTY_handleStartUpCommands(
    int argc, char **argv_pp)
{
#define LOG_COMMAND()  (!strcmp(argv_pp[i], "-l") || !strcmp(argv_pp[i], "--log"))
#define TTY_COMMAND()  (!strcmp(argv_pp[i], "-t") || !strcmp(argv_pp[i], "--tty"))
#define TEST_COMMAND() (!strcmp(argv_pp[i], "--test"))

    bool tty = false, log = false, test = false;
    char *startpage_p = NULL;

    for (int i = 1; i < argc; ++i) 
    {
             if (tty && (Nh_TTY_tty(&Terminal, 1, &argv_pp[i], false) == NH_SUCCESS)) {}
        else if (log && Nh_TTY_log(&Terminal, 1, &argv_pp[i], false) == NH_SUCCESS) {}
        else if (TTY_COMMAND())  {tty = true; log = false;}
        else if (LOG_COMMAND())  {log = true; tty = false;}
        else if (TEST_COMMAND()) {
            Nh_free(startpage_p); 
            startpage_p = Nh_allocateChars("nh:test:index"); 
        }
        else {
            Nh_free(startpage_p); 
            startpage_p = Nh_allocateChars(argv_pp[i]);
        }
    }

    return startpage_p;

#undef LOG_COMMAND
#undef TTY_COMMAND
#undef TEST_COMMAND
}

int Nh_TTY_run() 
{
#ifdef __unix__

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = Nh_TTY_terminate;
    sigaction(SIGTERM, &action, NULL);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;

#ifdef __unix__
    if (!Terminal.Flags.simple) {
        Nh_TTY_Linux_initTerminal(&Terminal);
    }
#endif

    while (Terminal.Flags.run) 
    {
        Nh_TTY_Tab *Tab_p = Nh_getListItem(&Terminal.Tabs, Terminal.currentTab);

        if (!Terminal.Flags.simple) {Nh_TTY_refreshScreen(&Terminal);}
        Nh_TTY_processKeypress(&Terminal);

        int r = Terminal.IPC.writeFD_p[0];

        fd_set set;
        FD_ZERO(&set);
        FD_SET(r, &set);

        int rv = select(r + 1, &set, NULL, NULL, &timeout);
        if (rv != -1 && rv != 0)
        { 
            char str_p[512] = {'\0'};
            int n = 0;
            read(r, &str_p[n], 1);
            while (str_p[n] != '\n' && n < 511) {read(r, &str_p[++n], 1);}
            if (n != 511) {Nh_TTY_addRow(Nh_getListItem(&Terminal.Tabs, Terminal.currentTab), str_p, n);}
        }

        for (int i = 0; i < Nh_getThreadPool()->totalThreads; ++i)
        {
            r = Nh_getThreadPool()->Threads_p[i].IPC.TTY.writeFD_p[0];

            FD_ZERO(&set);
            FD_SET(r, &set);
          
            rv = select(r + 1, &set, NULL, NULL, &timeout);
            if (rv != -1 && rv != 0)
            { 
                char str_p[512] = {'\0'};
                int n = 0;
                read(r, &str_p[n], 1);
                while (str_p[n] != '\n' && n < 511) {read(r, &str_p[++n], 1);}
                if (Tab_p->logThread == i && Tab_p->Flags.logging) {    
                    if (n != 511) {Nh_TTY_addRow(Nh_getListItem(&Terminal.Tabs, Terminal.currentTab), str_p, n);}
                }
            }
        }
    }

    Nh_TTY_closeTerminal();

#endif
}

Nh_TTY_Terminal *Nh_TTY_getTerminal()
{
    return &Terminal;
}

void Nh_TTY_closeTerminal()
{
#ifdef __unix__

    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    close(Terminal.IPC.readFD_p[1]);
    Nh_TTY_freeTerminal();
    exit(0);

#endif
}

// HELPER ==========================================================================================

void Nh_TTY_initTerminal()
{
    Terminal.focus            = NH_TTY_FOCUS_OUTPUT;
    Terminal.OldCommand_p     = NULL;
    Terminal.CurrentCommand_p = NULL;
    Terminal.currentTab       = 0;
    Terminal.historyIndex     = -1;

    NH_INIT_LIST(Terminal.CommandHistory)
    NH_INIT_LIST(Terminal.Tabs)

    Nh_TTY_Tab *Tab_p = Nh_allocate(sizeof(Nh_TTY_Tab));
    if (Tab_p == NULL) {return;}
    Nh_TTY_initTab(Tab_p);
    Nh_addListItem(&Terminal.Tabs, Tab_p);
}

void Nh_TTY_freeTerminal()
{
    Nh_destroyList(&Terminal.Tabs, true);
}

static void Nh_TTY_initTab(
    Nh_TTY_Tab *Tab_p)
{
    Tab_p->cursorX = 0;
    Tab_p->cursorY = 0;
    Tab_p->commandCursorX = 2;
    Tab_p->commandCursorY = Terminal.screenRows + 2;
    Tab_p->rowOffset = 0;
    Tab_p->colOffset = 0;
    Tab_p->rx = 0;
    Tab_p->logThread = 0;

    Tab_p->Flags.logging = false;

    NH_INIT_LIST(Tab_p->Rows)
}

void Nh_TTY_initFlags()
{
    Terminal.Flags.run        = false;
    Terminal.Flags.manualExit = false;
    Terminal.Flags.fatal      = false;
    Terminal.Flags.simple     = false;
}

