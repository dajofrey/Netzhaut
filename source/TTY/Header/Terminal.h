#ifndef NH_TTY_TERMINAL_H
#define NH_TTY_TERMINAL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Core/Header/List.h"
#include "../../Core/Header/String.h"

#ifdef __unix__
#include <termios.h>
#endif

#endif

/** @addtogroup TTYEnums Enums
 *  \ingroup TTY
 *  @{
 */

    typedef enum NH_TTY_FOCUS {
        NH_TTY_FOCUS_OUTPUT,
        NH_TTY_FOCUS_COMMAND_PROMPT,
        NH_TTY_FOCUS_SEARCH_PROMPT,
    } NH_TTY_FOCUS;

/** @} */


/** @addtogroup TTYStructs Structs
 *  \ingroup TTY
 *  @{
 */

    typedef struct Nh_TTY_Tab {

        int cursorX;
        int cursorY;
        int commandCursorX;
        int commandCursorY;
        int rowOffset;
        int colOffset;
        int rx;
        int logThread;
        Nh_List Rows;

        struct {
            bool logging;
        } Flags;

    } Nh_TTY_Tab;

    typedef struct Nh_TTY_Terminal {

        NH_TTY_FOCUS focus;
        Nh_String *CurrentCommand_p;
        Nh_String *OldCommand_p;

        Nh_List CommandHistory;
        Nh_List Tabs;

        int screenRows;
        int screenCols;
        int currentTab;
        int historyIndex;

#ifdef __unix__
        struct {
            struct termios origTermios;
        } Linux;
#endif

        struct {
            bool run;
            bool manualExit;
            bool fatal;
            bool simple;
        } Flags;

#ifdef __unix__
        struct IPC {
            int readFD_p[2];
            int writeFD_p[2];
            pid_t pid;
        } IPC;
#endif

    } Nh_TTY_Terminal;

/** @} */

/** @addtogroup TTYFunctions Functions
 *  \ingroup TTY
 *  @{
 */

    void Nh_TTY_write(
        const char *chars_p
    );

    void Nh_TTY_syncedWrite(
        const char *chars_p
    );

    void Nh_TTY_writeLog(
        Nh_Output *Output_p
    );

    void Nh_TTY_handleTermination(
        int signum
    );

    void Nh_TTY_initPublicFlags(
    );
    
    char *Nh_TTY_handleStartUpCommands(
        int argc, char **argv_pp
    );

    int Nh_TTY_run(
    );

    Nh_TTY_Terminal *Nh_TTY_getTerminal(
    );

    void Nh_TTY_closeTerminal(
    );

    void Nh_TTY_initFlags(
    );

    void Nh_TTY_initTerminal(
    );

    void Nh_TTY_freeTerminal(
    );

    void Nh_TTY_startTerminalThread(
    ); 

/** @} */

#endif 
