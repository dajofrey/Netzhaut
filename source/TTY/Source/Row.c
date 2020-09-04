// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// DEFINE ==========================================================================================

#define TAB_STOP 8

// INCLUDE =========================================================================================

#include "../Header/Row.h"
#include "../Header/Input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DECLARE =========================================================================================

static void Nh_TTY_updateRow(
    Nh_TTY_Row *row
); 

// ROW =============================================================================================

int Nh_TTY_rowCxToRx(
    Nh_TTY_Row *Row_p, int cx) 
{
    int rx = 0, j;
    for (j = 0; j < cx; j++) {
        if (Row_p->chars_p[j] == '\t') {
            rx += (TAB_STOP - 1) - (rx % TAB_STOP);
        }
        rx++;
    }
    return rx;
}

void Nh_TTY_addRow(
    Nh_TTY_Tab *Tab_p, const char *line_p, size_t len) 
{
    if (Nh_TTY_getTerminal()->Flags.simple) {
        if (line_p[len] != '\n') {printf("  %s\n", line_p);}
        else {printf("  %s", line_p);}
        return;
    }

    Nh_TTY_Row *Row_p = malloc(sizeof(Nh_TTY_Row));
    if (Row_p == NULL) {return;}
    Nh_addListItem(&Tab_p->Rows, Row_p);

    Row_p->size = len;
    Row_p->chars_p = malloc(len + 1);
    memcpy(Row_p->chars_p, line_p, len);
    Row_p->chars_p[len] = '\0';
  
    Row_p->rsize = 0;
    Row_p->render_p = NULL;
    Nh_TTY_updateRow(Row_p);
  
    Nh_TTY_moveCursor(Tab_p, NH_TTY_KEY_ARROW_DOWN);
    Tab_p->cursorX = 0;
}

void Nh_TTY_destroyRows(
    Nh_TTY_Tab *Tab_p)
{
    for (int i = 0; i < Tab_p->Rows.count; ++i) {
        Nh_TTY_Row *Row_p = Nh_getListItem(&Tab_p->Rows, i);
        free(Row_p->chars_p);
    }
    Nh_destroyList(&Tab_p->Rows, true);
}

// HELPER ==========================================================================================

static void Nh_TTY_updateRow(
    Nh_TTY_Row *Row_p) 
{
    int tabs = 0, j;
    for (j = 0; j < Row_p->size; j++) {if (Row_p->chars_p[j] == '\t') {tabs++;}}
  
    free(Row_p->render_p);
    Row_p->render_p = malloc(Row_p->size + tabs*(TAB_STOP - 1) + 1);
  
    int idx = 0;
    for (j = 0; j < Row_p->size; j++) {
        if (Row_p->chars_p[j] == '\t') {
            Row_p->render_p[idx++] = ' ';
            while (idx % TAB_STOP != 0) Row_p->render_p[idx++] = ' ';
        } else {
            Row_p->render_p[idx++] = Row_p->chars_p[j];
        }
    }
    Row_p->render_p[idx] = '\0';
    Row_p->rsize = idx;
}

