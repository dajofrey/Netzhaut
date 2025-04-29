#ifndef NH_HTML_NAVIGABLE_H
#define NH_HTML_NAVIGABLE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

// 7.3.1 and 7.3.1.1
typedef struct nh_html_Navigable {
    char id_p[255];
    struct nh_html_Navigable *Parent_p;
    nh_html_SessionHistoryEntry CurrentSessionHistoryEntry;
    nh_html_SessionHistoryEntry ActiveSessionHistoryEntry;
    bool isClosing;
    bool isDelayingLoadEvents;
    int currentSessionHistoryStep;
    nh_core_List SessionHistoryEntries;
    // TODO
    bool isTraversable // not in spec
} nh_html_Navigable;

#endif
