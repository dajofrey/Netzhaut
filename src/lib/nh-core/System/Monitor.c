// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Monitor.h"
#include "Logger.h"
#include "Memory.h"

#include "../Util/Time.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../../../external/TTyr/src/lib/ttyr-api/ttyr-tty.h"

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

// TYPES ===========================================================================================

typedef struct nh_core_MonitorNode {
    nh_core_LoggerNode *LoggerNode_p;
    bool isOpen;
    bool isCurrent;
    bool isSelected;
    bool hasFocus;
    long focusedRow;
    long offset;
    int peak;
    nh_core_List Children;
    struct nh_core_MonitorNode *Parent_p;
} nh_core_MonitorNode;

typedef struct nh_core_MonitorView {
    int height;
    int screenCursor;
    int rowOffset;
} nh_core_MonitorView;

typedef struct nh_core_Monitor {
    nh_core_MonitorView View;
    bool showCategories;
    int listingWidth;
    nh_core_MonitorNode Root;
    nh_core_SystemTime LastUpdate;
    double updateIntervalInSeconds;
} nh_core_Monitor;

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_core_updateMonitorNode(
    nh_core_LoggerNode *LoggerNode_p, nh_core_MonitorNode *MonitorNode_p)
{
    for (int i = 0; i < LoggerNode_p->Children.size; ++i) 
    {
        nh_core_LoggerNode *NextLoggerNode_p = LoggerNode_p->Children.pp[i];
        nh_core_MonitorNode *NextMonitorNode_p = nh_core_getFromList(&MonitorNode_p->Children, i);

        if (NextMonitorNode_p == NULL) 
        {
            NextMonitorNode_p = nh_core_allocate(sizeof(nh_core_MonitorNode));
            NH_CORE_CHECK_MEM(NextMonitorNode_p)

            NextMonitorNode_p->Children     = nh_core_initList(8);
            NextMonitorNode_p->LoggerNode_p = NextLoggerNode_p;
            NextMonitorNode_p->isOpen       = false;
            NextMonitorNode_p->isCurrent    = false;
            NextMonitorNode_p->isSelected   = false;
            NextMonitorNode_p->hasFocus     = false;
            NextMonitorNode_p->focusedRow   = 0;
            NextMonitorNode_p->offset       = 0;
            NextMonitorNode_p->peak         = 0;
            NextMonitorNode_p->Parent_p     = MonitorNode_p;

            nh_core_appendToList(&MonitorNode_p->Children, NextMonitorNode_p);
        }

        NH_CORE_CHECK(nh_core_updateMonitorNode(NextLoggerNode_p, NextMonitorNode_p))
    }

    return NH_API_SUCCESS;
}

static void nh_core_getMonitorNodes(
    nh_core_MonitorNode *Node_p, nh_core_List *List_p)
{
    if (!Node_p->isOpen) {return ;}

    for (int i = 0; i < Node_p->Children.size; ++i) 
    {
        nh_core_appendToList(List_p, Node_p->Children.pp[i]);
        nh_core_getMonitorNodes(Node_p->Children.pp[i], List_p);
    }

    return;
}

static int nh_core_getMonitorNodeDepth(
    nh_core_MonitorNode *Node_p)
{
    int depth = 0;
    nh_core_MonitorNode *Parent_p = Node_p->Parent_p;
    while ((Parent_p = Parent_p->Parent_p) != NULL) {
        depth++;
    }

    return depth;
}

static int nh_core_getCategoryListingWidth(
    nh_core_Monitor *Monitor_p)
{
    nh_core_List Nodes = nh_core_initList(16);
    nh_core_getMonitorNodes(&Monitor_p->Root, &Nodes);

    int maxWidth = 0;
    nh_core_MonitorNode *Node_p = NULL;
    for (int i = 0; (Node_p = nh_core_getFromList(&Nodes, i)) != NULL; ++i) {
        int width = (nh_core_getMonitorNodeDepth(Node_p) * 2) + strlen(Node_p->LoggerNode_p->name_p);
        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    nh_core_freeList(&Nodes, false);

    return maxWidth + 2;
}

static NH_API_RESULT nh_core_updateMonitor(
    ttyr_tty_Program *Program_p)
{
    nh_core_Monitor *Monitor_p = Program_p->handle_p;

    nh_core_SystemTime Now = nh_core_getSystemTime();
    if (nh_core_getSystemTimeDiffInSeconds(Monitor_p->LastUpdate, Now) < Monitor_p->updateIntervalInSeconds) {
        // Skip update if update-interval not met.
        return NH_API_SUCCESS;
    }

    bool init = Monitor_p->Root.Children.size == 0;

    nh_core_updateMonitorNode(&NH_LOGGER.Root, &Monitor_p->Root);

    if (init && Monitor_p->Root.Children.size > 0) {
        ((nh_core_MonitorNode*)Monitor_p->Root.Children.pp[0])->isCurrent = true;
    }

    Monitor_p->listingWidth = nh_core_getCategoryListingWidth(Monitor_p);
    Monitor_p->LastUpdate = Now;

    // force screen refresh
    Program_p->refresh = true;

    return NH_API_SUCCESS;
}

// INPUT ===========================================================================================

static nh_core_List nh_core_getSelectedMonitorNodes(
    nh_core_Monitor *Monitor_p)
{
    nh_core_List SelectedNodes = nh_core_initList(8);
    nh_core_List Nodes = nh_core_initList(32);
    nh_core_getMonitorNodes(&Monitor_p->Root, &Nodes);

    for (int i = 0; i < Nodes.size; ++i) {
        if (((nh_core_MonitorNode*)Nodes.pp[i])->isSelected) {
            nh_core_appendToList(&SelectedNodes, Nodes.pp[i]);
        }
    }

    nh_core_freeList(&Nodes, false);

    return SelectedNodes;
}

static void nh_core_resetMonitorFocuses(
    nh_core_Monitor *Monitor_p)
{
    nh_core_List SelectedNodes = nh_core_getSelectedMonitorNodes(Monitor_p);
    for (int i = 0; i < SelectedNodes.size; ++i) {
        ((nh_core_MonitorNode*)SelectedNodes.pp[i])->hasFocus = false;
    }
    nh_core_freeList(&SelectedNodes, false);

    return;
}

static nh_core_MonitorNode *nh_core_getFocusedMonitorNode(
    nh_core_Monitor *Monitor_p)
{
    nh_core_List SelectedNodes = nh_core_getSelectedMonitorNodes(Monitor_p);
    if (SelectedNodes.size <= 0) {return NULL;}

    int focus;
    for (focus = 0; focus < SelectedNodes.size && !((nh_core_MonitorNode*)SelectedNodes.pp[focus])->hasFocus; ++focus);
    nh_core_MonitorNode *Node_p = SelectedNodes.pp[focus];

    nh_core_freeList(&SelectedNodes, false);

    return Node_p;
}

static nh_core_MonitorNode *nh_core_getCurrentMonitorNode(
    nh_core_MonitorNode *Node_p)
{
    if (Node_p->isCurrent) {
        return Node_p;
    }
    else
    {
        for (int i = 0; Node_p->isOpen && i < Node_p->Children.size; ++i) 
        {
            nh_core_MonitorNode *Result_p = 
                nh_core_getCurrentMonitorNode(Node_p->Children.pp[i]);
            if (Result_p != NULL) {return Result_p;}
        }
    }

    return NULL;
}

static void nh_core_moveCursorVertically(
    ttyr_tty_Program *Program_p, nh_core_MonitorNode *Current_p, int key) 
{
    nh_core_Monitor *Monitor_p = Program_p->handle_p;

    nh_core_List Nodes = nh_core_initList(16);
    nh_core_getMonitorNodes(&Monitor_p->Root, &Nodes);
    int index;
    for (index = 0; index < Nodes.size && Nodes.pp[index] != Current_p; ++index);

    switch (key) 
    {
        case 'w' :
            if (index > 0) 
            {
                Current_p->isCurrent = false;
                ((nh_core_MonitorNode*)Nodes.pp[index - 1])->isCurrent = true;

                if (Monitor_p->View.screenCursor == 0 && Monitor_p->View.rowOffset > 0) {Monitor_p->View.rowOffset--;}
                else if (Monitor_p->View.screenCursor > 0) {Monitor_p->View.screenCursor--;}
            }
            break;
        case 's' :
            if (Nodes.size > index + 1) 
            {
                Current_p->isCurrent = false;
                ((nh_core_MonitorNode*)Nodes.pp[index + 1])->isCurrent = true;

                if (Monitor_p->View.screenCursor < Monitor_p->View.height - 1) {Monitor_p->View.screenCursor++;}
                else {Monitor_p->View.rowOffset++;}
            }
            break;
    }

    nh_core_freeList(&Nodes, false);

    return;
}

static void nh_core_unselectMonitorNode(
    nh_core_Monitor *Monitor_p, nh_core_MonitorNode *Selected_p)
{
    nh_core_List SelectedNodes = nh_core_getSelectedMonitorNodes(Monitor_p);
    
    if (SelectedNodes.size > 1 && Selected_p->hasFocus) 
    {
        int index;
        for (index = 0; index < SelectedNodes.size && SelectedNodes.pp[index] != Selected_p; ++index);

        if (index == 0) {((nh_core_MonitorNode*)SelectedNodes.pp[SelectedNodes.size - 1])->hasFocus = true;}
        else if (index == SelectedNodes.size - 1) {
            ((nh_core_MonitorNode*)SelectedNodes.pp[0])->hasFocus = true;
        }
        else {
            ((nh_core_MonitorNode*)SelectedNodes.pp[index - 1])->hasFocus = true;
        }
    }

    Selected_p->isSelected = false;
    Selected_p->hasFocus = false;

    nh_core_freeList(&SelectedNodes, false);

    return;
}

static void nh_core_changeFocus(
    nh_core_Monitor *Monitor_p, int c)
{
    nh_core_List SelectedNodes = nh_core_getSelectedMonitorNodes(Monitor_p);
    if (SelectedNodes.size <= 1) {return ;}

    int focus;
    for (focus = 0; focus < SelectedNodes.size && !((nh_core_MonitorNode*)SelectedNodes.pp[focus])->hasFocus; ++focus);

    switch (c) 
    {
        case 'f' :
            if (focus > 0) {
                ((nh_core_MonitorNode*)SelectedNodes.pp[focus])->hasFocus = false;
                ((nh_core_MonitorNode*)SelectedNodes.pp[focus - 1])->hasFocus = true;
            }
            break;
        case 'g' :
            if (focus < SelectedNodes.size - 1) {
                ((nh_core_MonitorNode*)SelectedNodes.pp[focus])->hasFocus = false;
                ((nh_core_MonitorNode*)SelectedNodes.pp[focus + 1])->hasFocus = true;
            }
            break;
    }

    nh_core_freeList(&SelectedNodes, false);

    return;
}

static NH_API_RESULT nh_core_handleMonitorInput(
    ttyr_tty_Program *Program_p, nh_api_WSIEvent Event)
{
    if (Event.type != NH_API_WSI_EVENT_KEYBOARD) {
        return NH_API_SUCCESS;
    }

    if (Event.Keyboard.trigger != NH_API_TRIGGER_PRESS) {return NH_API_SUCCESS;}

    NH_ENCODING_UTF32 c = Event.Keyboard.codepoint;

    nh_core_Monitor *Monitor_p = Program_p->handle_p;
    nh_core_MonitorNode *Current_p = nh_core_getCurrentMonitorNode(&Monitor_p->Root);
    if (Current_p == NULL) {return NH_API_SUCCESS;}

    nh_core_MonitorNode *Node_p = nh_core_getFocusedMonitorNode(Monitor_p);

    switch (c)
    {
        case 'w' :
        case 's' :
            nh_core_moveCursorVertically(Program_p, Current_p, c);
            break;

        case 'f' :
        case 'g' :
            nh_core_changeFocus(Monitor_p, c);
            break;

        case 'h' :
            if (Node_p != NULL && Node_p->peak > 0) {Node_p->peak--;}
            break;
        case 'l' :
            if (Node_p != NULL) {Node_p->peak++;}
            break;

        case 'j' :
            if (Node_p != NULL && Node_p->LoggerNode_p->Messages.size > Node_p->focusedRow + 1) {
                Node_p->focusedRow++;
                Node_p->peak = 0;
            }
            break;
        case 'k' :
            if (Node_p != NULL) {
                Node_p->focusedRow--;
                Node_p->peak = 0;
            }
            break;

        case 'a' :
            if (Current_p->isSelected) {
                nh_core_unselectMonitorNode(Monitor_p, Current_p);
            }
            else if (Current_p->Parent_p->Parent_p != NULL) 
            {
                for (int i = 0; i < Current_p->Parent_p->Children.size; ++i) 
                {
                    nh_core_MonitorNode *Sibling_p = Current_p->Parent_p->Children.pp[i];
                    if (Sibling_p->isSelected) {
                        nh_core_unselectMonitorNode(Monitor_p, Sibling_p);
                    } 
                    Sibling_p->isCurrent = false;
                }
                Current_p->Parent_p->isOpen = false;
                Current_p->Parent_p->isCurrent = true;
            }
            break;

        case 'd' :
            Current_p->isOpen = true;
            if (Current_p->LoggerNode_p->Messages.size == 0) {
                Monitor_p->listingWidth = nh_core_getCategoryListingWidth(Monitor_p);
            }
            else {
                nh_core_resetMonitorFocuses(Monitor_p);
                Current_p->isSelected = true;
                Current_p->hasFocus = true;
            }
            break;

        case 'q' :
            Program_p->close = true;
            break;

//        case CTRL_KEY('b') :
//            Monitor_p->showCategories = !Monitor_p->showCategories; 
//            break;
    }
 
    Program_p->refresh = true;

    return NH_API_SUCCESS;
}

// DRAW ============================================================================================

static void nh_core_setNextGlyph(
    ttyr_tty_Glyph **Glyphs_pp, NH_ENCODING_UTF32 codepoint)
{
    ttyr_tty_Glyph Glyph;
    memset(&Glyph, 0, sizeof(ttyr_tty_Glyph));
    Glyph.Attributes.reverse = false;
    Glyph.codepoint = codepoint;
 
    (*Glyphs_pp)[0] = Glyph;
    (*Glyphs_pp) = (*Glyphs_pp)+1;

    return;
}

static const char *help_pp[] =
{
    "Netzhaut Monitor                             ",
    "",
    "- Keyboard Controls -                        ",
    "",
    "[w][a][s][d] Navigate logging categories.    ",
    "[h][j][k][l] Navigate log entries.           ",
    "[f][g]       Jump to left/right selected log.",
    "[c]          Copy selected log entry.        ",
    "[b]          Toggle logging categories.      ",
    "[q]          Quit.                           ",
    "",
};

static NH_API_RESULT nh_core_drawHelp(
    ttyr_tty_Glyph **Glyphs_pp, int row, int cols, int rows)
{
    int entries = sizeof(help_pp) / sizeof(help_pp[0]);
    int begin = (rows / 2) - (entries / 2);

    if (row >= begin && row < begin + entries) {
        int sides = cols < strlen(help_pp[row-begin]) ? 0 
            : (cols - strlen(help_pp[row - begin])) / 2;
        for (int i = 0; i < sides; ++i) {
            nh_core_setNextGlyph(Glyphs_pp, ' ');
        }
        for (int i = 0; i < strlen(help_pp[row - begin]) && i < cols; ++i) {
            nh_core_setNextGlyph(Glyphs_pp, help_pp[row - begin][i]);
        }
    }
    else {
        for (int i = 0; i < cols; ++i) {
            nh_core_setNextGlyph(Glyphs_pp, ' ');
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_drawSelected(
    nh_core_Monitor *Monitor_p, ttyr_tty_Glyph **Glyphs_pp, int row, int cols, int rows)
{
    nh_core_List SelectedNodes = nh_core_getSelectedMonitorNodes(Monitor_p);
    cols = SelectedNodes.size > 0 ? cols / SelectedNodes.size : cols;

    for (int i = 0; i < SelectedNodes.size; ++i)
    {
        nh_core_MonitorNode *SelectedNode_p = SelectedNodes.pp[i]; 

        if (SelectedNode_p->focusedRow > rows - 1) {
            SelectedNode_p->offset++;
            SelectedNode_p->focusedRow = rows - 1;
        }
        else if (SelectedNode_p->focusedRow < 0) {
            if (SelectedNode_p->offset > 0) {SelectedNode_p->offset--;}
            SelectedNode_p->focusedRow = 0;
        }

        nh_core_List *Messages_p = &SelectedNode_p->LoggerNode_p->Messages;
        char *message_p = nh_core_getFromList(Messages_p, row + SelectedNode_p->offset);

        if (message_p != NULL) 
        {
            if (SelectedNode_p->hasFocus && SelectedNode_p->focusedRow == row && strlen(message_p) > cols) {
                for (int i = 0; i < SelectedNode_p->peak && strlen(message_p) > cols; ++i) {
                    message_p = message_p + (cols - 1);
                }
            }

            if (SelectedNode_p->hasFocus && SelectedNode_p->focusedRow == row) {
                nh_core_setNextGlyph(Glyphs_pp, '>');
            }
            else {
                nh_core_setNextGlyph(Glyphs_pp, ' ');
            }

            int length = strlen(message_p) + 1 > cols ? cols - 1: strlen(message_p);
            for (int i = 0; i < length; ++i) {
                nh_core_setNextGlyph(Glyphs_pp, message_p[i]);
            }
 
            for (int j = 0; j < cols - (length + 1); ++j) {
                nh_core_setNextGlyph(Glyphs_pp, ' ');
            }
        }
        else {
            for (int j = 0; j < cols; ++j) {nh_core_setNextGlyph(Glyphs_pp, ' ');}
        }
    }

    if (SelectedNodes.size == 0) {nh_core_drawHelp(Glyphs_pp, row, cols, rows);}

    nh_core_freeList(&SelectedNodes, false);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_drawMonitorRow(
    ttyr_tty_Program *Program_p, ttyr_tty_Glyph *Glyphs_p, int width, int height, int row)
{
    nh_core_Monitor *Monitor_p = Program_p->handle_p;
    Monitor_p->View.height = height;

    int tmp = -1;
    nh_core_List Nodes = nh_core_initList(16);
    nh_core_getMonitorNodes(&Monitor_p->Root, &Nodes);
    nh_core_MonitorNode *Node_p = nh_core_getFromList(&Nodes, row + Monitor_p->View.rowOffset);

    ttyr_tty_Glyph *Tmp_p = Glyphs_p;
    for (int i = 0; i < width; ++i) {
        nh_core_setNextGlyph(&Glyphs_p, ' ');
    }
    Glyphs_p = Tmp_p;
 
    if (Monitor_p->showCategories)
    {
        if (Node_p != NULL && row < Nodes.size) 
        {
            int offset = nh_core_getMonitorNodeDepth(Node_p);
            for (int i = 0; i < offset; ++i) {
                nh_core_setNextGlyph(&Glyphs_p, ' ');
                nh_core_setNextGlyph(&Glyphs_p, ' ');
            }
            offset *= 2;
            offset += 1;

            if (Node_p->isCurrent) {nh_core_setNextGlyph(&Glyphs_p, '>');}
            else if (Node_p->isSelected) {nh_core_setNextGlyph(&Glyphs_p, '>');}
            else {nh_core_setNextGlyph(&Glyphs_p, ' ');}

            for (int i = 0; i < strlen(Node_p->LoggerNode_p->name_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, Node_p->LoggerNode_p->name_p[i]);
            }

            for (int i = 0; i < Monitor_p->listingWidth - (strlen(Node_p->LoggerNode_p->name_p) + offset); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, ' ');
            }
        }
        else {
            for (int i = 0; i < Monitor_p->listingWidth; ++i) {
                nh_core_setNextGlyph(&Glyphs_p, ' ');
            }
        }

        NH_CORE_CHECK(nh_core_drawSelected(Monitor_p, &Glyphs_p, row, width - (Monitor_p->listingWidth), height))
    }
    else {NH_CORE_CHECK(nh_core_drawSelected(Monitor_p, &Glyphs_p, row, width, height))}

    nh_core_freeList(&Nodes, false);

    return NH_API_SUCCESS;
}

// INIT/DESTROY ====================================================================================

static void *nh_core_initMonitor(
    void *arg_p)
{
    nh_core_Monitor *Monitor_p = nh_core_allocate(sizeof(nh_core_Monitor));
    NH_CORE_CHECK_MEM_2(NULL, Monitor_p)

    memset(&Monitor_p->View, 0, sizeof(nh_core_MonitorView));

    Monitor_p->showCategories = true;
    Monitor_p->listingWidth   = 0;
    Monitor_p->LastUpdate     = nh_core_getSystemTime();
    Monitor_p->updateIntervalInSeconds = 1.0;

    Monitor_p->Root.LoggerNode_p = &NH_LOGGER.Root;
    Monitor_p->Root.isOpen       = true;
    Monitor_p->Root.isCurrent    = false;
    Monitor_p->Root.Children     = nh_core_initList(4);
    Monitor_p->Root.Parent_p     = NULL;

    return Monitor_p;
}

static void nh_core_destroyMonitor(
    void *handle_p)
{
    nh_core_free(handle_p);

    return;
}

// PROTOTYPE =======================================================================================

void *nh_core_createMonitorInterface()
{
    ttyr_tty_Interface *Monitor_p = nh_core_allocate(sizeof(ttyr_tty_Interface));
    NH_CORE_CHECK_MEM_2(NULL, Monitor_p)

    memset(Monitor_p, 0, sizeof(ttyr_tty_Interface));

    Monitor_p->Callbacks.init_f = nh_core_initMonitor;
    Monitor_p->Callbacks.draw_f = nh_core_drawMonitorRow;
    Monitor_p->Callbacks.handleInput_f = nh_core_handleMonitorInput;
    Monitor_p->Callbacks.update_f = nh_core_updateMonitor;
    Monitor_p->Callbacks.destroy_f = nh_core_destroyMonitor;

    return Monitor_p;
}

void nh_core_freeMonitorInterface(
    void *Interface_p)
{
    nh_core_free(Interface_p);

    return;
}
