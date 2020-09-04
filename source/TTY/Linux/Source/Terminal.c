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

#include "../Header/Terminal.h"
#include "../Header/Input.h"

#include "../../Header/Terminal.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// DECLARE =========================================================================================

static void Nh_TTY_Linux_enableRawMode(
    Nh_TTY_Terminal *Terminal_p
); 

static void Nh_TTY_Linux_disableRawMode(
); 

// TERMINAL ========================================================================================

int Nh_TTY_Linux_getWindowSize(
    int *rows, int *cols) 
{
#ifdef __unix__

    struct winsize ws;
  
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        return Nh_TTY_Linux_getCursorPosition(rows, cols);
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
    }

#endif

    return 0;
}

void Nh_TTY_Linux_initTerminal(
    Nh_TTY_Terminal *Terminal_p) 
{
#ifdef __unix__

    Nh_TTY_Linux_enableRawMode(Terminal_p);

    if (Nh_TTY_Linux_getWindowSize(&Terminal_p->screenRows, &Terminal_p->screenCols) == -1) {
        Nh_TTY_Linux_die("getWindowSize");
    }

    Terminal_p->screenRows -= 2;

#endif
}

void Nh_TTY_Linux_die(
    const char *s) 
{
#ifdef __unix__

    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);

#endif
}

// HELPER ==========================================================================================

static void Nh_TTY_Linux_enableRawMode(
    Nh_TTY_Terminal *Terminal_p) 
{
#ifdef __unix__

    if (tcgetattr(STDIN_FILENO, &Terminal_p->Linux.origTermios) == -1) Nh_TTY_Linux_die("tcgetattr");
    atexit(Nh_TTY_Linux_disableRawMode);
  
    struct termios raw = Terminal_p->Linux.origTermios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
  
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) Nh_TTY_Linux_die("tcsetattr");

#endif
}

static void Nh_TTY_Linux_disableRawMode() 
{
#ifdef __unix__

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &Nh_TTY_getTerminal()->Linux.origTermios) == -1) {
        Nh_TTY_Linux_die("tcsetattr");
    }

#endif
}

