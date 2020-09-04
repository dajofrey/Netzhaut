// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// DEFINE ==========================================================================================

#define CMD_NOT_FND "Command not found!"

// INCLUDE =========================================================================================

#include "../Header/Command.h"
#include "../Header/Row.h"
#include "../Header/Query.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

static void Nh_TTY_clear(
    Nh_TTY_Terminal *Terminal_p, char **argv_pp
); 
static void Nh_TTY_help(
    Nh_TTY_Terminal *Terminal_p, char **argv_pp
); 
static void Nh_TTY_quit(
    Nh_TTY_Terminal *Terminal_p
); 
static void Nh_TTY_thrd(
    char **argv_pp
); 

// EXECUTE =========================================================================================

NH_RESULT Nh_TTY_executeCommand(
    Nh_TTY_Terminal *Terminal_p, Nh_String *String_p)
{
    char *full_p = malloc(sizeof(char) * (strlen(Nh_getChars(String_p)) + 1));
    if (full_p == NULL) {return NH_ERROR_BAD_STATE;}
    strcpy(full_p, Nh_getChars(String_p));
    
    char *command_p = strtok(full_p, " ");

    int argc = 0;
    char *argv_pp[100] = {NULL};
    char *tmp_p = NULL;

    do {
        tmp_p = strtok(NULL, " ");
        if (tmp_p != NULL) {argv_pp[argc++] = tmp_p;}
    } while (tmp_p != NULL);

         if (!strcmp(command_p, "clear") || !strcmp(command_p, "c")) {Nh_TTY_clear(Terminal_p, argv_pp);}
    else if (!strcmp(command_p, "help") || !strcmp(command_p, "h"))  {Nh_TTY_help(Terminal_p, argv_pp);}
    else if (!strcmp(command_p, "log") || !strcmp(command_p, "l"))   {Nh_TTY_log(Terminal_p, -1, argv_pp, true);}
    else if (!strcmp(command_p, "quit") || !strcmp(command_p, "q"))  {Nh_TTY_quit(Terminal_p);}
    else if (!strcmp(command_p, "thrd") || !strcmp(command_p, "t"))  {Nh_TTY_thrd(argv_pp);}
    else if (!strcmp(command_p, "tty"))                              {
        Nh_TTY_tty(Terminal_p, -1, argv_pp, true);
        Nh_TTY_tty(Terminal_p, -1, argv_pp, false);
    }
    else {Nh_TTY_addRow(Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab), CMD_NOT_FND, strlen(CMD_NOT_FND));}

    free(full_p);

    return NH_SUCCESS;
}

// COMMANDS ========================================================================================

static void Nh_TTY_clear(
    Nh_TTY_Terminal *Terminal_p, char **argv_pp) 
{    
    if (argv_pp[0] == NULL) {
        Nh_TTY_destroyRows(Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab));
        return;
    }
    if (!strcmp(argv_pp[0], "all")) {
        for (int i = 0; i < Terminal_p->Tabs.count; ++i) {
            Nh_TTY_destroyRows(Nh_getListItem(&Terminal_p->Tabs, i));
        }
    }
}

static void Nh_TTY_help(
    Nh_TTY_Terminal *Terminal_p, char **argv_pp) 
{
    char title_p[]  = "Commands:"; 
    char clear1_p[] = "{clear | c} [all]";
    char clear2_p[] = "Clear previous command output.";
    char help1_p[]  = "{help | h}";
    char help2_p[]  = "Show this text.";
    char log1_p[]   = "{log | l} [<number>] [stop] [resume] [reset] [init] [mouse] [vulkan] [script] [listener] [event] [jsinterpreter] [jsparser] [sheet] [cssparser] [document] [htmlparser]";
    char log2_p[]   = "Activate and show output from the logging system for the thread defined by NUMBER.";
    char quit1_p[]  = "{quit | q}";
    char quit2_p[]  = "Exit this terminal.";
    char thrd1_p[]  = "{thrd | t} [<number>]";
    char thrd2_p[]  = "Show thread information.";
    char tty1_p[]   = "{tty} [run] [manualexit] [fatal] [reset]";
    char tty2_p[]   = "Configure TTY terminal.";

    Nh_TTY_Tab *Tab_p = Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab);

    Nh_TTY_addRow(Tab_p, title_p, strlen(title_p));
    Nh_TTY_addRow(Tab_p, clear1_p, strlen(clear1_p));
    Nh_TTY_addRow(Tab_p, clear2_p, strlen(clear2_p));
    Nh_TTY_addRow(Tab_p, help1_p, strlen(help1_p));
    Nh_TTY_addRow(Tab_p, help2_p, strlen(help2_p));
    Nh_TTY_addRow(Tab_p, log1_p, strlen(log1_p));
    Nh_TTY_addRow(Tab_p, log2_p, strlen(log2_p));
    Nh_TTY_addRow(Tab_p, quit1_p, strlen(quit1_p));
    Nh_TTY_addRow(Tab_p, quit2_p, strlen(quit2_p));
    Nh_TTY_addRow(Tab_p, thrd1_p, strlen(thrd1_p));
    Nh_TTY_addRow(Tab_p, thrd2_p, strlen(thrd2_p));
    Nh_TTY_addRow(Tab_p, tty1_p, strlen(tty1_p));
    Nh_TTY_addRow(Tab_p, tty2_p, strlen(tty2_p));
}

NH_RESULT Nh_TTY_log(
    Nh_TTY_Terminal *Terminal_p, int argc, char **argv_pp, bool IPC) 
{
    Nh_TTY_Tab *Tab_p = Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab);

    for (int i = 0; i < argc || (argc == -1 && argv_pp[i] != NULL); ++i)
    {
        if (atoi(argv_pp[i]) || argv_pp[i][0] == 0) {
            Tab_p->logThread = atoi(argv_pp[i]);
            char str_p[255] = {'\0'};
            snprintf(str_p, 255, "Logging thread %d.", atoi(argv_pp[i]));
            Nh_TTY_addRow(Tab_p, str_p, strlen(str_p));
        }
        else if (!strcmp(argv_pp[i], "stop")) {
            Nh_TTY_addRow(Tab_p, "Logging stopped.", strlen("Logging stopped."));
            Tab_p->Flags.logging = false;
        }
        else if (!strcmp(argv_pp[i], "resume")) {
            Nh_TTY_addRow(Tab_p, "Logging resumed.", strlen("Logging resumed."));
            Tab_p->Flags.logging = true;
        }
#ifdef __unix__
        else if (!strcmp(argv_pp[i], "reset")) {
            Nh_TTY_addRow(Tab_p, "Logging reset.", strlen("Logging reset."));
            char *send_p = "logreset";
            if (IPC) {write(Terminal_p->IPC.readFD_p[1], send_p, strlen(send_p) + 1);}
            else {Nh_TTY_handleQueries(send_p);}
            Tab_p->Flags.logging = false;
        }
        else {
            char send_p[255] = {'\0'};
            snprintf(send_p, 255, "log%s", argv_pp[i]);
            if (IPC) {write(Terminal_p->IPC.readFD_p[1], send_p, strlen(send_p) + 1);}
            else if (Nh_TTY_handleQueries(send_p) != NH_SUCCESS) {return NH_ERROR_BAD_STATE;}
            Tab_p->Flags.logging = true;
        }
#endif
    }

    return NH_SUCCESS;
}

static void Nh_TTY_quit(
    Nh_TTY_Terminal *Terminal_p) 
{
    Terminal_p->Flags.run = false;
}

static void Nh_TTY_thrd(
    char **argv_pp) 
{
#ifdef __unix__
    if (argv_pp[0] == NULL) {
        const char *send_p = "thrddump";
        write(Nh_TTY_getTerminal()->IPC.readFD_p[1], send_p, strlen(send_p) + 1);
    }
#endif
}

NH_RESULT Nh_TTY_tty(
    Nh_TTY_Terminal *Terminal_p, int argc, char **argv_pp, bool IPC) 
{
    Nh_TTY_Tab *Tab_p = Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab);

    for (int i = 0; (argc == -1 && argv_pp[i] != NULL) || i < argc; ++i)
    {
        char send_p[255] = {'\0'};
        snprintf(send_p, 255, "tty%s", argv_pp[i]);
        if (IPC) {write(Terminal_p->IPC.readFD_p[1], send_p, strlen(send_p) + 1);}
        else if (Nh_TTY_handleQueries(send_p) != NH_SUCCESS) {return NH_ERROR_BAD_STATE;}
        i++;
    }

    return NH_SUCCESS;
}

