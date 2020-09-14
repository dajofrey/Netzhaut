// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// DEFINE ==========================================================================================

#ifdef __unix__
    #define CTRL_KEY(k) ((k) & 0x1f)
#endif

// INCLUDE =========================================================================================

#include "../Header/Input.h"
#include "../Header/Row.h"
#include "../Header/Command.h"

#include "../Linux/Header/Input.h"

#include <unistd.h>
#include <stdio.h>

// INPUT ===========================================================================================

void Nh_TTY_moveCursor(
    Nh_TTY_Tab *Tab_p, int key) 
{
    Nh_TTY_Row *Row_p = (Tab_p->cursorY >= Tab_p->Rows.count) ? NULL : Nh_getListItem(&Tab_p->Rows, Tab_p->cursorY);

    switch (key) 
    {
        case NH_TTY_KEY_ARROW_LEFT:
            if (Tab_p->cursorX != 0) {
                Tab_p->cursorX--;
            } else if (Tab_p->cursorY > 0) {
                Tab_p->cursorY--;
                Tab_p->cursorX = ((Nh_TTY_Row*)Nh_getListItem(&Tab_p->Rows, Tab_p->cursorY))->size;
            }
            break;

        case NH_TTY_KEY_ARROW_RIGHT:
            if (Row_p && Tab_p->cursorX < Row_p->size) {
                Tab_p->cursorX++;
            } else if (Row_p && Tab_p->cursorX == Row_p->size) {
                Tab_p->cursorY++;
                Tab_p->cursorX = 0;
            }
            break;

        case NH_TTY_KEY_ARROW_UP:
            if (Tab_p->cursorY != 0) {Tab_p->cursorY--;}
            break;

        case NH_TTY_KEY_ARROW_DOWN:
            if (Tab_p->cursorY < Tab_p->Rows.count) {Tab_p->cursorY++;}
            break;
    }
  
    Row_p = (Tab_p->cursorY >= Tab_p->Rows.count) ? NULL : Nh_getListItem(&Tab_p->Rows, Tab_p->cursorY);
    int rowlen = Row_p ? Row_p->size : 0;
    if (Tab_p->cursorX > rowlen) {Tab_p->cursorX = rowlen;}
}

void Nh_TTY_processKeypress(
    Nh_TTY_Terminal *Terminal_p) 
{
    Nh_TTY_Tab *Tab_p = Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab);

#ifdef __unix__
    int c = Nh_TTY_Linux_readKey();
#endif

    if (c == -1) {return;}

    switch (c) 
    {
#ifdef __unix__
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            Terminal_p->Flags.run = false;
            break;

        case CTRL_KEY('s'):
            break;
#endif
    }

    if (Terminal_p->focus == NH_TTY_FOCUS_OUTPUT && !Terminal_p->Flags.simple)
    {  
        switch (c) 
        {
            case NH_TTY_KEY_HOME:
                Tab_p->cursorX = 0;
                break;
      
            case NH_TTY_KEY_END:
                if (Tab_p->cursorY < Tab_p->Rows.count) {Tab_p->cursorX = ((Nh_TTY_Row*)Nh_getListItem(&Tab_p->Rows, Tab_p->cursorY))->size;}
                break;
      
            case NH_TTY_KEY_PAGE_UP:
            case NH_TTY_KEY_PAGE_DOWN:
            {
                if (c == NH_TTY_KEY_PAGE_UP) {
                    Tab_p->cursorY = Tab_p->rowOffset;
                } else if (c == NH_TTY_KEY_PAGE_DOWN) {
                    Tab_p->cursorY = Tab_p->rowOffset + Terminal_p->screenRows - 1;
                    if (Tab_p->cursorY > Tab_p->Rows.count) Tab_p->cursorY = Tab_p->Rows.count;
                }
      
                int times = Terminal_p->screenRows;
                while (times--) {
                    Nh_TTY_moveCursor(Tab_p, c == NH_TTY_KEY_PAGE_UP ? NH_TTY_KEY_ARROW_UP : NH_TTY_KEY_ARROW_DOWN);
                }
            }
                break;
      
            case NH_TTY_KEY_ARROW_UP    :
            case NH_TTY_KEY_ARROW_DOWN  :
            case NH_TTY_KEY_ARROW_LEFT  :
            case NH_TTY_KEY_ARROW_RIGHT :
                Nh_TTY_moveCursor(Tab_p, c);
                break;
    
            case ':':
                Terminal_p->focus = NH_TTY_FOCUS_COMMAND_PROMPT;
                break;
        }
    }
    else 
    {
        switch (c) 
        {
            case NH_TTY_KEY_ARROW_LEFT  :
            case NH_TTY_KEY_ARROW_RIGHT :
                break;

            case NH_TTY_KEY_ARROW_UP    :
                if (Terminal_p->historyIndex < (Terminal_p->CommandHistory.count - 1)) {
                    Terminal_p->historyIndex++; 
                    if (Terminal_p->historyIndex == 0) {
                        Terminal_p->OldCommand_p = Terminal_p->CurrentCommand_p;
                    }
                    Terminal_p->CurrentCommand_p = Nh_getListItem(&Terminal_p->CommandHistory, Terminal_p->CommandHistory.count - Terminal_p->historyIndex - 1);
                }
                break;

            case NH_TTY_KEY_ARROW_DOWN  :
                if (Terminal_p->historyIndex >= 0) {
                    Terminal_p->historyIndex--;
                }
                if (Terminal_p->historyIndex >= 0) {
                    Terminal_p->CurrentCommand_p = Nh_getListItem(&Terminal_p->CommandHistory, Terminal_p->CommandHistory.count - Terminal_p->historyIndex - 1);
                } else {
                    Terminal_p->CurrentCommand_p = Terminal_p->OldCommand_p;
                }
                break;
    
            case 27: // escape
                Terminal_p->focus = NH_TTY_FOCUS_OUTPUT;
                break;
    
            case 127: // delete 
                if (Terminal_p->CurrentCommand_p != NULL) {
                    Nh_setChars(Terminal_p->CurrentCommand_p, '\0', -1, 1);
                    Tab_p->commandCursorX--;
                }
                break;
            
            case 10  : // enter
            case '\r':
                Nh_TTY_executeCommand(Terminal_p, Terminal_p->CurrentCommand_p);
                Nh_addListItem(&Terminal_p->CommandHistory, Terminal_p->CurrentCommand_p);
                Terminal_p->CurrentCommand_p = NULL;
                Tab_p->commandCursorX = 2; 
                if (Terminal_p->Flags.simple) {
                    printf("Netzhaut: ");
                    fflush(stdout);
                }
                break;
    
            default :
                if (Terminal_p->CurrentCommand_p == NULL) {
                    Terminal_p->CurrentCommand_p = Nh_allocateString(NULL);
                }
                Nh_appendCharToString(Terminal_p->CurrentCommand_p, c);
                Tab_p->commandCursorX++;
        }
    }
}

