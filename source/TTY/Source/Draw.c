// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// DEFINE ==========================================================================================

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#define KILO_VERSION "0.0.1"
#define KILO_TAB_STOP 8

#define CTRL_KEY(k) ((k) & 0x1f)

// INCLUDE =========================================================================================

#include "../Header/Draw.h"
#include "../Header/Row.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// DECLARE =========================================================================================

struct AppendBuffer {
    char *b;
    int len;
};

static void Nh_TTY_drawRows(
    Nh_TTY_Terminal *Terminal_p, struct AppendBuffer *ab
); 
static void Nh_TTY_drawStatusBar(
    Nh_TTY_Terminal *Terminal_p, struct AppendBuffer *ab
); 
static void Nh_TTY_drawCommandPrompt(
    Nh_TTY_Terminal *Terminal_p, struct AppendBuffer *ab
); 

static void Nh_TTY_scroll(
    Nh_TTY_Terminal *Terminal_p, Nh_TTY_Tab *Tab_p
); 
static void Nh_TTY_abAppend(
    struct AppendBuffer *ab, const char *s, int len
); 
static void Nh_TTY_abFree(
);

// API =============================================================================================

void Nh_TTY_refreshScreen(
    Nh_TTY_Terminal *Terminal_p) 
{
    Nh_TTY_Tab *Tab_p = Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab);
    Nh_TTY_scroll(Terminal_p, Tab_p);
  
    struct AppendBuffer ab = {NULL, 0};
  
    Nh_TTY_abAppend(&ab, "\x1b[?25l", 6);
    Nh_TTY_abAppend(&ab, "\x1b[H", 3);
  
    Nh_TTY_drawRows(Terminal_p, &ab);
    Nh_TTY_drawStatusBar(Terminal_p, &ab);
    Nh_TTY_drawCommandPrompt(Terminal_p, &ab);

    char buf[32];
    if (Terminal_p->focus == NH_TTY_FOCUS_OUTPUT) {
        snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (Tab_p->cursorY - Tab_p->rowOffset) + 1,
        (Tab_p->rx - Tab_p->colOffset) + 1);
    }
    else {
        snprintf(buf, sizeof(buf), "\x1b[%d;%dH", Tab_p->commandCursorY, Tab_p->commandCursorX);
    }

    Nh_TTY_abAppend(&ab, buf, strlen(buf));
    Nh_TTY_abAppend(&ab, "\x1b[?25h", 6);

 #ifdef __unix__
 
    write(STDOUT_FILENO, ab.b, ab.len);
    Nh_TTY_abFree(&ab);

#endif
}

// DRAW ===========================================================================================

static void Nh_TTY_drawRows(
    Nh_TTY_Terminal *Terminal_p, struct AppendBuffer *ab) 
{
    Nh_TTY_Tab *Tab_p = Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab);

    int y;
    for (y = 0; y < Terminal_p->screenRows; y++) {
        int filerow = y + Tab_p->rowOffset;
        if (filerow >= Tab_p->Rows.count) {
            if (Tab_p->Rows.count == 0 && y == Terminal_p->screenRows / 3) {
                char welcome[80];
                int welcomelen = snprintf(welcome, sizeof(welcome),
                  "%s %s%s TTY Terminal", NH_NAME, NH_VERSION, NH_PRE_RELEASE_VERSION);
                if (welcomelen > Terminal_p->screenCols) welcomelen = Terminal_p->screenCols;
                int padding = (Terminal_p->screenCols - welcomelen) / 2;
                if (padding) {
                    Nh_TTY_abAppend(ab, "~", 1);
                    padding--;
                }
                while (padding--) Nh_TTY_abAppend(ab, " ", 1);
                Nh_TTY_abAppend(ab, welcome, welcomelen);
            } else {
                Nh_TTY_abAppend(ab, "~", 1);
            }
        } else {
            int len = ((Nh_TTY_Row*)Nh_getListItem(&Tab_p->Rows, filerow))->rsize - Tab_p->colOffset;
            if (len < 0) len = 0;
            if (len > Terminal_p->screenCols) len = Terminal_p->screenCols;
            Nh_TTY_abAppend(ab, &((Nh_TTY_Row*)Nh_getListItem(&Tab_p->Rows, filerow))->render_p[Tab_p->colOffset], len);
        }
  
        Nh_TTY_abAppend(ab, "\x1b[K", 3);
        Nh_TTY_abAppend(ab, "\r\n", 2);
    }
}

static void Nh_TTY_drawStatusBar(
    Nh_TTY_Terminal *Terminal_p, struct AppendBuffer *ab) 
{
    Nh_TTY_Tab *Tab_p = Nh_getListItem(&Terminal_p->Tabs, Terminal_p->currentTab);

    Nh_TTY_abAppend(ab, "\x1b[7m", 4);
    char status[80], rstatus[80];

    int len = snprintf(status, sizeof(status), "%.20s - %d lines", Tab_p->Flags.logging ? "Logging active" : "Logging inactive", Tab_p->Rows.count);
    int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", Tab_p->cursorY + 1, Tab_p->Rows.count);
    if (len > Terminal_p->screenCols) len = Terminal_p->screenCols;
    Nh_TTY_abAppend(ab, status, len);

    while (len < Terminal_p->screenCols) {
        if (Terminal_p->screenCols - len == rlen) {
            Nh_TTY_abAppend(ab, rstatus, rlen);
            break;
        } else {
            Nh_TTY_abAppend(ab, " ", 1);
            len++;
        }
    }

    Nh_TTY_abAppend(ab, "\x1b[m", 3);
    Nh_TTY_abAppend(ab, "\r\n", 2);
}

static void Nh_TTY_drawCommandPrompt(
    Nh_TTY_Terminal *Terminal_p, struct AppendBuffer *ab) 
{
    Nh_TTY_abAppend(ab, "\x1b[K", 3);

    if (Terminal_p->focus == NH_TTY_FOCUS_COMMAND_PROMPT) {
        Nh_TTY_abAppend(ab, ":", 1);
        if (Terminal_p->CurrentCommand_p != NULL) {
            Nh_TTY_abAppend(ab, Nh_getChars(Terminal_p->CurrentCommand_p), strlen(Nh_getChars(Terminal_p->CurrentCommand_p)));
        }
    }
}

// HELPER ==========================================================================================

static void Nh_TTY_scroll(
    Nh_TTY_Terminal *Terminal_p, Nh_TTY_Tab *Tab_p) 
{
    Tab_p->rx = 0;
    if (Tab_p->cursorY < Tab_p->Rows.count) {
        Tab_p->rx = Nh_TTY_rowCxToRx(Nh_getListItem(&Tab_p->Rows, Tab_p->cursorY), Tab_p->cursorX);
    }
  
    if (Tab_p->cursorY < Tab_p->rowOffset) {
        Tab_p->rowOffset = Tab_p->cursorY;
    }
    if (Tab_p->cursorY >= Tab_p->rowOffset + Terminal_p->screenRows && Terminal_p->focus == NH_TTY_FOCUS_OUTPUT) {
        Tab_p->rowOffset = Tab_p->cursorY - Terminal_p->screenRows + 1;
    }
    if (Tab_p->rx < Tab_p->colOffset) {
        Tab_p->colOffset = Tab_p->rx;
    }
    if (Tab_p->rx >= Tab_p->colOffset + Terminal_p->screenCols) {
        Tab_p->colOffset = Tab_p->rx - Terminal_p->screenCols + 1;
    }
}

static void Nh_TTY_abAppend(
    struct AppendBuffer *ab, const char *s, int len) 
{
    char *new_p = realloc(ab->b, ab->len + len);
  
    if (new_p == NULL) return;
    memcpy(&new_p[ab->len], s, len);
    ab->b = new_p;
    ab->len += len;
}

static void Nh_TTY_abFree(
    struct AppendBuffer *ab) 
{
    free(ab->b);
}

