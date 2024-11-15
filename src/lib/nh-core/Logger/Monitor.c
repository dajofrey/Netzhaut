// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Monitor.h"
#include "Logger.h"
#include "Server.h"

#include "../System/Memory.h"
#include "../Util/Time.h"
#include "../Common/Config.h"
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

// FUNCTIONS =======================================================================================

static nh_core_MonitorNode nh_core_initMonitorNode(
    nh_core_LoggerNode *LoggerNode_p, bool isOpen, nh_core_MonitorNode *Parent_p)
{
    nh_core_MonitorNode Node = {0};
    memset(&Node, 0, sizeof(nh_core_MonitorNode));

    Node.Children     = nh_core_initList(8);
    Node.LoggerNode_p = LoggerNode_p;
    Node.isOpen       = isOpen;
    Node.Parent_p     = Parent_p;

    return Node; 
}

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
            *NextMonitorNode_p = nh_core_initMonitorNode(NextLoggerNode_p, false, MonitorNode_p);
            nh_core_appendToList(&MonitorNode_p->Children, NextMonitorNode_p);
        }

        NH_CORE_CHECK(nh_core_updateMonitorNode(NextLoggerNode_p, NextMonitorNode_p))
    }

    return NH_API_SUCCESS;
}

static void nh_core_getPreviewNodes(
    nh_core_MonitorNode *Node_p, nh_core_List *List_p)
{
    nh_core_MonitorNode *Child_p = NULL;
    for (int i = 0; i < Node_p->Children.size; ++i) {
        if (((nh_core_MonitorNode*)Node_p->Children.pp[i])->isOpen) {
            Child_p = Node_p->Children.pp[i];
        }
    }

    if (Child_p) {
        nh_core_getPreviewNodes(Child_p, List_p);
    } else {
        for (int i = 0; i < Node_p->Children.size; ++i) {
            nh_core_appendToList(List_p, Node_p->Children.pp[i]);
        }
    }

    return;
}

static void nh_core_getMonitorNodes2(
    nh_core_MonitorNode *Node_p, nh_core_List *List_p)
{
    if (!Node_p->isOpen && !Node_p->isCurrent) {return;}

    if (Node_p->Parent_p) {
        nh_core_appendToList(List_p, Node_p);
    }

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_core_getMonitorNodes2(Node_p->Children.pp[i], List_p);
    }

    return;
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

static void nh_core_freeMonitorNodes(
    nh_core_MonitorNode *Node_p)
{
    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_core_freeMonitorNodes(Node_p->Children.pp[i]);
    } 
    nh_core_freeList(&Node_p->Children, true);

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

int nh_core_splitByTwoPluses(
    char *input, char **part1, char **part2, char **part3) 
{
    // Find the first occurrence of '+'
    char *first_plus = strchr(input, '\x1F');
    if (first_plus == NULL) {
        return 0;  // First '+' not found
    }

    // Find the second occurrence of '+'
    char *second_plus = strchr(first_plus + 1, '\x1F');
    if (second_plus == NULL) {
        return 0;  // Second '+' not found
    }

    // Split the string into three parts
    *first_plus = '\0';         // End the first part
    *part1 = input;             // First part starts at the beginning

    if (first_plus + 1 == second_plus) {
        // If there's nothing between the two pluses, set part2 to NULL
        *part2 = NULL;
    } else {
        // Otherwise, set part2 to the substring between the pluses
        *part2 = first_plus + 1;
        *second_plus = '\0';    // End the second part
    }

    *part3 = second_plus + 1;   // Third part starts after the second '+'
    return 1;  // Split successful
}

static NH_API_RESULT nh_core_updatePeerMonitor(
    nh_core_Monitor *Monitor_p)
{
    static char buffer_p[256001] = {0};

    switch (Monitor_p->Peer.state)
    {
        case 0:
            Monitor_p->Peer.server_fd = nh_core_createMonitorSocket(nh_core_getConfig().monitorPort);
            if (Monitor_p->Peer.server_fd < 0) {
                break;
            }
            Monitor_p->Peer.state = 1;
            break;

        case 1:
            Monitor_p->Peer.client_socket = nh_core_acceptLogger(Monitor_p->Peer.server_fd);
            if (Monitor_p->Peer.client_socket < 0) {
                break;
            }
            Monitor_p->Peer.state = 2;
            break;

        case 2:
        case 3:
        {
            int received = 0, counter = 0;
            do {
                memset(buffer_p, 0, sizeof(buffer_p));
                received = nh_core_receiveMessageFromLogger(Monitor_p->Peer.client_socket, buffer_p, sizeof(buffer_p)-1, 256);
                if (received == 0) {
                    // Connection closed bro.
                    nh_core_closeMonitorSockets(Monitor_p->Peer.client_socket, Monitor_p->Peer.server_fd);
                    nh_core_freeLogger(&Monitor_p->Peer.Logger);
                    nh_core_initLogger(&Monitor_p->Peer.Logger);
                    nh_core_freeMonitorNodes(&Monitor_p->Root);
                    Monitor_p->Peer.Root = nh_core_initMonitorNode(NULL, true, NULL);
                    Monitor_p->Peer.state = 0;
                    break;
                }
                if (received < 0) {break;}
                nh_core_sendLoggerMessageAck(Monitor_p->Peer.client_socket);
                char *buffer1_p = NULL;
                char *buffer2_p = NULL;
                char *buffer3_p = NULL;
                for (int i = 0; i < received; i += 256) {
                    if (nh_core_splitByTwoPluses(buffer_p+i, &buffer1_p, &buffer2_p, &buffer3_p)) {
                        nh_core_handleLogMessage(&(Monitor_p->Peer.Logger), buffer1_p, buffer2_p, buffer3_p);
                    } else {
                        puts("sgewegew");
                        exit(0);
                    }
                }
                Monitor_p->Peer.Root.LoggerNode_p = &(Monitor_p->Peer.Logger.Root);
                Monitor_p->Peer.state = 3;
            } while (counter++ < 100);

            break;
        }
    }
}
 
static NH_API_RESULT nh_core_updateMonitor(
    ttyr_tty_Program *Program_p)
{
    nh_core_Monitor *Monitor_p = Program_p->handle_p;

    nh_core_SystemTime Now = nh_core_getSystemTime();
    if (nh_core_getSystemTimeDiffInSeconds(Monitor_p->LastUpdate, Now) < Monitor_p->updateIntervalInSeconds) {
        return NH_API_SUCCESS;
    }

    nh_core_MonitorNode *Root_p = Monitor_p->Peer.state == 3 ? &(Monitor_p->Peer.Root) : &(Monitor_p->Root);

    bool init = Root_p->Children.size == 0;
    nh_core_updateMonitorNode(Root_p->LoggerNode_p, Root_p);

    if (init && Root_p->Children.size > 0) {
        ((nh_core_MonitorNode*)Root_p->Children.pp[0])->isCurrent = true;
    }

    if (nh_core_getConfig().monitorPort > 0) {
        nh_core_updatePeerMonitor(Monitor_p);
    }

    Monitor_p->LastUpdate = Now;
    Program_p->refresh = true;

    return NH_API_SUCCESS;
}

// INPUT ===========================================================================================

static nh_core_List nh_core_getSelectedMonitorNodes(
    nh_core_Monitor *Monitor_p)
{
    nh_core_List SelectedNodes = nh_core_initList(8);
    nh_core_List Nodes = nh_core_initList(32);
    nh_core_getMonitorNodes(Monitor_p->Peer.state == 3 ? &(Monitor_p->Peer.Root) : &(Monitor_p->Root), &Nodes);

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
    nh_core_getPreviewNodes(Monitor_p->Peer.state == 3 ? &(Monitor_p->Peer.Root) : &(Monitor_p->Root), &Nodes);

    int index;
    for (index = 0; index < Nodes.size && Nodes.pp[index] != Current_p; ++index);

    switch (key) 
    {
        case 'w' :
            if (index <= 0) {break;}
            Current_p->isCurrent = false;
            Current_p->isOpen = false;
            ((nh_core_MonitorNode*)Nodes.pp[index - 1])->isCurrent = true;
            if (Current_p->Parent_p->nodesYCursor > 0)      {Current_p->Parent_p->nodesYCursor--;}
            else if (Current_p->Parent_p->nodesYOffset > 0) {Current_p->Parent_p->nodesYOffset--;}
            break;
        case 's' :
            if (Nodes.size == index+1) {break;}
            if (Current_p->Parent_p->nodesYCursor < Nodes.size && Current_p->Parent_p->nodesYCursor < Monitor_p->height/2-2) {
                Current_p->Parent_p->nodesYCursor++;
            } else {
                Current_p->Parent_p->nodesYOffset++;
            } 
            Current_p->isCurrent = false;
            Current_p->isOpen = false;
            ((nh_core_MonitorNode*)Nodes.pp[index + 1])->isCurrent = true;
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
    nh_core_MonitorNode *Current_p = 
        nh_core_getCurrentMonitorNode(
            Monitor_p->Peer.state == 3 ? &(Monitor_p->Peer.Root) : &(Monitor_p->Root));
    if (Current_p == NULL) {return NH_API_SUCCESS;}

    nh_core_List SelectedNodes = nh_core_getSelectedMonitorNodes(Monitor_p);
    bool selected = SelectedNodes.size > 0;
    nh_core_freeList(&SelectedNodes, false);

    nh_core_MonitorNode *Node_p = nh_core_getFocusedMonitorNode(Monitor_p);

    switch (c)
    {
        case 's' :
            if (selected) {
                if (Node_p != NULL && Node_p->LoggerNode_p->Messages.size > Node_p->focusedRow + 1) {
                    Node_p->focusedRow++;
                    Node_p->peak = 0;
                }
            } else {
                nh_core_moveCursorVertically(Program_p, Current_p, c);
            }
            break;

        case 'w' :
            if (selected) {
                if (Node_p != NULL) {
                    Node_p->focusedRow--;
                    Node_p->peak = 0;
                }
            } else {
                nh_core_moveCursorVertically(Program_p, Current_p, c);
            }
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

        case 'a' :
            if (Current_p->Parent_p->Parent_p == NULL) {
                 break;
            }
            if (Current_p->isSelected) {
                nh_core_unselectMonitorNode(Monitor_p, Current_p);
                Current_p->isOpen = false;
                Current_p->isCurrent = true;
                Current_p->hasFocus = true;
            } else {
                Current_p->isCurrent = false;
                Current_p->hasFocus = false;
                Current_p->isOpen = false;
                Current_p->Parent_p->isOpen = false;
                Current_p->Parent_p->isCurrent = true;
                Current_p->Parent_p->hasFocus = true;
                Current_p->Parent_p->nodesYOffset = 0;
            }
            break;

        case 'd' :
            Current_p->isOpen = true;
            Current_p->isCurrent = false;
            if (Current_p->LoggerNode_p->Messages.size > 0) {
                Current_p->isSelected = true;
                Current_p->hasFocus = true;
                Current_p->isCurrent = true;
            } else {
                ((nh_core_MonitorNode*)Current_p->Children.pp[0])->isCurrent = true;
            }

            break;

        case 'q' :
            Program_p->close = true;
            break;
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
    Glyph.codepoint = codepoint;
 
    (*Glyphs_pp)[0] = Glyph;
    (*Glyphs_pp) = (*Glyphs_pp)+1;

    return;
}

static void nh_core_setNextReverseGlyph(
    ttyr_tty_Glyph **Glyphs_pp, NH_ENCODING_UTF32 codepoint)
{
    ttyr_tty_Glyph Glyph;
    memset(&Glyph, 0, sizeof(ttyr_tty_Glyph));
    Glyph.Attributes.reverse = true;
    Glyph.codepoint = codepoint;
 
    (*Glyphs_pp)[0] = Glyph;
    (*Glyphs_pp) = (*Glyphs_pp)+1;

    return;
}

static NH_API_RESULT nh_core_drawSelected(
    nh_core_Monitor *Monitor_p, ttyr_tty_Glyph **Glyphs_pp, int row, int cols, int rows)
{
    nh_core_List SelectedNodes = nh_core_getSelectedMonitorNodes(Monitor_p);
    if (SelectedNodes.size <= 0) {return NH_API_SUCCESS;}

    nh_core_MonitorNode *SelectedNode_p = SelectedNodes.pp[0]; 
 
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
            nh_core_setNextGlyph(Glyphs_pp, ' ');
            nh_core_setNextGlyph(Glyphs_pp, '>');
            nh_core_setNextGlyph(Glyphs_pp, ' ');
        }
        else {
            nh_core_setNextGlyph(Glyphs_pp, ' ');
            nh_core_setNextGlyph(Glyphs_pp, ' ');
            nh_core_setNextGlyph(Glyphs_pp, ' ');
        }
 
        int length = strlen(message_p) > cols - 3 ? cols - 3: strlen(message_p);
        for (int i = 0; i < length; ++i) {
            nh_core_setNextGlyph(Glyphs_pp, message_p[i]);
        }
    }

    nh_core_freeList(&SelectedNodes, false);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_drawMonitorRow(
    ttyr_tty_Program *Program_p, ttyr_tty_Glyph *Glyphs_p, int width, int height, int row)
{
    nh_core_Monitor *Monitor_p = Program_p->handle_p;
    Monitor_p->height = height;

    int tmp = -1;
    nh_core_List Nodes = nh_core_initList(16);
    nh_core_getPreviewNodes(
        Monitor_p->Peer.state == 3 ? &(Monitor_p->Peer.Root) : &(Monitor_p->Root), &Nodes);

    ttyr_tty_Glyph *Tmp_p = Glyphs_p;
    for (int i = 0; i < width; ++i) {
        nh_core_setNextGlyph(&Glyphs_p, ' ');
    }
    Glyphs_p = Tmp_p;

    if (Monitor_p->Peer.state > 0 && Monitor_p->Peer.state < 3) {
        if (row == 0) {
            char title_p[] = " Log Selector ";
            for (int i = 0; i < width - strlen(title_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, '-');
            }
            for (int i = 0; i < strlen(title_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, title_p[i]);
            }
        } else if (row == 1) {
            char state_p[64] = {0};
            switch (Monitor_p->Peer.state) {
                case 0: 
                    sprintf(state_p, "Trying to open port %d", nh_core_getConfig().monitorPort);
                    break;
                case 1: 
                    sprintf(state_p, "Listening on 127.0.0.1:%d", nh_core_getConfig().monitorPort);
                    break;
                case 2:
                    sprintf(state_p, "Accepted connection, waiting for messages");
                    break;
            }
            for (int i = 0; i < strlen(state_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, state_p[i]);
            } 
        } else if (row == 2) {
            char title_p[255] = {0};
            if (Monitor_p->Peer.state > 0) {
                sprintf(title_p, " Log Viewer ");
            } else {
                sprintf(title_p, " Log Viewer ");
        }
 
            for (int i = 0; i < width - strlen(title_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, '-');
            }
            for (int i = 0; i < strlen(title_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, title_p[i]);
            }
        } else if (row == height - 2) {
            char title_p[255] = {0};
            sprintf(title_p, " %s | %d logs ", nh_core_getConfig().monitorName_p, Monitor_p->Peer.Logger.totalLogs);
            for (int i = 0; i < width - strlen(title_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, '-');
            }
            for (int i = 0; i < strlen(title_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, title_p[i]);
            }
        } else if (row == height - 1) {
            char footer_p[244] = {0};
            sprintf(footer_p, "[w][a][s][d] Navigate"); 
            for (int i = 0; i < strlen(footer_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, footer_p[i]);
            }
        }
    } else if (row == 0) {
        char title_p[] = " Log Selector ";
        for (int i = 0; i < width - strlen(title_p); ++i) {
            nh_core_setNextGlyph(&Glyphs_p, '-');
        }
        for (int i = 0; i < strlen(title_p); ++i) {
            nh_core_setNextGlyph(&Glyphs_p, title_p[i]);
        }
    } else if (row == 1) {
        Monitor_p->nodesXOffset = 0;
        nh_core_List Nodes2 = nh_core_initList(16);
        nh_core_getMonitorNodes2(
            Monitor_p->Peer.state == 3 ? &(Monitor_p->Peer.Root) : &(Monitor_p->Root), &Nodes2);
        nh_core_MonitorNode *LastNode_p = NULL;
        for (int i = 0; i < Nodes2.size; ++i) {
            nh_core_MonitorNode *Node2_p = Nodes2.pp[i];
            if (Nodes2.size == i+1 && !Node2_p->isSelected) {
                Node2_p = Node2_p->Parent_p->Children.pp[Node2_p->Parent_p->nodesYOffset];
            }
            if (LastNode_p) {
                Monitor_p->nodesXOffset +=strlen(LastNode_p->LoggerNode_p->name_p);
            }
            nh_core_setNextGlyph(&Glyphs_p, ' ');
            if (Nodes2.size == i+1) {
                if (Node2_p->isCurrent || Node2_p->isSelected) {
                    nh_core_setNextGlyph(&Glyphs_p, '>');
                } else {
                    nh_core_setNextGlyph(&Glyphs_p, ' ');
                }
            } else {nh_core_setNextGlyph(&Glyphs_p, '>');}
            nh_core_setNextGlyph(&Glyphs_p, ' ');
            for (int i = 0; i < strlen(Node2_p->LoggerNode_p->name_p); ++i) {
                if (Node2_p->isSelected) {
                    nh_core_setNextReverseGlyph(&Glyphs_p, Node2_p->LoggerNode_p->name_p[i]);
                } else {
                    nh_core_setNextGlyph(&Glyphs_p, Node2_p->LoggerNode_p->name_p[i]);
                }
            }
            LastNode_p = Node2_p;
            if (Nodes2.size > i+1) {
                Monitor_p->nodesXOffset += 3;
            }
        }
    } else if (Nodes.size > 1 && (row-1) < Nodes.size && row < height/2) {
        nh_core_MonitorNode *Node_p = nh_core_getFromList(&Nodes, (row-1)+((nh_core_MonitorNode*)Nodes.pp[0])->Parent_p->nodesYOffset);
        if (Node_p) {
            for (int i = 0; i < Monitor_p->nodesXOffset; ++i) {
                nh_core_setNextGlyph(&Glyphs_p, ' ');
            }
            if (Node_p->isCurrent) {
                nh_core_setNextGlyph(&Glyphs_p, ' ');
                nh_core_setNextGlyph(&Glyphs_p, '>');
                nh_core_setNextGlyph(&Glyphs_p, ' ');
            }
            else if (Node_p->isSelected) {nh_core_setNextGlyph(&Glyphs_p, '>');}
            else {
                nh_core_setNextGlyph(&Glyphs_p, ' ');
                nh_core_setNextGlyph(&Glyphs_p, ' ');
                nh_core_setNextGlyph(&Glyphs_p, ' ');
            }

            for (int i = 0; i < strlen(Node_p->LoggerNode_p->name_p); ++i) {
                nh_core_setNextGlyph(&Glyphs_p, Node_p->LoggerNode_p->name_p[i]);
            }
        }
    } else if ((row-1) == Nodes.size || row == 2 || ((Nodes.size > height/2) && row == height/2)) {
        char title_p[] = " Log Viewer ";
        for (int i = 0; i < width - strlen(title_p); ++i) {
            nh_core_setNextGlyph(&Glyphs_p, '-');
        }
        for (int i = 0; i < strlen(title_p); ++i) {
            nh_core_setNextGlyph(&Glyphs_p, title_p[i]);
        }
    } else if (row == height - 2) {
        char title_p[255] = {0};
        if (Monitor_p->Peer.state > 0) {
            sprintf(title_p, " %s | 127.0.0.1@%d | %d logs ", nh_core_getConfig().monitorName_p, nh_core_getConfig().monitorPort, Monitor_p->Peer.Logger.totalLogs);
        } else {
            sprintf(title_p, " %s | %d logs ", nh_core_getConfig().monitorName_p, nh_core_getLogger()->totalLogs);
        }
        for (int i = 0; i < width - strlen(title_p); ++i) {
            nh_core_setNextGlyph(&Glyphs_p, '-');
        }
        for (int i = 0; i < strlen(title_p); ++i) {
            nh_core_setNextGlyph(&Glyphs_p, title_p[i]);
        }
    } else if (row == height - 1) {
        char footer_p[244] = {0};
        sprintf(footer_p, "[w][a][s][d] Navigate"); 
        for (int i = 0; i < strlen(footer_p); ++i) {
            nh_core_setNextGlyph(&Glyphs_p, footer_p[i]);
        }
    } else {
        NH_CORE_CHECK(nh_core_drawSelected(Monitor_p, &Glyphs_p, row - 3, width, height - 5))
    }

    nh_core_freeList(&Nodes, false);

    return NH_API_SUCCESS;
}

static void *nh_core_initMonitor(
    void *arg_p)
{
    nh_core_Monitor *Monitor_p = nh_core_allocate(sizeof(nh_core_Monitor));
    NH_CORE_CHECK_MEM_2(NULL, Monitor_p)

    memset(Monitor_p, 0, sizeof(nh_core_Monitor));

    Monitor_p->LastUpdate = nh_core_getSystemTime();
    Monitor_p->updateIntervalInSeconds = 1.0;

    Monitor_p->Root = nh_core_initMonitorNode(&(nh_core_getLogger()->Root), true, NULL);
    Monitor_p->Peer.Root = nh_core_initMonitorNode(NULL, true, NULL);

    nh_core_initLogger(&Monitor_p->Peer.Logger);

    return Monitor_p;
}

static void nh_core_destroyMonitor(
    void *handle_p)
{
    nh_core_free(handle_p);

    return;
}

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
