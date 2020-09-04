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

#define CTRL_KEY(k) ((k) & 0x1f)

// INCLUDE =========================================================================================

#include "../Header/Input.h"
#include "../../Header/Input.h"

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

// INPUT ===========================================================================================

int Nh_TTY_Linux_readKey() 
{
#ifdef __unix__

    fd_set set;
    FD_ZERO(&set);
    FD_SET(fileno(stdin), &set);
  
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
  
    int rv = select(fileno(stdin) + 1, &set, NULL, NULL, &timeout);
    if (rv == -1 || rv == 0) {return -1;}

    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {Nh_TTY_Linux_die("read");}
    }

    if (c == '\x1b') {
        char seq[3];
    
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
    
        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '1': return NH_TTY_KEY_HOME;
                        case '3': return NH_TTY_KEY_DEL;
                        case '4': return NH_TTY_KEY_END;
                        case '5': return NH_TTY_KEY_PAGE_UP;
                        case '6': return NH_TTY_KEY_PAGE_DOWN;
                        case '7': return NH_TTY_KEY_HOME;
                        case '8': return NH_TTY_KEY_END;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A': return NH_TTY_KEY_ARROW_UP;
                    case 'B': return NH_TTY_KEY_ARROW_DOWN;
                    case 'C': return NH_TTY_KEY_ARROW_RIGHT;
                    case 'D': return NH_TTY_KEY_ARROW_LEFT;
                    case 'H': return NH_TTY_KEY_HOME;
                    case 'F': return NH_TTY_KEY_END;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H': return NH_TTY_KEY_HOME;
                case 'F': return NH_TTY_KEY_END;
            }
        }
        return '\x1b';
    } else {
        return c;
    }

#endif
}

int Nh_TTY_Linux_getCursorPosition(
    int *rows, int *cols) 
{
#ifdef __unix__

    char buf[32];
    unsigned int i = 0;
  
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
  
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';
  
    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
  
    return 0;

#endif
}

